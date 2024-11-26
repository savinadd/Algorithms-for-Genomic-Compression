#include "DecompressCommand.h"
#include "Logger.h"
#include <iostream>

DecompressCommand::DecompressCommand(CLI::App& app) {
    auto decompress = app.add_subcommand("decompress", "Decompress a genomic data file");

    // Input file option (required)
    decompress->add_option("-i,--input", inputFile, "Path to the input compressed file")
        ->required()
        ->check(CLI::ExistingFile);

    // Output file option (required)
    decompress->add_option("-o,--output", outputFile, "Path to the output decompressed file")
        ->required();

    // Algorithm selection option
    decompress->add_option("-a,--algorithm", algorithm, "Decompression algorithm (huffman, rle, both)")
        ->default_val("huffman")
        ->check(CLI::IsMember({"huffman", "rle", "both"}));

    // Verbose flag
    decompress->add_flag("-v,--verbose", verbose, "Enable verbose output");

    // Bind the execute function to the callback
    decompress->callback([this]() { this->execute(); });
}

void DecompressCommand::execute() {
    Logger::getInstance().log("Starting decompression...");

    if (verbose) {
        Logger::getInstance().log("Input File: " + inputFile);
        Logger::getInstance().log("Output File: " + outputFile);
        Logger::getInstance().log("Algorithm: " + algorithm);
    }

    if (algorithm == "huffman") {
        HuffmanGenome huffman;
        huffman.decodeFromFile(inputFile, outputFile);
     
        // bool isValid = huffman.validateDecodedFile(originalFilePath, outputFile);
        // if (isValid) { Logger::getInstance().log("Validation successful."); }
        // else { Logger::getInstance().log("Validation failed."); }
    }
    else if (algorithm == "rle") {
        RLEGenome rle;
        rle.decodeFromFile(inputFile, outputFile);
       
        // bool isValid = rle.validateDecodedFile(originalFilePath, outputFile);
        // if (isValid) { Logger::getInstance().log("Validation successful."); }
        // else { Logger::getInstance().log("Validation failed."); }
    }
    else if (algorithm == "both") {
        CombinedCompressor combined;
        combined.decodeFromFile(inputFile, outputFile);
    
        // bool isValid = combined.validateDecodedFile(originalFilePath, outputFile);
        // if (isValid) { Logger::getInstance().log("Validation successful."); }
        // else { Logger::getInstance().log("Validation failed."); }
    }
    else {
        Logger::getInstance().log("Unsupported decompression algorithm: " + algorithm);
    }

    Logger::getInstance().log("Decompression completed.");
}
