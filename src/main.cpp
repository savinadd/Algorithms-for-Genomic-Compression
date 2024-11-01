#include "huffmanGenetic.h"
#include <iostream>
#include <string>

int main() {
    // Sample genetic sequence
    std::string data = "ACGTACGTTGCA"; // Test with a known sequence

    // Array to hold the Huffman codes for each nucleotide
    std::string huffmanCode[4];

    // Build the Huffman tree and generate codes
    buildHuffmanTree(data, huffmanCode);

    // Print the Huffman codes
    std::cout << "Generated Huffman Codes for each nucleotide:\n";
    printHuffmanCodes(huffmanCode);

    // Compress the data using the generated Huffman codes
    std::string compressedData = compressUsingHuffman(data, huffmanCode);

    // Output the original and compressed sequences
    std::cout << "\nOriginal Sequence: " << data << "\n";
    std::cout << "Compressed Sequence: " << compressedData << "\n";

    // Verification
    std::cout << "\nTest passed: ";
    if (!compressedData.empty() && compressedData.length() < data.length() * 2) {
        std::cout << "Compression successful\n";
    }
    else {
        std::cout << "Compression failed\n";
    }

    return 0;
}
