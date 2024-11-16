#ifndef RLEGENOME_H
#define RLEGENOME_H

#include <iostream>
#include <string>

class RLEGenome {
public:
    // Encode the genomic sequence using Run-Length Encoding
    std::string encode(const std::string& sequence);

    // Decode the encoded sequence back to its original form
    std::string decode(const std::string& encodedSequence);

    // Calculate the compression ratio
    double getCompressionRatio(const std::string& originalSequence, const std::string& encodedSequence);
};

#endif // RLEGENOME_H
