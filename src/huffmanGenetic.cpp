#include "huffmanGenetic.h"
#include <iostream>
#include <queue>
#include <vector>

// Mapping functions
int charToIndex(char ch) {
    switch (ch) {
    case 'A': return 0;
    case 'C': return 1;
    case 'G': return 2;
    case 'T': return 3;
    default: return -1; // Error handling
    }
}

char indexToChar(int index) {
    switch (index) {
    case 0: return 'A';
    case 1: return 'C';
    case 2: return 'G';
    case 3: return 'T';
    default: return '\0'; // Error handling
    }
}

// Destructor for HuffmanNode
HuffmanNode::~HuffmanNode() {
    delete left;
    delete right;
}

// Constructor for HuffmanNode
HuffmanNode::HuffmanNode(char data, int frequency) {
    left = right = nullptr;
    this->data = data;
    this->frequency = frequency;
}

// Comparison object for priority queue (min-heap)
struct compare {
    bool operator()(HuffmanNode* l, HuffmanNode* r) {
        return l->frequency > r->frequency;
    }
};

// Recursively generate Huffman codes
void generateHuffmanCodes(HuffmanNode* root, std::string str, std::string huffmanCode[4]) {
    if (!root) return;

    if (!root->left && !root->right) {
        int index = charToIndex(root->data);
        if (index != -1) {
            huffmanCode[index] = str;
        }
    }

    generateHuffmanCodes(root->left, str + "0", huffmanCode);
    generateHuffmanCodes(root->right, str + "1", huffmanCode);
}

// Build the Huffman Tree and generate codes
void buildHuffmanTree(const std::string& data, std::string huffmanCode[4]) {
    int freq[4] = { 0 };
    for (char ch : data) {
        int index = charToIndex(ch);
        if (index != -1) {
            freq[index]++;
        }
    }

    std::priority_queue<HuffmanNode*, std::vector<HuffmanNode*>, compare> minHeap;
    for (int i = 0; i < 4; ++i) {
        if (freq[i] > 0) {
            minHeap.push(new HuffmanNode(indexToChar(i), freq[i]));
        }
    }

    // Edge case: If there's only one type of character
    if (minHeap.size() == 1) {
        HuffmanNode* node = minHeap.top(); minHeap.pop();
        HuffmanNode* root = new HuffmanNode('\0', node->frequency);
        root->left = node;
        generateHuffmanCodes(root, "", huffmanCode);
        delete root;
        return;
    }

    while (minHeap.size() > 1) {
        HuffmanNode* left = minHeap.top(); minHeap.pop();
        HuffmanNode* right = minHeap.top(); minHeap.pop();
        HuffmanNode* node = new HuffmanNode('\0', left->frequency + right->frequency);
        node->left = left;
        node->right = right;
        minHeap.push(node);
    }

    HuffmanNode* root = minHeap.top();

    generateHuffmanCodes(root, "", huffmanCode);

    // Clean up the Huffman tree
    delete root;
}

// Compress the input data using the Huffman codes
std::string compressUsingHuffman(const std::string& data, const std::string huffmanCode[4]) {
    std::string compressedData = "";
    for (char ch : data) {
        int index = charToIndex(ch);
        if (index != -1) {
            compressedData += huffmanCode[index];
        }
    }
    return compressedData;
}

// Print the Huffman codes
void printHuffmanCodes(const std::string huffmanCode[4]) {
    std::cout << "Huffman Codes:\n";
    for (int i = 0; i < 4; ++i) {
        if (!huffmanCode[i].empty()) {
            std::cout << indexToChar(i) << " : " << huffmanCode[i] << "\n";
        }
    }
}
