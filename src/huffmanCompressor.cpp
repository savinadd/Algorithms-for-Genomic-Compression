#include "HuffmanCompressor.h"
#include "Logger.h"
#include <fstream>
#include <sstream>
#include <stdexcept>
#include <bitset>
#include <vector>
#include <cstring>
#include <queue>

const size_t BUFFER_SIZE = 65536; // 64 KB buffer

HuffmanCompressor::HuffmanCompressor() : root(nullptr) {}

HuffmanCompressor::~HuffmanCompressor() {
    deleteTree(root);
}

void HuffmanCompressor::deleteTree(HuffmanNode* node) {
    if (!node) return;
    deleteTree(node->left);
    deleteTree(node->right);
    delete node;
}

void HuffmanCompressor::encodeFromFile(const std::string& inputFilename, const std::string& outputFilename) {
    try {
        Logger::getInstance().log("Starting Huffman encoding...");

        deleteTree(root);
        root = nullptr;
        huffmanCodes.clear();
        frequencyMap.clear();
        metrics = CompressionMetrics(); 

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
                unsigned char byte = static_cast<unsigned char>(buffer[i]);
                frequencyMap[byte]++;
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
                unsigned char byte = static_cast<unsigned char>(buffer[i]);
                bitBuffer += huffmanCodes[byte];
                // Write bytes to file when we have at least 8 bits
                while (bitBuffer.size() >= 8) {
                    std::bitset<8> byteBits(bitBuffer.substr(0, 8));
                    unsigned char outputByte = static_cast<unsigned char>(byteBits.to_ulong());
                    outfile.put(outputByte);
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
        // Original size is total bits from input file
        std::ifstream originalFile(inputFilename, std::ios::binary | std::ios::ate);
        if (originalFile) {
            std::streamsize size = originalFile.tellg();
            metrics.calculateOriginalSize(size * 8); // Total bits
            originalFile.close();
        }

        metrics.calculateCompressedSizeFromFile(outputFilename, paddingBits);

        Logger::getInstance().log("Huffman encoding completed.");
    } catch (const std::exception& e) {
        Logger::getInstance().log(std::string("Exception during Huffman encoding: ") + e.what());
    }
}

void HuffmanCompressor::decodeFromFile(const std::string& inputFilename, const std::string& outputFilename) {
    try {
        Logger::getInstance().log("Starting Huffman decoding...");  

        // Ensure that the Huffman tree has been built
        if (!root) {
            throw std::runtime_error("Error: Huffman tree not built. Load frequency map or encode data first.");
        }

        // Open input file
        std::ifstream infile(inputFilename, std::ios::binary);
        if (!infile) {
            throw std::runtime_error("Error: Unable to open input file '" + inputFilename + "'.");
        }

        // Open output file
        std::ofstream outfile(outputFilename, std::ios::binary);
        if (!outfile) {
            infile.close();
            throw std::runtime_error("Error: Unable to open output file '" + outputFilename + "'.");
        }

        // Read padding bits count from the last byte
        infile.seekg(-1, std::ios::end);
        char paddingBitsChar;
        infile.get(paddingBitsChar);
        int paddingBits = static_cast<unsigned char>(paddingBitsChar);
        Logger::getInstance().log("Padding bits read during decoding: " + std::to_string(paddingBits));

        if (paddingBits < 0 || paddingBits > 7) {
            throw std::runtime_error("Error: Invalid padding bits value in encoded file.");
        }

        size_t fileSize = infile.tellg(); // Position before padding bits byte
        infile.seekg(0, std::ios::beg);   // Reset to the beginning

        // Read the encoded data into a bit string
        std::string bitString;
        char ch;
        size_t bytesRead = 0;
        while (bytesRead < fileSize && infile.get(ch)) {
            std::bitset<8> bits(static_cast<unsigned char>(ch));
            bitString += bits.to_string();
            bytesRead++;
        }
        infile.close();

        // Remove padding bits from the end
        if (paddingBits > 0) {
            bitString.erase(bitString.end() - paddingBits, bitString.end());
        }

        // Decode the bit string
        HuffmanNode* currentNode = root;
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
                outfile.put(static_cast<char>(currentNode->byte));
                currentNode = root;
            }
        }

        outfile.close();

        Logger::getInstance().log("Huffman decoding completed.");
    } catch (const std::exception& e) {
        Logger::getInstance().log(std::string("Exception during Huffman decoding: ") + e.what());
    }
}

void HuffmanCompressor::buildTree() {
    std::priority_queue<HuffmanNode*, std::vector<HuffmanNode*>, Compare> pq;

    for (const auto& entry : frequencyMap) {
        pq.push(new HuffmanNode(entry.first, entry.second));
    }

    while (pq.size() > 1) {
        HuffmanNode* left = pq.top(); pq.pop();
        HuffmanNode* right = pq.top(); pq.pop();

        int sum = left->frequency + right->frequency;
        HuffmanNode* newNode = new HuffmanNode(0, sum);
        newNode->left = left;
        newNode->right = right;
        pq.push(newNode);
    }

    if (!pq.empty()) {
        root = pq.top();
        generateCodes(root, "");
    }
}

void HuffmanCompressor::generateCodes(HuffmanNode* node, const std::string& code) {
    if (!node) return;

    if (!node->left && !node->right) {
        huffmanCodes[node->byte] = code;
        // Log the code for each byte
        Logger::getInstance().log("Byte '" + std::to_string(node->byte) + "' has code: " + code);
    }

    generateCodes(node->left, code + "0");
    generateCodes(node->right, code + "1");
}

CompressionMetrics HuffmanCompressor::getMetrics() const {
    return metrics;
}

bool HuffmanCompressor::validateDecodedFile(const std::string& originalFilename, const std::string& decodedFilename) {
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
