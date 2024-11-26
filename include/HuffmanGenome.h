#ifndef HUFFMANGENOME_H
#define HUFFMANGENOME_H

#include <iostream>
#include <string>
#include <queue>
#include <unordered_map>
#include "CompressionMetrics.h"
#include "Compressor.h"

struct HuffmanGenomeNode {
    char character;
    int frequency;
    HuffmanGenomeNode* left;
    HuffmanGenomeNode* right;

    HuffmanGenomeNode(char ch, int freq) : character(ch), frequency(freq), left(nullptr), right(nullptr) {}
};

struct GenomeCompare {
    bool operator()(HuffmanGenomeNode* left, HuffmanGenomeNode* right) {
        return left->frequency > right->frequency;
    }
};

class HuffmanGenome : public Compressor {
public:
    HuffmanGenome();
    ~HuffmanGenome();

    void encode(const std::string& sequence);

    std::string decode(const std::string& encodedSequence) const;


    void encodeFromFile(const std::string& inputFilename, const std::string& outputFilename) override;

    void decodeFromFile(const std::string& inputFilename, const std::string& outputFilename) override;

    std::string getEncodedSequence() const;


    void printCodes() const;

    CompressionMetrics getMetrics() const override;


    void saveFrequencyMap(const std::string& filename) const;

    void loadFrequencyMap(const std::string& filename);

    bool validateDecodedFile(const std::string& originalFilename, const std::string& decodedFilename) override;

private:
    void buildTree();
    void generateCodes(HuffmanGenomeNode* node, const std::string& code);
    void deleteTree(HuffmanGenomeNode* node);

    HuffmanGenomeNode* root;
    std::string huffmanCodes[256]; 
    std::string encodedSequence;
    int frequencyMap[256]; 

    CompressionMetrics metrics;

    size_t getFileSize(const std::string& filename);
};

#endif 
