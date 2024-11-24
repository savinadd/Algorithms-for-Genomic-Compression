// HuffmanGenome.h

#ifndef HUFFMANGENOME_H
#define HUFFMANGENOME_H

#include <iostream>
#include <string>
#include <queue>
#include <unordered_map>
#include "CompressionMetrics.h"
#include "Compressor.h"

// Huffman tree node structure for genomic data
struct HuffmanGenomeNode {
    char character;
    int frequency;
    HuffmanGenomeNode* left;
    HuffmanGenomeNode* right;

    HuffmanGenomeNode(char ch, int freq) : character(ch), frequency(freq), left(nullptr), right(nullptr) {}
};

// Comparator for the priority queue specific to genomic data
struct GenomeCompare {
    bool operator()(HuffmanGenomeNode* left, HuffmanGenomeNode* right) {
        return left->frequency > right->frequency;
    }
};

class HuffmanGenome : public Compressor {
public:
    HuffmanGenome();
    ~HuffmanGenome();

    // Encode the input sequence from a string
    void encode(const std::string& sequence);

    // Decode the encoded sequence from a string
    std::string decode(const std::string& encodedSequence) const;

    // Encode the input sequence from a file
    void encodeFromFile(const std::string& inputFilename, const std::string& outputFilename) override;

    // Decode the encoded sequence from a file
    void decodeFromFile(const std::string& inputFilename, const std::string& outputFilename) override;

    // Get the encoded sequence
    std::string getEncodedSequence() const;

    // Print the Huffman codes
    void printCodes() const;

    // Get compression metrics
    CompressionMetrics getMetrics() const override;

    // Save the frequency map to a file
    void saveFrequencyMap(const std::string& filename) const;

    // Load the frequency map from a file
    void loadFrequencyMap(const std::string& filename);

    // Validate the decoded file against the original file
    bool validateDecodedFile(const std::string& originalFilename, const std::string& decodedFilename) override;

private:
    void buildTree();
    void generateCodes(HuffmanGenomeNode* node, const std::string& code);
    void deleteTree(HuffmanGenomeNode* node);

    HuffmanGenomeNode* root;
    std::string huffmanCodes[256]; // Using ASCII codes as index
    std::string encodedSequence;
    int frequencyMap[256]; // Using ASCII codes as index

    CompressionMetrics metrics;

    size_t getFileSize(const std::string& filename);
};

#endif // HUFFMANGENOME_H
