// #include "../include/huffman.h"
// #include <iostream>
// #include <queue>
// #include <vector>

// // Constructor for HuffmanNode
// HuffmanNode::HuffmanNode(char data, int frequency) {
//     left = right = nullptr;
//     this->data = data;
//     this->frequency = frequency;
// }

// // Comparison object for priority queue (min-heap)
// struct compare {
//     bool operator()(HuffmanNode* l, HuffmanNode* r) {
//         return l->frequency > r->frequency;
//     }
// };

// // Recursively generate Huffman codes
// void generateHuffmanCodes(HuffmanNode* root, std::string str, std::unordered_map<char, std::string>& huffmanCode) {
//     if (!root) return;

//     if (!root->left && !root->right) {
//         huffmanCode[root->data] = str;
//     }

//     generateHuffmanCodes(root->left, str + "0", huffmanCode);
//     generateHuffmanCodes(root->right, str + "1", huffmanCode);
// }

// // Build the Huffman Tree and generate codes
// std::unordered_map<char, std::string> buildHuffmanTree(const std::string& data) {
//     std::unordered_map<char, int> freq;
//     for (char ch : data) {
//         freq[ch]++;
//     }

//     std::priority_queue<HuffmanNode*, std::vector<HuffmanNode*>, compare> minHeap;
//     for (auto pair : freq) {
//         minHeap.push(new HuffmanNode(pair.first, pair.second));
//     }

//     while (minHeap.size() != 1) {
//         HuffmanNode* left = minHeap.top(); minHeap.pop();
//         HuffmanNode* right = minHeap.top(); minHeap.pop();
//         HuffmanNode* node = new HuffmanNode('\0', left->frequency + right->frequency);
//         node->left = left;
//         node->right = right;
//         minHeap.push(node);
//     }

//     HuffmanNode* root = minHeap.top();
//     std::unordered_map<char, std::string> huffmanCode;
//     generateHuffmanCodes(root, "", huffmanCode);
//     return huffmanCode;
// }

// // Compress the input data using the Huffman codes
// std::string compressUsingHuffman(const std::string& data, const std::unordered_map<char, std::string>& huffmanCode) {
//     std::string compressedData = "";
//     for (char ch : data) {
//         compressedData += huffmanCode.at(ch);
//     }
//     return compressedData;
// }

// // Print the Huffman codes
// void printHuffmanCodes(const std::unordered_map<char, std::string>& huffmanCode) {
//     std::cout << "Huffman Codes:\n";
//     for (auto pair : huffmanCode) {
//         std::cout << pair.first << " : " << pair.second << "\n";
//     }
// }
