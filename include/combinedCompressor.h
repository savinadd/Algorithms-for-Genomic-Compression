#ifndef COMBINEDCOMPRESSOR_H
#define COMBINEDCOMPRESSOR_H

#include <string>
#include "Compressor.h"
#include "RLEGenome.h"
#include "HuffmanCompressor.h" 

class CombinedCompressor : public Compressor {
public:
    CombinedCompressor();

    void encodeFromFile(const std::string& inputFilename, const std::string& outputFilename) override;
    void decodeFromFile(const std::string& inputFilename, const std::string& outputFilename) override;
    CompressionMetrics getMetrics() const override;
    bool validateDecodedFile(const std::string& originalFilename, const std::string& decodedFilename) override;

private:
    RLEGenome rleCompressor;
    HuffmanCompressor huffmanCompressor; 
    CompressionMetrics metrics;
};

#endif
