// include/CompressCommand.h
#ifndef COMPRESS_COMMAND_H
#define COMPRESS_COMMAND_H

#include "BaseCommand.h"
#include "HuffmanGenome.h"
#include "RLEGenome.h"
#include "CombinedCompressor.h"
#include "CompressionMetrics.h"
#include <memory>
#include <string>

class CompressCommand : public BaseCommand {
public:
    CompressCommand(CLI::App& app);

    void execute() override;

private:
    std::string inputFile;
    std::string outputFile;
    std::string algorithm;
    bool verbose;
};

#endif
