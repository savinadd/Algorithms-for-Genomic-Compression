#include "HuffmanGenome.h"
#include <fstream>
#include <cctype> // For isdigit

HuffmanGenome::HuffmanGenome() : root(nullptr), maxCodeLength(0) {
    frequencyMap = {{'A', 0}, {'C', 0}, {'T', 0}, {'G', 0}};
}

HuffmanGenome::~HuffmanGenome() {
    deleteTree(root);
}

void HuffmanGenome::deleteTree(HuffmanNode* node) {
    if (!node) return;
    deleteTree(node->left);
    deleteTree(node->right);
    delete node;
}

void HuffmanGenome::encode(const std::string& sequence) {
    // Reset internal state
    deleteTree(root);
    root = nullptr;
    frequencyMap = {{'A', 0}, {'C', 0}, {'T', 0}, {'G', 0}};
    huffmanCodes.clear();
    reverseCodes.clear();
    encodedSequence.clear();
    maxCodeLength = 0;

    // Validate input sequence and build frequency map
    for (char ch : sequence) {
        if (frequencyMap.find(ch) != frequencyMap.end()) {
            frequencyMap[ch]++;
        } else {
            std::cerr << "Error: Invalid character '" << ch << "' in sequence." << std::endl;
            return;
        }
    }

    buildTree();

    // Encode the sequence
    for (char ch : sequence) {
        encodedSequence += huffmanCodes[ch];
    }
}

void HuffmanGenome::buildTree() {
    std::priority_queue<HuffmanNode*, std::vector<HuffmanNode*>, Compare> pq;

    for (const auto& pair : frequencyMap) {
        if (pair.second > 0) {
            pq.push(new HuffmanNode(pair.first, pair.second));
        }
    }

    // Edge case: If there's only one unique character
    if (pq.size() == 1) {
        HuffmanNode* onlyNode = pq.top(); pq.pop();
        root = new HuffmanNode('\0', onlyNode->frequency);
        root->left = onlyNode;
        generateCodes(root, "");
        return;
    }

    while (pq.size() > 1) {
        HuffmanNode* left = pq.top(); pq.pop();
        HuffmanNode* right = pq.top(); pq.pop();

        int sum = left->frequency + right->frequency;
        HuffmanNode* newNode = new HuffmanNode('\0', sum);
        newNode->left = left;
        newNode->right = right;
        pq.push(newNode);
    }

    root = pq.top();
    generateCodes(root, "");
}

void HuffmanGenome::generateCodes(HuffmanNode* node, const std::string& code) {
    if (!node) return;

    if (!node->left && !node->right) {
        huffmanCodes[node->character] = code;
        reverseCodes[code] = node->character;

        // Update max code length for error handling
        if (code.length() > maxCodeLength) {
            maxCodeLength = code.length();
        }
    }

    generateCodes(node->left, code + "0");
    generateCodes(node->right, code + "1");
}

std::string HuffmanGenome::decode(const std::string& encodedSequence) const {
    std::string decodedSequence = "";
    std::string currentCode = "";

    for (char bit : encodedSequence) {
        if (bit != '0' && bit != '1') {
            std::cerr << "Error: Invalid bit '" << bit << "' in encoded sequence." << std::endl;
            return "";
        }

        currentCode += bit;
        auto it = reverseCodes.find(currentCode);
        if (it != reverseCodes.end()) {
            decodedSequence += it->second;
            currentCode.clear();
        }
        // Handle invalid codes
        else if (currentCode.length() > maxCodeLength) {
            std::cerr << "Error: Invalid encoded sequence at code '" << currentCode << "'." << std::endl;
            return "";
        }
    }

    // Check if there is any remaining code that couldn't be decoded
    if (!currentCode.empty()) {
        std::cerr << "Error: Incomplete encoded sequence at the end." << std::endl;
        return "";
    }

    return decodedSequence;
}

std::string HuffmanGenome::getEncodedSequence() const {
    return encodedSequence;
}

void HuffmanGenome::printCodes() const {
    for (const auto& pair : huffmanCodes) {
        std::cout << pair.first << ": " << pair.second << std::endl;
    }
}

double HuffmanGenome::getCompressionRatio() const {
    // Calculate original size
    int originalSize = 0;
    for (const auto& pair : frequencyMap) {
        originalSize += pair.second * 8; // Assuming 2 bits per nucleotide ('A', 'C', 'G', 'T')
    }
    
    // Log frequency map and original size
    std::cout << "Frequency Map Contents:\n";
    for (const auto& pair : frequencyMap) {
        std::cout << "Nucleotide: " << pair.first << ", Frequency: " << pair.second << "\n";
    }
    std::cout << "Calculated Original Size (in bits): " << originalSize << "\n";

    std::cout << "Huffman Codes:\n";
    for (const auto& pair : huffmanCodes) {
    std::cout << "Nucleotide: " << pair.first << ", Code: " << pair.second << "\n";
}

    // Calculate compressed size
    int compressedSize = encodedSequence.size(); // Size in bits
    
    // Log encoded sequence and compressed size
    std::cout << "Encoded Sequence Length (in bits): " << compressedSize << "\n";

    // Handle edge case where compressed sequence size is zero
    if (compressedSize == 0) {
        std::cout << "Compressed size is zero. Returning compression ratio as 0.0.\n";
        return 0.0;
    }

    // Calculate and log compression ratio
    double compressionRatio = static_cast<double>(originalSize) / compressedSize;
    std::cout << "Compression Ratio: " << compressionRatio << "\n";

    return compressionRatio;
}


// Save the frequency map to a file
void HuffmanGenome::saveFrequencyMap(const std::string& filename) const {
    std::ofstream outfile(filename);
    if (!outfile) {
        std::cerr << "Error: Unable to open file '" << filename << "' for writing frequency map." << std::endl;
        return;
    }
    for (const auto& pair : frequencyMap) {
        outfile << pair.first << " " << pair.second << "\n";
    }
    outfile.close();
}

// Load the frequency map from a file and rebuild the Huffman tree
void HuffmanGenome::loadFrequencyMap(const std::string& filename) {
    // Reset internal state
    deleteTree(root);
    root = nullptr;
    frequencyMap.clear();
    huffmanCodes.clear();
    reverseCodes.clear();
    encodedSequence.clear();
    maxCodeLength = 0;

    std::ifstream infile(filename);
    if (!infile) {
        std::cerr << "Error: Unable to open file '" << filename << "' for reading frequency map." << std::endl;
        return;
    }
    char ch;
    int freq;
    while (infile >> ch >> freq) {
        frequencyMap[ch] = freq;
    }
    infile.close();

    // Rebuild the Huffman tree with the loaded frequency map
    buildTree();
}
