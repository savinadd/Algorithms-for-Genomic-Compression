#include "HuffmanGenome.h"
#include "Logger.h"
#include <fstream>
#include <sstream>
#include <cstring> 
#include <stdexcept>
#include <bitset>
#include <vector>

const size_t BUFFER_SIZE = 65536; // 64 KB buffer

HuffmanGenome::HuffmanGenome() : root(nullptr) {
    memset(frequencyMap, 0, sizeof(frequencyMap));
}

HuffmanGenome::~HuffmanGenome() {
    deleteTree(root);
}

void HuffmanGenome::deleteTree(HuffmanGenomeNode* node) {
    if (!node) return;
    deleteTree(node->left);
    deleteTree(node->right);
    delete node;
}

size_t HuffmanGenome::getFileSize(const std::string& filename) {
    std::ifstream infile(filename, std::ios::binary | std::ios::ate);
    if (!infile) {
        throw std::runtime_error("Error: Unable to open file '" + filename + "' to get file size.");
    }
    return static_cast<size_t>(infile.tellg());
}

void HuffmanGenome::encodeFromFile(const std::string& inputFilename, const std::string& outputFilename) {
    try {
        Logger::getInstance().log("Starting Huffman encoding...");

        // Reset internal state
        deleteTree(root);
        root = nullptr;
        memset(frequencyMap, 0, sizeof(frequencyMap));
        for (int i = 0; i < 256; ++i) huffmanCodes[i].clear();
        encodedSequence.clear();
        metrics = CompressionMetrics(); // Reset metrics

        // Open input file
        std::ifstream infile(inputFilename, std::ios::binary);
        if (!infile) {
            throw std::runtime_error("Error: Unable to open input file '" + inputFilename + "'.");
        }

        // Build frequency map
        char buffer[BUFFER_SIZE];
        while (infile.read(buffer, sizeof(buffer)) || infile.gcount()) {
            std::streamsize bytesRead = infile.gcount();

            for (std::streamsize i = 0; i < bytesRead; ++i) {
                char ch = buffer[i];
                if (ch == 'A' || ch == 'C' || ch == 'G' || ch == 'T') {
                    frequencyMap[(unsigned char)ch]++;
                } else {
                    throw std::runtime_error("Error: Invalid character '" + std::string(1, ch) + "' in input file.");
                }
            }
        }

        infile.clear();
        infile.seekg(0, std::ios::beg); // Rewind file

        // Build Huffman tree
        buildTree();

        // Open output file
        std::ofstream outfile(outputFilename, std::ios::binary);
        if (!outfile) {
            infile.close();
            throw std::runtime_error("Error: Unable to open output file '" + outputFilename + "'.");
        }

        // Encode and write to output file
        std::string bitBuffer;
        while (infile.read(buffer, sizeof(buffer)) || infile.gcount()) {
            std::streamsize bytesRead = infile.gcount();

            for (std::streamsize i = 0; i < bytesRead; ++i) {
                char ch = buffer[i];
                bitBuffer += huffmanCodes[(unsigned char)ch];
                // Write bytes to file when we have at least 8 bits
                while (bitBuffer.size() >= 8) {
                    std::bitset<8> byteBits(bitBuffer.substr(0, 8));
                    unsigned char byte = static_cast<unsigned char>(byteBits.to_ulong());
                    outfile.put(byte);
                    bitBuffer.erase(0, 8);
                }
            }
        }

        // Write remaining bits (if any)
        int paddingBits = 0;
        if (!bitBuffer.empty()) {
            paddingBits = 8 - bitBuffer.size();
            bitBuffer.append(paddingBits, '0'); // Pad with zeros
            std::bitset<8> byteBits(bitBuffer);
            unsigned char byte = static_cast<unsigned char>(byteBits.to_ulong());
            outfile.put(byte);
        }

        // Log padding bits added
        Logger::getInstance().log("Padding bits added during encoding: " + std::to_string(paddingBits));

        // Write padding information as the last byte
        outfile.put(static_cast<char>(paddingBits));

        infile.close();
        outfile.close();

        // Calculate compression metrics
        metrics.calculateOriginalSize(frequencyMap);
        metrics.calculateCompressedSizeFromFile(outputFilename, paddingBits);

        Logger::getInstance().log("Huffman encoding completed.");
    } catch (const std::exception& e) {
        Logger::getInstance().log(std::string("Exception during Huffman encoding: ") + e.what());
    }
}

