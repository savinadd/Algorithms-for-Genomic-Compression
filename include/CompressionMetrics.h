// CompressionMetrics.h

#ifndef COMPRESSIONMETRICS_H
#define COMPRESSIONMETRICS_H

#include <string>

class CompressionMetrics {
public:
    CompressionMetrics();

    void calculateOriginalSize(const int frequencyMap[256]);
    void calculateCompressedSize(const std::string& encodedSequence);
    void calculateCompressedSizeFromFile(const std::string& filename, int paddingBits = 0);
    void addOriginalSize(long long bits);
    void addCompressedSize(long long bits);
    double getCompressionRatio() const;
    void printMetrics() const;

private:
    long long originalSize;    // in bits
    long long compressedSize;  // in bits
};

#endif // COMPRESSIONMETRICS_H
