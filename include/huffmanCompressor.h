#ifndef HUFFMANCOMPRESSOR_H
#define HUFFMANCOMPRESSOR_H

#include <string>
#include <unordered_map>
#include "CompressionMetrics.h"
#include "Compressor.h"

struct HuffmanNode {
    unsigned char byte;
    int frequency;
    HuffmanNode* left;
    HuffmanNode* right;

    HuffmanNode(unsigned char b, int freq) 
        : byte(b), frequency(freq), left(nullptr), right(nullptr) {}
};

struct Compare {
    bool operator()(HuffmanNode* left, HuffmanNode* right) {
        return left->frequency > right->frequency;
    }
};

class HuffmanCompressor : public Compressor {
public:
    // Constructor and Destructor
    HuffmanCompressor();
    ~HuffmanCompressor() override;

    // Overridden methods from Compressor interface
    void encodeFromFile(const std::string& inputFilename, const std::string& outputFilename) override;
    void decodeFromFile(const std::string& inputFilename, const std::string& outputFilename) override;
    CompressionMetrics getMetrics() const override;
    bool validateDecodedFile(const std::string& originalFilename, const std::string& decodedFilename) override;

private:
    // helpers
    void buildTree();
    void generateCodes(HuffmanNode* node, const std::string& code);
    void deleteTree(HuffmanNode* node);

    HuffmanNode* root;
    std::unordered_map<unsigned char, std::string> huffmanCodes; // Map bytes to codes
    std::unordered_map<unsigned char, int> frequencyMap;         // Map bytes to frequencies

    CompressionMetrics metrics;
};

#endif 
