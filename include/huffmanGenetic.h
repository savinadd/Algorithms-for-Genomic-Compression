#ifndef HUFFMAN_H
#define HUFFMAN_H

#include <string>

// Huffman Tree Node
struct HuffmanNode {
    char data;
    int frequency;
    HuffmanNode* left;
    HuffmanNode* right;

    HuffmanNode(char data, int frequency);
    ~HuffmanNode(); // Destructor to delete child nodes
};

// Function declarations
void buildHuffmanTree(const std::string& data, std::string huffmanCode[4]);
std::string compressUsingHuffman(const std::string& data, const std::string huffmanCode[4]);
void printHuffmanCodes(const std::string huffmanCode[4]);

#endif
