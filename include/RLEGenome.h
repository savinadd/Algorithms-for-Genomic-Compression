// RLEGenome.h

#ifndef RLEGENOME_H
#define RLEGENOME_H

#include <iostream>
#include <string>
#include "CompressionMetrics.h"
#include "Compressor.h"

class RLEGenome : public Compressor {
public:
    RLEGenome();

    // Encode the genomic sequence from a string
    std::string encode(const std::string& sequence);

    // Decode the encoded sequence back to its original form from a string
    std::string decode(const std::string& encodedSequence);

    // Encode the genomic sequence from a file
    void encodeFromFile(const std::string& inputFilename, const std::string& outputFilename) override;

    // Decode the encoded sequence from a file
    void decodeFromFile(const std::string& inputFilename, const std::string& outputFilename) override;

    // Get compression metrics
    CompressionMetrics getMetrics() const override;

    // Validate the decoded file against the original file
    bool validateDecodedFile(const std::string& originalFilename, const std::string& decodedFilename) override;

private:
    CompressionMetrics metrics;

    size_t getFileSize(const std::string& filename);
};

#endif // RLEGENOME_H
