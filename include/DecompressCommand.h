#ifndef DECOMPRESS_COMMAND_H
#define DECOMPRESS_COMMAND_H

#include "BaseCommand.h"
#include "HuffmanGenome.h"
#include "RLEGenome.h"
#include "CombinedCompressor.h"
#include "CompressionMetrics.h"
#include <memory>
#include <string>

class DecompressCommand : public BaseCommand {
public:
    DecompressCommand(CLI::App& app);

    void execute() override;

private:
    std::string inputFile;
    std::string outputFile;
    std::string algorithm;
    bool verbose;
};

#endif