void HuffmanGenome::decodeFromFile(const std::string& inputFilename, const std::string& outputFilename) {
    try {
        Logger::getInstance().log("Starting Huffman decoding...");

        if (!root) {
            throw std::runtime_error("Error: Huffman tree not built. Load frequency map or encode data first.");
        }

        // Open input file in binary mode and move to the end
        std::ifstream infile(inputFilename, std::ios::binary | std::ios::ate);
        if (!infile) {
            throw std::runtime_error("Error: Unable to open input file '" + inputFilename + "'.");
        }

        // Get the total file size
        std::streamsize fileSize = infile.tellg();
        if (fileSize < 1) {
            throw std::runtime_error("Error: Encoded file is too small.");
        }

        // Read padding bits count from the last byte
        infile.seekg(fileSize - 1, std::ios::beg);
        char paddingBitsChar;
        infile.get(paddingBitsChar);
        int paddingBits = static_cast<unsigned char>(paddingBitsChar);
        Logger::getInstance().log("Padding bits read during decoding: " + std::to_string(paddingBits));

        if (paddingBits < 0 || paddingBits > 7) {
            throw std::runtime_error("Error: Invalid padding bits value in encoded file.");
        }

        // Calculate the size of the data excluding the padding bits byte
        std::streamsize dataSize = fileSize - 1;
        infile.seekg(0, std::ios::beg); // Reset to the beginning

        // Read the encoded data into a buffer
        std::vector<char> buffer(dataSize);
        infile.read(buffer.data(), dataSize);
        infile.close();

        // Convert buffer to bit string
        std::string bitString;
        for (std::streamsize i = 0; i < dataSize; ++i) {
            std::bitset<8> bits(static_cast<unsigned char>(buffer[i]));
            bitString += bits.to_string();
        }

        // Remove padding bits from the end
        if (paddingBits > 0) {
            bitString.erase(bitString.end() - paddingBits, bitString.end());
        }

        Logger::getInstance().log("Total bits to process: " + std::to_string(bitString.size()));

        // Open output file
        std::ofstream outfile(outputFilename, std::ios::binary);
        if (!outfile) {
            throw std::runtime_error("Error: Unable to open output file '" + outputFilename + "'.");
        }

        // Decode the bit string
        HuffmanGenomeNode* currentNode = root;
        for (size_t i = 0; i < bitString.size(); ++i) {
            char bitChar = bitString[i];
            if (bitChar == '0') {
                currentNode = currentNode->left;
            } else if (bitChar == '1') {
                currentNode = currentNode->right;
            } else {
                throw std::runtime_error("Error: Invalid bit character in bit string.");
            }

            if (!currentNode) {
                throw std::runtime_error("Error: Decoding failed. Invalid path in Huffman tree.");
            }

            // If it's a leaf node
            if (!currentNode->left && !currentNode->right) {
                outfile.put(currentNode->character);
                currentNode = root;
            }
        }

        outfile.close();

        Logger::getInstance().log("Huffman decoding completed.");
    } catch (const std::exception& e) {
        Logger::getInstance().log(std::string("Exception during Huffman decoding: ") + e.what());
    }
}

void HuffmanGenome::encode(const std::string& sequence) {
    // Implementation can be added if needed
}

std::string HuffmanGenome::decode(const std::string& encodedSequence) const {
    // Implementation can be added if needed
    return "";
}

void HuffmanGenome::buildTree() {
    std::priority_queue<HuffmanGenomeNode*, std::vector<HuffmanGenomeNode*>, GenomeCompare> pq;

    for (int i = 0; i < 256; ++i) {
        if (frequencyMap[i] > 0) {
            pq.push(new HuffmanGenomeNode((char)i, frequencyMap[i]));
        }
    }

    // Edge case: If there's only one unique character
    if (pq.size() == 1) {
        HuffmanGenomeNode* onlyNode = pq.top(); pq.pop();
        root = new HuffmanGenomeNode('\0', onlyNode->frequency);
        root->left = onlyNode;
        generateCodes(root, "");
        return;
    }

    while (pq.size() > 1) {
        HuffmanGenomeNode* left = pq.top(); pq.pop();
        HuffmanGenomeNode* right = pq.top(); pq.pop();

        int sum = left->frequency + right->frequency;
        HuffmanGenomeNode* newNode = new HuffmanGenomeNode('\0', sum);
        newNode->left = left;
        newNode->right = right;
        pq.push(newNode);
    }

    if (!pq.empty()) {
        root = pq.top();
        generateCodes(root, "");
    }
}

