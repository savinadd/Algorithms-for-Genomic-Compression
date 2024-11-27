// main.cpp
#include "CLI11.hpp"
#include "Compressor.h"
#include "HuffmanGenome.h"
#include "HuffmanCompressor.h"
#include "RLEGenome.h"
#include "CombinedCompressor.h"
#include "Logger.h"
#include <iostream>
#include <memory>

int main(int argc, char** argv) {
    CLI::App app{"Genomic Data Compressor"};

    std::string inputFile;
    std::string outputFile;
    std::string method;

    app.add_option("-i,--input", inputFile, "Input file to compress")
        ->required()
        ->check(CLI::ExistingFile);
    app.add_option("-o,--output", outputFile, "Output file for the compressed data")
        ->required();
    app.add_option("-m,--method", method, "Compression method: huffman, rle, combined, huffman_compressor")
        ->required()
        ->check(CLI::IsMember({"huffman", "rle", "combined", "huffman_compressor"}));

    CLI11_PARSE(app, argc, argv);

    try {
        // Use smart pointers for automatic memory management
        std::unique_ptr<Compressor> compressor;

        if (method == "huffman") {
            compressor = std::make_unique<HuffmanGenome>();
            Logger::getInstance().log("Using HuffmanGenome Compressor.");
        } else if (method == "huffman_compressor") {
            compressor = std::make_unique<HuffmanCompressor>();
            Logger::getInstance().log("Using HuffmanCompressor.");
        } else if (method == "rle") {
            compressor = std::make_unique<RLEGenome>();
            Logger::getInstance().log("Using RLEGenome Compressor.");
        } else if (method == "combined") {
            compressor = std::make_unique<CombinedCompressor>();
            Logger::getInstance().log("Using CombinedCompressor (RLE + Huffman).");
        } else {
            std::cerr << "Unknown method. Please choose huffman, rle, combined, or huffman_compressor.\n";
            return 1;
        }

        // Perform encoding
        compressor->encodeFromFile(inputFile, outputFile);
        std::cout << method << " compression completed.\n";
        compressor->getMetrics().printMetrics();

        // Optionally, perform decoding and validation if needed
        /*
        std::string decodedFile = "decoded_output.txt";
        compressor->decodeFromFile(outputFile, decodedFile);
        if (compressor->validateDecodedFile(inputFile, decodedFile)) {
            std::cout << "Validation successful. Decoded file matches the original.\n";
        } else {
            std::cerr << "Validation failed. Decoded file does not match the original.\n";
        }
        */

    } catch (const std::exception& e) {
        Logger::getInstance().log(std::string("Error: ") + e.what());
        std::cerr << "Error: " << e.what() << "\n";
        return 1;
    }

    return 0;
}

// // int main(int argc, char** argv) {
// //     // Initialize Logger
// //     Logger::getInstance().log("Application started.");

// //     // Initialize and run CLI Manager
// //     CLIManager cliManager(argc, argv);
// //     int exit_code = cliManager.run();

// //     Logger::getInstance().log("Application exited.");
// //     return exit_code;
// // }


// #include <iostream>
// #include "RLEGenome.h"
// #include "HuffmanGenome.h"
// #include "combinedCompressor.h"
// #include "Logger.h"

// int main(int argc, char* argv[]) {
//     try {
//         // File paths
//         std::string inputFile = "genome_data.txt";

//         // **RLE Compression and Decompression**
//         Logger::getInstance().log("\nStarting RLE Compression...");

//         std::string rleEncodedFile = "genome_data_rle_encoded.bin";
//         std::string rleDecodedFile = "genome_data_rle_decoded.txt";

//         RLEGenome rleCompressor;
//         rleCompressor.encodeFromFile(inputFile, rleEncodedFile);
//         rleCompressor.decodeFromFile(rleEncodedFile, rleDecodedFile);

//         // Validate decoded data
//         if (rleCompressor.validateDecodedFile(inputFile, rleDecodedFile)) {
//             Logger::getInstance().log("RLE decoded data is valid.");
//         } else {
//             Logger::getInstance().log("RLE decoded data is invalid.");
//         }

//         // Get and print RLE compression metrics
//         CompressionMetrics rleMetrics = rleCompressor.getMetrics();
//         rleMetrics.printMetrics();

//         Logger::getInstance().log("RLE Compression completed successfully.");

//         // **Huffman Compression and Decompression**
//         Logger::getInstance().log("\nStarting Huffman Compression...");

//         std::string huffmanEncodedFile = "genome_data_huffman_encoded.bin";
//         std::string huffmanDecodedFile = "genome_data_huffman_decoded.txt";

//         HuffmanGenome huffmanCompressor;
//         huffmanCompressor.encodeFromFile(inputFile, huffmanEncodedFile);
//         huffmanCompressor.decodeFromFile(huffmanEncodedFile, huffmanDecodedFile);

//         // Validate decoded data
//         if (huffmanCompressor.validateDecodedFile(inputFile, huffmanDecodedFile)) {
//             Logger::getInstance().log("Huffman decoded data is valid.");
//         } else {
//             Logger::getInstance().log("Huffman decoded data is invalid.");
//         }

//         // Get and print Huffman compression metrics
//         CompressionMetrics huffmanMetrics = huffmanCompressor.getMetrics();
//         huffmanMetrics.printMetrics();

//         Logger::getInstance().log("Huffman Compression completed successfully.");

//         // **Combined Compression and Decompression**
//         Logger::getInstance().log("\nStarting Combined Compression...");

//         std::string combinedEncodedFile = "genome_data_combined_encoded.bin";
//         std::string combinedDecodedFile = "genome_data_combined_decoded.txt";

//         CombinedCompressor combinedCompressor;
//         combinedCompressor.encodeFromFile(inputFile, combinedEncodedFile);
//         combinedCompressor.decodeFromFile(combinedEncodedFile, combinedDecodedFile);

//         // Validate decoded data
//         if (combinedCompressor.validateDecodedFile(inputFile, combinedDecodedFile)) {
//             Logger::getInstance().log("Combined decoded data is valid.");
//         } else {
//             Logger::getInstance().log("Combined decoded data is invalid.");
//         }

//         // Get and print Combined compression metrics
//         CompressionMetrics combinedMetrics = combinedCompressor.getMetrics();
//         combinedMetrics.printMetrics();

//         Logger::getInstance().log("Combined Compression completed successfully.");

//     } catch (const std::exception& e) {
//         Logger::getInstance().log(std::string("Exception: ") + e.what());
//         return 1;
//     }

//     return 0;
// }
