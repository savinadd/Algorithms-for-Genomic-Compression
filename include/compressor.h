#ifndef COMPRESSOR_H
#define COMPRESSOR_H

#include <string>
#include "CompressionMetrics.h"

class Compressor {
public:
    virtual ~Compressor() = default;

    virtual void encodeFromFile(const std::string& inputFilename, const std::string& outputFilename) = 0;
    virtual void decodeFromFile(const std::string& inputFilename, const std::string& outputFilename) = 0;
    virtual CompressionMetrics getMetrics() const = 0;
    virtual bool validateDecodedFile(const std::string& originalFilename, const std::string& decodedFilename) = 0;
    virtual bool validateInputFile(const std::string& inputFilename) const = 0;
};

#endif 
