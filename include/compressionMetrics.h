#ifndef COMPRESSIONMETRICS_H
#define COMPRESSIONMETRICS_H

#include <string>
#include <unordered_map>
#include <array>

class CompressionMetrics {
public:
    CompressionMetrics();

    void calculateOriginalSize(const std::unordered_map<unsigned char, int>& frequencyMap);
    void calculateOriginalSize(const std::array<unsigned int, 4>& frequencyMap);
    void calculateOriginalSize(long long bits);

    void calculateCompressedSize(long long bits);
    void calculateCompressedSizeFromFile(const std::string& filename, int paddingBits = 0);
    void addOriginalSize(long long bits);
    void addCompressedSize(long long bits);
    long long getOriginalSize() const;
    long long getCompressedSize() const;
    double getCompressionRatio() const;
    void printMetrics() const;
    void setEntropyReduction(double originalEntropy, double compressedEntropy) {
        if (originalEntropy > 0) {
            entropyReduction = ((originalEntropy - compressedEntropy) / originalEntropy) * 100.0;
        } else {
            entropyReduction = 0.0;
        }
    }
    double getCompressionEfficiency() const {
        return (originalSize > 0) ? static_cast<double>(compressedSize) / originalSize : 0.0;
    }

private:
    long long originalSize;    // in bits
    long long compressedSize;  // in bits
    double entropyReduction = 0.0;
    
    long getFileSizeInBytes(const std::string& filename) const;
};

#endif
