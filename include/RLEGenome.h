#ifndef RLEGENOME_H
#define RLEGENOME_H

#include <string>
#include "CompressionMetrics.h"
#include "Compressor.h"

class RLEGenome : public Compressor {
public:
    RLEGenome();
    virtual ~RLEGenome() = default;

    void encodeFromFile(const std::string& inputFilename, const std::string& outputFilename) override;
    void decodeFromFile(const std::string& inputFilename, const std::string& outputFilename) override;
    CompressionMetrics getMetrics() const override;
    bool validateDecodedFile(const std::string& originalFilename, const std::string& decodedFilename) override;
    bool validateInputFile(const std::string& inputFilename) const override;

private:
    CompressionMetrics metrics;
    std::string encode(const std::string& sequence);
    std::string decode(const std::string& encodedSequence);
};

#endif
