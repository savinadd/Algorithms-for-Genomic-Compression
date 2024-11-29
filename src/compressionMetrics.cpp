#include "CompressionMetrics.h"
#include <iostream>
#include <fstream>
#include <stdexcept>
#include <cmath>

CompressionMetrics::CompressionMetrics() : originalSize(0), compressedSize(0) {}

void CompressionMetrics::calculateOriginalSize(const std::unordered_map<unsigned char, int> &frequencyMap)
{
    originalSize = 0;
    for (const auto &entry : frequencyMap)
    {
        if (entry.second < 0)
        {
            throw std::invalid_argument("Frequency cannot be negative.");
        }
        originalSize += entry.second * 8; // Each character is 8 bits (ASCII encoding)
    }
}

void CompressionMetrics::calculateOriginalSize(const std::array<unsigned int, 4> &frequencyMap)
{
    originalSize = 0;
    for (size_t i = 0; i < frequencyMap.size(); ++i)
    {
        originalSize += frequencyMap[i] * 8; // Each character is 8 bits (ASCII encoding)
    }
}

long CompressionMetrics::getFileSizeInBytes(const std::string &filename) const
{
    std::ifstream file(filename, std::ios::binary | std::ios::ate);
    if (!file.is_open())
    {
        throw std::runtime_error("Error: Unable to open file '" + filename + "' to get its size.");
    }
    return static_cast<long>(file.tellg());
}

void CompressionMetrics::calculateCompressedSizeFromFile(const std::string &compressedFilename, int paddingBits)
{
    long compressedFileSizeBytes = getFileSizeInBytes(compressedFilename);

    if (compressedFileSizeBytes == 0)
    {
        throw std::runtime_error("Error: Compressed file '" + compressedFilename + "' is empty.");
    }

    long encodedDataBits = (compressedFileSizeBytes - 1) * 8 - paddingBits;

    if (encodedDataBits < 0)
    {
        throw std::runtime_error("Error: Calculated encoded bits are negative. Check padding bits.");
    }

    std::string freqFilename = compressedFilename + ".freq";
    long freqFileSizeBytes = getFileSizeInBytes(freqFilename);
    long freqFileSizeBits = freqFileSizeBytes * 8;

    // Total compressed size is encoded data bits plus frequency map bits
    this->compressedSize = encodedDataBits + freqFileSizeBits;

    getCompressionRatio();
}

void CompressionMetrics::calculateOriginalSize(long long bits)
{
    if (bits < 0)
    {
        throw std::invalid_argument("Size in bits cannot be negative.");
    }
    originalSize = bits;
}

void CompressionMetrics::addOriginalSize(long long bits)
{
    originalSize += bits;
}

void CompressionMetrics::addCompressedSize(long long bits)
{
    compressedSize += bits;
}

long long CompressionMetrics::getOriginalSize() const
{
    return originalSize;
}

long long CompressionMetrics::getCompressedSize() const
{
    return compressedSize;
}

double CompressionMetrics::getCompressionRatio() const
{
    if (compressedSize == 0)
    {
        return 0.0;
    }
    return static_cast<double>(originalSize) / compressedSize;
}

double calculateEntropy(const std::unordered_map<char, int> &frequencies, int totalSymbols)
{
    double entropy = 0.0;
    for (const auto &entry : frequencies)
    {
        double probability = static_cast<double>(entry.second) / totalSymbols;
        entropy -= probability * std::log2(probability);
    }
    return entropy;
}

void CompressionMetrics::printMetrics() const
{
    std::cout << "Original Size (bits): " << originalSize << "\n";
    std::cout << "Compressed Size (bits): " << compressedSize << "\n";
    std::cout << "Compression Ratio: " << getCompressionRatio() << "\n";
}
