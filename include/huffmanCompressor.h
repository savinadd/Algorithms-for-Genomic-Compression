// HuffmanCompressor.h

#ifndef HUFFMANCOMPRESSOR_H
#define HUFFMANCOMPRESSOR_H

#include <string>
#include <unordered_map>
#include "CompressionMetrics.h"
#include "Compressor.h"

// Huffman tree node structure
struct HuffmanNode {
    unsigned char byte;
    int frequency;
    HuffmanNode* left;
    HuffmanNode* right;

    HuffmanNode(unsigned char b, int freq) : byte(b), frequency(freq), left(nullptr), right(nullptr) {}
};

// Comparator for the priority queue
struct Compare {
    bool operator()(HuffmanNode* left, HuffmanNode* right) {
        return left->frequency > right->frequency;
    }
};

class HuffmanCompressor : public Compressor {
public:
    HuffmanCompressor();
    ~HuffmanCompressor();

    // Encode the input sequence from a file
    void encodeFromFile(const std::string& inputFilename, const std::string& outputFilename) override;

    // Decode the encoded sequence from a file
    void decodeFromFile(const std::string& inputFilename, const std::string& outputFilename) override;

    // Get compression metrics
    CompressionMetrics getMetrics() const override;

    // Validate the decoded file against the original file
    bool validateDecodedFile(const std::string& originalFilename, const std::string& decodedFilename) override;

private:
    void buildTree();
    void generateCodes(HuffmanNode* node, const std::string& code);
    void deleteTree(HuffmanNode* node);

    HuffmanNode* root;
    std::unordered_map<unsigned char, std::string> huffmanCodes; // Map bytes to codes
    std::unordered_map<unsigned char, int> frequencyMap;         // Map bytes to frequencies

    CompressionMetrics metrics;
};

#endif // HUFFMANCOMPRESSOR_H
