#include "CompressCommand.h"
#include "Logger.h"
#include <iostream>

CompressCommand::CompressCommand(CLI::App& app) {
    auto compress = app.add_subcommand("compress", "Compress a genomic data file");

    compress->add_option("-i,--input", inputFile, "Path to the input genomic data file")
        ->required()
        ->check(CLI::ExistingFile);

    compress->add_option("-o,--output", outputFile, "Path to the output compressed file")
        ->required();

    compress->add_option("-a,--algorithm", algorithm, "Compression algorithm (huffman, rle, both)")
        ->default_val("huffman")
        ->check(CLI::IsMember({"huffman", "rle", "both"}));

    compress->add_flag("-v,--verbose", verbose, "Enable verbose output");

    compress->callback([this]() { this->execute(); });
}

void CompressCommand::execute() {
    Logger::getInstance().log("Starting compression...");

    if (verbose) {
        Logger::getInstance().log("Input File: " + inputFile);
        Logger::getInstance().log("Output File: " + outputFile);
        Logger::getInstance().log("Algorithm: " + algorithm);
    }

    std::unique_ptr<BaseCommand> compressor;

    if (algorithm == "huffman") {
        HuffmanGenome huffman;
        huffman.encodeFromFile(inputFile, outputFile);
        CompressionMetrics metrics = huffman.getMetrics();
        Logger::getInstance().log("Compression completed.");
        Logger::getInstance().log("Original Size: " + std::to_string(metrics.getOriginalSize()) + " bits");
        Logger::getInstance().log("Compressed Size: " + std::to_string(metrics.getCompressedSize()) + " bits");
        Logger::getInstance().log("Compression Ratio: " + std::to_string(metrics.getCompressionRatio()));
    }
    else if (algorithm == "rle") {
        RLEGenome rle;
        rle.encodeFromFile(inputFile, outputFile);
        CompressionMetrics metrics = rle.getMetrics();
        Logger::getInstance().log("Compression completed.");
        Logger::getInstance().log("Original Size: " + std::to_string(metrics.getOriginalSize()) + " bits");
        Logger::getInstance().log("Compressed Size: " + std::to_string(metrics.getCompressedSize()) + " bits");
        Logger::getInstance().log("Compression Ratio: " + std::to_string(metrics.getCompressionRatio()));
    }
    else if (algorithm == "both") {
        CombinedCompressor combined;
        combined.encodeFromFile(inputFile, outputFile);
        CompressionMetrics metrics = combined.getMetrics();
        Logger::getInstance().log("Compression completed.");
        Logger::getInstance().log("Original Size: " + std::to_string(metrics.getOriginalSize()) + " bits");
        Logger::getInstance().log("Compressed Size: " + std::to_string(metrics.getCompressedSize()) + " bits");
        Logger::getInstance().log("Compression Ratio: " + std::to_string(metrics.getCompressionRatio()));
    }
    else {
        Logger::getInstance().log("Unsupported compression algorithm: " + algorithm);
    }
}