void HuffmanGenome::generateCodes(HuffmanGenomeNode* node, const std::string& code) {
    if (!node) return;

    if (!node->left && !node->right) {
        huffmanCodes[(unsigned char)node->character] = code;
        // Log the code for each character
        Logger::getInstance().log(std::string("Character '") + node->character + "' has code: " + code);
    }

    generateCodes(node->left, code + "0");
    generateCodes(node->right, code + "1");
}

std::string HuffmanGenome::getEncodedSequence() const {
    return encodedSequence;
}

void HuffmanGenome::printCodes() const {
    for (char ch : {'A', 'C', 'G', 'T'}) {
        if (!huffmanCodes[(unsigned char)ch].empty()) {
            std::cout << ch << ": " << huffmanCodes[(unsigned char)ch] << std::endl;
        }
    }
}

CompressionMetrics HuffmanGenome::getMetrics() const {
    return metrics;
}

void HuffmanGenome::saveFrequencyMap(const std::string& filename) const {
    std::ofstream outfile(filename);
    if (!outfile) {
        throw std::runtime_error("Error: Unable to open file '" + filename + "' for writing frequency map.");
    }
    for (char ch : {'A', 'C', 'G', 'T'}) {
        outfile << ch << " " << frequencyMap[(unsigned char)ch] << "\n";
    }
    outfile.close();
}

void HuffmanGenome::loadFrequencyMap(const std::string& filename) {
    try {
        // Reset internal state
        deleteTree(root);
        root = nullptr;
        memset(frequencyMap, 0, sizeof(frequencyMap));
        for (int i = 0; i < 256; ++i) huffmanCodes[i].clear();
        encodedSequence.clear();
        metrics = CompressionMetrics(); // Reset metrics

        std::ifstream infile(filename);
        if (!infile) {
            throw std::runtime_error("Error: Unable to open file '" + filename + "' for reading frequency map.");
        }
        char ch;
        int freq;
        while (infile >> ch >> freq) {
            frequencyMap[(unsigned char)ch] = freq;
        }
        infile.close();

        // Rebuild the Huffman tree with the loaded frequency map
        buildTree();
    } catch (const std::exception& e) {
        Logger::getInstance().log(std::string("Exception during loading frequency map: ") + e.what());
    }
}

bool HuffmanGenome::validateDecodedFile(const std::string& originalFilename, const std::string& decodedFilename) {
    try {
        Logger::getInstance().log("Validating decoded file...");

        const size_t BUFFER_SIZE = 65536; // 64 KB buffer
        std::ifstream originalFile(originalFilename, std::ios::binary);
        std::ifstream decodedFile(decodedFilename, std::ios::binary);

        if (!originalFile.is_open()) {
            throw std::runtime_error("Error: Unable to open original file '" + originalFilename + "'.");
        }
        if (!decodedFile.is_open()) {
            throw std::runtime_error("Error: Unable to open decoded file '" + decodedFilename + "'.");
        }

        // Get sizes of both files
        originalFile.seekg(0, std::ios::end);
        std::streamsize originalFileSize = originalFile.tellg();
        originalFile.seekg(0, std::ios::beg);

        decodedFile.seekg(0, std::ios::end);
        std::streamsize decodedFileSize = decodedFile.tellg();
        decodedFile.seekg(0, std::ios::beg);

        Logger::getInstance().log("Original file size: " + std::to_string(originalFileSize));
        Logger::getInstance().log("Decoded file size: " + std::to_string(decodedFileSize));

        // Allocate buffers on the heap
        std::vector<char> originalBuffer(BUFFER_SIZE);
        std::vector<char> decodedBuffer(BUFFER_SIZE);

        while (true) {
            // Read from original file
            originalFile.read(originalBuffer.data(), BUFFER_SIZE);
            std::streamsize originalBytesRead = originalFile.gcount();

            // Read from decoded file
            decodedFile.read(decodedBuffer.data(), BUFFER_SIZE);
            std::streamsize decodedBytesRead = decodedFile.gcount();

            // Check if bytes read are equal
            if (originalBytesRead != decodedBytesRead) {
                Logger::getInstance().log("Error: Files have different sizes.");
                return false;
            }

            // If no bytes were read, we've reached EOF for both files
            if (originalBytesRead == 0) {
                break;
            }

            // Compare the buffers
            if (std::memcmp(originalBuffer.data(), decodedBuffer.data(), static_cast<size_t>(originalBytesRead)) != 0) {
                Logger::getInstance().log("Error: Files differ.");
                return false;
            }
        }

        Logger::getInstance().log("Validation successful. Decoded file matches the original.");
        return true;
    } catch (const std::exception& e) {
        Logger::getInstance().log(std::string("Exception during validation: ") + e.what());
        return false;
    }
}
