// RLEGenome.h

#ifndef RLEGENOME_H
#define RLEGENOME_H

#include <string>
#include "CompressionMetrics.h"
#include "Compressor.h"

class RLEGenome : public Compressor {
public:
    RLEGenome();

    // Encode the genomic sequence using Run-Length Encoding from a file
    void encodeFromFile(const std::string& inputFilename, const std::string& outputFilename) override;

    // Decode the encoded sequence back to its original form from a file
    void decodeFromFile(const std::string& inputFilename, const std::string& outputFilename) override;

    // Validate the decoded file against the original
    bool validateDecodedFile(const std::string& originalFilename, const std::string& decodedFilename) override;

    // Get compression metrics
    CompressionMetrics getMetrics() const override;

private:
    CompressionMetrics metrics;
    std::string encode(const std::string& sequence);
    std::string decode(const std::string& encodedSequence);
};

#endif // RLEGENOME_H
