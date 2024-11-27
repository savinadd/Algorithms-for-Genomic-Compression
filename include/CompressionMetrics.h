#ifndef COMPRESSIONMETRICS_H
#define COMPRESSIONMETRICS_H

#include <string>
#include <unordered_map>

class CompressionMetrics {
public:
    CompressionMetrics();

    void calculateOriginalSize(const std::unordered_map<unsigned char, int>& frequencyMap);
    void calculateOriginalSize(const int frequencyMap[256]);
    void calculateOriginalSize(long long bits);

    void calculateCompressedSize(long long bits);
    void calculateCompressedSizeFromFile(const std::string& filename, int paddingBits = 0);
    void addOriginalSize(long long bits);
    void addCompressedSize(long long bits);
    long long getOriginalSize() const;
    long long getCompressedSize() const;
    double getCompressionRatio() const;
    void printMetrics() const;

private:
    long long originalSize;    // in bits
    long long compressedSize;  // in bits
};

#endif
