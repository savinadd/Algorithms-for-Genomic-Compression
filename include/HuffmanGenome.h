#ifndef HUFFMANGENOME_H
#define HUFFMANGENOME_H

#include <iostream>
#include <unordered_map>
#include <string>
#include <queue>

// Huffman tree node structure
struct HuffmanNode {
    char character;
    int frequency;
    HuffmanNode* left;
    HuffmanNode* right;

    HuffmanNode(char ch, int freq) : character(ch), frequency(freq), left(nullptr), right(nullptr) {}
};

// Comparator for the priority queue
struct Compare {
    bool operator()(HuffmanNode* left, HuffmanNode* right) {
        return left->frequency > right->frequency;
    }
};

class HuffmanGenome {
public:
    HuffmanGenome();
    ~HuffmanGenome();

    // Encode the input sequence
    void encode(const std::string& sequence);

    // Decode the encoded sequence
    std::string decode(const std::string& encodedSequence) const;

    // Get the encoded sequence
    std::string getEncodedSequence() const;

    // Print the Huffman codes
    void printCodes() const;

    // Calculate and return the compression ratio
    double getCompressionRatio() const;

    // Save the frequency map to a file
    void saveFrequencyMap(const std::string& filename) const;

    // Load the frequency map from a file
    void loadFrequencyMap(const std::string& filename);

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

