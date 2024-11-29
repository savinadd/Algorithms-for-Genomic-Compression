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
    bool operator()(HuffmanNode* left, HuffmanNode* right) const {
        if (left->frequency != right->frequency) {
            return left->frequency > right->frequency; // Min-heap based on frequency
        }
        return left->byte > right->byte; // Tiebreaker: lex order
    }
};

class HuffmanCompressor : public Compressor {
public:
    HuffmanCompressor();
    ~HuffmanCompressor() override;

    void encodeFromFile(const std::string& inputFilename, const std::string& outputFilename) override;
    void decodeFromFile(const std::string& inputFilename, const std::string& outputFilename) override;
    CompressionMetrics getMetrics() const override;
    bool validateDecodedFile(const std::string& originalFilename, const std::string& decodedFilename) override;
    bool validateInputFile(const std::string& inputFilename) const override;
    void saveFrequencyMap(const std::string& freqFilename);
    void loadFrequencyMap(const std::string& freqFilename);
    std::unordered_map<unsigned char, int> frequencyMap;  
    

private:

    void buildTree();
    void generateCodes(HuffmanNode* node, const std::string& code);
    void deleteTree(HuffmanNode* node);

    HuffmanNode* root;
    std::unordered_map<unsigned char, std::string> huffmanCodes; // Map bytes to codes
   // std::unordered_map<unsigned char, int> frequencyMap;         // Map bytes to frequencies

    CompressionMetrics metrics;
};

#endif 
