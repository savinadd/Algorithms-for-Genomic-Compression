#ifndef HUFFMANGENOME_H
#define HUFFMANGENOME_H

#include <iostream>
#include <string>
#include <queue>
#include <array>
#include "CompressionMetrics.h"
#include "Compressor.h"

struct HuffmanGenomeNode {
    char character;
    int frequency;
    HuffmanGenomeNode* left;
    HuffmanGenomeNode* right;

    HuffmanGenomeNode(char ch, int freq) 
        : character(ch), frequency(freq), left(nullptr), right(nullptr) {}
};

struct GenomeCompare {
    bool operator()(HuffmanGenomeNode* left, HuffmanGenomeNode* right) {
        return left->frequency > right->frequency;
    }
};

class HuffmanGenome : public Compressor {
public:
    // Constructor and Destructor
    HuffmanGenome();
    ~HuffmanGenome() override;

    void encode(const std::string& sequence);
    std::string decode(const std::string& encodedSequence) const;
    std::string getEncodedSequence() const; 

    void encodeFromFile(const std::string& inputFilename, const std::string& outputFilename) override;
    void decodeFromFile(const std::string& inputFilename, const std::string& outputFilename) override;
    CompressionMetrics getMetrics() const override;
    bool validateDecodedFile(const std::string& originalFilename, const std::string& decodedFilename) override;

    void printCodes() const;
    void saveFrequencyMap(const std::string& filename) const;
    void loadFrequencyMap(const std::string& filename);

    bool validateInputFile(const std::string& inputFilename) const override;

private:

    void buildTree();
    void generateCodes(HuffmanGenomeNode* node, const std::string& code);
    void deleteTree(HuffmanGenomeNode* node);

    HuffmanGenomeNode* root;
    std::array<std::string, 4> huffmanCodes; 
    std::string encodedSequence;
    
    enum GenomeBase { A = 0, C, G, T, BASE_COUNT };
    std::array<unsigned int, BASE_COUNT> frequencyMap;

    CompressionMetrics metrics;

    size_t getFileSize(const std::string& filename);

    int charToIndex(char ch) const;
};

#endif
