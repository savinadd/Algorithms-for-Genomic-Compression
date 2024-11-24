// CompressionMetrics.cpp

#include "CompressionMetrics.h"
#include <iostream>
#include <fstream>
#include <stdexcept>

CompressionMetrics::CompressionMetrics() : originalSize(0), compressedSize(0) {}

void CompressionMetrics::calculateOriginalSize(const int frequencyMap[256]) {
    originalSize = 0;
    for (char ch : {'A', 'C', 'G', 'T'}) {
        originalSize += frequencyMap[(unsigned char)ch] * 2; // 2 bits per nucleotide
    }
}

void CompressionMetrics::calculateCompressedSize(const std::string& encodedSequence) {
    compressedSize = encodedSequence.length(); // Size in bits
}

void CompressionMetrics::calculateCompressedSizeFromFile(const std::string& filename, int paddingBits) {
    std::ifstream infile(filename, std::ios::binary | std::ios::ate);
    if (infile) {
        std::streamsize size = infile.tellg();
        compressedSize = ((size - 1) * 8) - paddingBits; // Exclude padding bits byte and padding bits
        infile.close();
    } else {
        throw std::runtime_error("Error: Unable to open file '" + filename + "' to calculate compressed size.");
    }
}

void CompressionMetrics::addOriginalSize(long long bits) {
    originalSize += bits;
}

void CompressionMetrics::addCompressedSize(long long bits) {
    compressedSize += bits;
}

double CompressionMetrics::getCompressionRatio() const {
    if (compressedSize == 0) {
        return 0.0;
    }
    return static_cast<double>(originalSize) / compressedSize;
}

void CompressionMetrics::printMetrics() const {
    std::cout << "Original Size (bits): " << originalSize << "\n";
    std::cout << "Compressed Size (bits): " << compressedSize << "\n";
    std::cout << "Compression Ratio: " << getCompressionRatio() << "\n";
}
