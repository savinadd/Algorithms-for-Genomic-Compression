#include "../include/HuffmanGenome.h"

HuffmanGenome::HuffmanGenome() : root(nullptr) {
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
    for (char ch : sequence) {
        if (frequencyMap.find(ch) != frequencyMap.end()) {
            frequencyMap[ch]++;
        }
    }

    buildTree();

    encodedSequence.clear();
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
    }

    generateCodes(node->left, code + "0");
    generateCodes(node->right, code + "1");
}

std::string HuffmanGenome::decode(const std::string& encodedSequence) const {
    std::string decodedSequence = "";
    std::string currentCode = "";

    for (char bit : encodedSequence) {
        currentCode += bit;
        if (reverseCodes.find(currentCode) != reverseCodes.end()) {
            decodedSequence += reverseCodes.at(currentCode);
            currentCode = "";
        }
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

// Calculate and return the compression ratio
double HuffmanGenome::getCompressionRatio() const {
    int originalSize = (frequencyMap.at('A') + frequencyMap.at('C') + frequencyMap.at('T') + frequencyMap.at('G')) * 8;

    int compressedSize = encodedSequence.size(); // Size in bits

    if (compressedSize == 0) return 0.0; // Avoid division by zero
    return static_cast<double>(originalSize) / compressedSize;
}
