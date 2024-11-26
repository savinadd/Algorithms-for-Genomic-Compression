#ifndef COMPRESSOR_H
#define COMPRESSOR_H

#include <string>
#include "CompressionMetrics.h"

class Compressor {
public:
    virtual ~Compressor() = default;

    // Pure virtual methods
    virtual void encodeFromFile(const std::string& inputFilename, const std::string& outputFilename) = 0;
    virtual void decodeFromFile(const std::string& inputFilename, const std::string& outputFilename) = 0;

    // Get compression metrics
    virtual CompressionMetrics getMetrics() const = 0;

    // Validate the decoded file against the original file
    virtual bool validateDecodedFile(const std::string& originalFilename, const std::string& decodedFilename) = 0;
};

#endif // COMPRESSOR_H
