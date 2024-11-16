#ifndef HUFFMANGENOME_H
#define HUFFMANGENOME_H

#include <iostream>
#include <unordered_map>
#include <vector>
#include <string>
#include <queue>

struct HuffmanNode {
    char character;
    int frequency;
    HuffmanNode* left;
    HuffmanNode* right;

    HuffmanNode(char ch, int freq) : character(ch), frequency(freq), left(nullptr), right(nullptr) {}
};

struct Compare {
    bool operator()(HuffmanNode* left, HuffmanNode* right) {
        return left->frequency > right->frequency;
    }
};

class HuffmanGenome {
public:
    HuffmanGenome();
    ~HuffmanGenome();

    void encode(const std::string& sequence);
    std::string decode(const std::string& encodedSequence) const;
    std::string getEncodedSequence() const;
    void printCodes() const;
    void saveFrequencyMap(const std::string& filename) const;
    void loadFrequencyMap(const std::string& filename);


    // Calculate and return the compression ratio
    double getCompressionRatio() const;

private:
    void buildTree();
    void generateCodes(HuffmanNode* node, const std::string& code);
    void deleteTree(HuffmanNode* node);

    HuffmanNode* root;
    std::unordered_map<char, std::string> huffmanCodes;
    std::unordered_map<std::string, char> reverseCodes;
    std::string encodedSequence;
    std::unordered_map<char, int> frequencyMap;

    // Maximum length of Huffman codes, used for error handling in decoding
    size_t maxCodeLength;
};

#endif // HUFFMANGENOME_H
