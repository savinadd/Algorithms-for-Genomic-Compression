#include "CompressionMetrics.h"
#include <iostream>
#include <fstream>
#include <stdexcept>

CompressionMetrics::CompressionMetrics() : originalSize(0), compressedSize(0) {}

void CompressionMetrics::calculateOriginalSize(const std::unordered_map<unsigned char, int>& frequencyMap) {
    originalSize = 0;
    for (const auto& entry : frequencyMap) {
        if (entry.second < 0) {
            throw std::invalid_argument("Frequency cannot be negative.");
        }
        originalSize += entry.second * 8; // Each character is 8 bits (ASCII encoding)
    }
}

void CompressionMetrics::calculateOriginalSize(const int frequencyMap[256]) {
    originalSize = 0;
    for (int i = 0; i < 256; ++i) {
        if (frequencyMap[i] < 0) {
            throw std::invalid_argument("Frequency cannot be negative.");
        }
        originalSize += frequencyMap[i] * 8; // Each character is 8 bits (ASCII encoding)
    }
}

void CompressionMetrics::calculateOriginalSize(long long bits) {
    if (bits < 0) {
        throw std::invalid_argument("Size in bits cannot be negative.");
    }
    originalSize = bits;
}

void CompressionMetrics::calculateCompressedSizeFromFile(const std::string& filename, int paddingBits) {
    std::ifstream infile(filename, std::ios::binary | std::ios::ate);

    if (!infile) {
        throw std::runtime_error("Error: Unable to open file '" + filename + "' to calculate compressed size.");
    }

    std::streamsize fileSize = infile.tellg();
    infile.close();

    if (fileSize <= 0) {
        throw std::runtime_error("Error: File is empty or unreadable.");
    }

    compressedSize = (fileSize * 8) - paddingBits;

    if (compressedSize < 0) {
        throw std::runtime_error("Error: Calculated compressed size is negative. Check paddingBits value.");
    }
}

void CompressionMetrics::addOriginalSize(long long bits) {
    originalSize += bits;
}

void CompressionMetrics::addCompressedSize(long long bits) {
    compressedSize += bits;
}

long long CompressionMetrics::getOriginalSize() const {
    return originalSize;
}

long long CompressionMetrics::getCompressedSize() const {
    return compressedSize;
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