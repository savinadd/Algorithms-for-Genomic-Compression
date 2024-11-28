#include "Application.h"
#include "CLI11.hpp"
#include "HuffmanGenome.h"
#include "HuffmanCompressor.h"
#include "RLEGenome.h"
#include "CombinedCompressor.h"
#include "CompressionException.h" 
#include "Logger.h"
#include "rang.hpp"
#include <iostream>
#include <memory>
#include <string>
#include <FileValidator.h>
#include <cstdio>
#include <filesystem> 

using namespace rang;
namespace fs = std::filesystem; 

Application::Application(int argc, char** argv) 
    : argc_(argc), argv_(argv), compressor(nullptr), useMenu(false), compressMode(false), decompressMode(false) {}

void Application::parseArguments() {
    CLI::App app{"Genomic Data Compressor: Compresses or decompresses data using Huffman coding, Run Length Encoding, and other variations"};

    app.get_formatter()->column_width(70);
    
    // Define CLI flags for compression and decompression
    app.add_flag("-c,--compress", compressMode, "Compression mode: Compress the input file.");
    app.add_flag("-d,--decompress", decompressMode, "Decompression mode: Decompress the input file.");
    
    // Define CLI options
    app.add_option("-i,--input", inputFile, "Input file for compression or decompression")
        ->required()
        ->check(CLI::ExistingFile); 
    app.add_option("-o,--output", outputFile, "Output file for the compressed or decompressed data")
        ->required();
    app.add_option("-m,--method", method, "Compression method: huffmangenome, rle, combined, huffman")
        ->required()
        ->check(CLI::IsMember({"huffmangenome", "rle", "combined", "huffman"}));
    app.add_flag("--menu", useMenu, "Display a welcome menu with usage instructions");
    
    app.footer("Examples:\n"
           "  Compress using Huffman Genome Compressor:\n"
           "    compressor -c -i genome_data.txt -o genomeDataTest.bin -m huffmangenome\n\n"
           "  Decompress using Huffman Genome Compressor:\n"
           "    compressor -d -i genomeDataTest.bin -o decoded_genomeDataTest.txt -m huffmangenome\n\n"
           "  Compress using Run-Length Encoding (RLE):\n"
           "    compressor -c -i genome_data.txt -o genomeDataTest.rle -m rle\n\n"
           "  Compress using Combined RLE + Huffman:\n"
           "    compressor -c -i genome_data.txt -o genomeDataTest.combined -m combined\n\n"
           "  Display the menu:\n"
           "    compressor --menu\n\n"
           "  View the help menu:\n"
           "    compressor --help\n");
    
    // Parse the CLI arguments
    try {
        app.parse(argc_, argv_);
    } catch (const CLI::ParseError &e) {
        exit(app.exit(e));
    }
}

void Application::displayMenu() const {
    std::cout << "*************************************************************************************\n";
    std::cout << style::bold << fg::cyan << "                    Welcome to the Genomic Data Compressor\n" << style::reset;
    std::cout << "*************************************************************************************\n";
    std::cout << style::bold << fg::cyan << "This tool helps you compress and decompress genomic data using various algorithms.\n\n" << style::reset;
    std::cout << style::bold << fg::cyan << "Usage Examples:\n" << style::reset;
    std::cout << "1. Compress a file using the Huffman Genome Compressor:\n";
    std::cout << "   compressor -c -i path/to/input/file.txt -o outputfilename.huffg -m huffmangenome\n\n";
    std::cout << "2. Decompress a file using the Huffman Genome Compressor:\n";
    std::cout << "   compressor -d -i genomeDataTest.bin -o decoded_genomeDataTest.txt -m huffmangenome\n\n";
    std::cout << "3. Compress a file using Run-Length Encoding (RLE):\n";
    std::cout << "   compressor -c -i path/to/input/file.txt -o outputfilename.rle -m rle\n\n";
    std::cout << "4. Compress a file using the combined RLE + Huffman method:\n";
    std::cout << "   compressor -c -i path/to/input/file.txt -o outputfilename.combined -m combined\n\n";
    std::cout << "5. View this menu again:\n";
    std::cout << "   compressor --menu\n\n";
    std::cout << "6. View the help menu:\n";
    std::cout << "   compressor --help\n\n";
    std::cout << "Note:\n";
    std::cout << "- The input file (-i) must exist and have a .txt extension for compression.\n";
    std::cout << "- The output file (-o) will be created if it doesn't exist.\n";
    std::cout << "- The method (-m) must be one of: huffmangenome, rle, combined, huffman.\n";
    std::cout << "- For decompression, ensure that the frequency map file (inputFile.freq) exists.\n";
    std::cout << "=============================================\n";
}

int Application::run() {
    parseArguments();

    if (useMenu) {
        displayMenu();
        return 0;
    }

    try {
        if (!compressMode && !decompressMode) {
            std::cerr << "Error: Must specify either compression (-c) or decompression (-d) mode.\n";
            std::cerr << "Run `compressor --menu` for usage instructions.\n";
            return 1;
        }

        if (compressMode && decompressMode) {
            std::cerr << "Error: Cannot specify both compression (-c) and decompression (-d) modes simultaneously.\n";
            std::cerr << "Run `compressor --menu` for usage instructions.\n";
            return 1;
        }

        if (compressMode) {
            // Compression mode
            // Validate input file
            if (!FileValidator::fileExists(inputFile)) {
                std::cerr << "Error: Input file does not exist.\n";
                return 1;
            }

            if (!FileValidator::hasTxtExtension(inputFile)) {
                std::cerr << "Error: Input file must have a .txt extension for compression.\n";
                return 1;
            }

            if (!FileValidator::hasValidGenomeData(inputFile)) {
                std::cerr << "Error: Input file contains invalid characters. Only A, C, G, T are allowed.\n";
                return 1;
            }

            // Initialize the appropriate compressor
            if (method == "huffmangenome") {
                compressor = std::make_unique<HuffmanGenome>();
                Logger::getInstance().log("Using Huffman Genome Compressor.");
            } else if (method == "huffman") {
                compressor = std::make_unique<HuffmanCompressor>();
                Logger::getInstance().log("Using Huffman Compressor.");
            } else if (method == "rle") {
                compressor = std::make_unique<RLEGenome>();
                Logger::getInstance().log("Using Run Length Encoding Compressor.");
            } else if (method == "combined") {
                compressor = std::make_unique<CombinedCompressor>();
                Logger::getInstance().log("Using Combined Compressor (RLE + Huffman).");
            } else {
                std::cerr << "Unknown method. Please choose huffmangenome, rle, combined, or huffman.\n";
                return 1;
            }

   
            //std::cout << "Starting compression...\n";
            compressor->encodeFromFile(inputFile, outputFile);
            //std::cout << "Compression completed. Output file: " << outputFile << "\n";

      
            fs::path outputPath(outputFile);
            fs::path parentPath = outputPath.parent_path();
            fs::path filename = outputPath.filename();
            fs::path tempDecodedFilePath = parentPath / ("temp_decoded_" + filename.string());


            std::string tempDecodedFile = tempDecodedFilePath.string();
            //std::cout << "Starting decompression for validation...\n";
            compressor->decodeFromFile(outputFile, tempDecodedFile);
            //std::cout << "Decompression completed. Decoded file: " << tempDecodedFile << "\n";
            bool isValid = compressor->validateDecodedFile(inputFile, tempDecodedFile);
            if (isValid) {
                //std::cout << "Validation successful: Decoded file matches the original.\n";
                compressor->getMetrics().printMetrics();
            } else {
                std::cerr << "Validation failed: Decoded file does not match the original.\n";
            }

             try {
    fs::path outputAbsPath = fs::absolute(outputPath);
    std::cout << "Encoded file saved at: " << outputAbsPath.string() << "\n";
} catch (const fs::filesystem_error& e) {
    std::cerr << "Error retrieving file path: " << e.what() << "\n";
    Logger::getInstance().log(std::string("Filesystem error: ") + e.what());
}

            // std::remove(tempDecodedFile.c_str());

        }
        else if (decompressMode) {
            // Decompression mode
            // Validate input file
            if (!FileValidator::fileExists(inputFile)) {
                std::cerr << "Error: Input file does not exist.\n";
                return 1;
            }

            // Initialize the appropriate compressor
            if (method == "huffmangenome") {
                compressor = std::make_unique<HuffmanGenome>();
                Logger::getInstance().log("Using Huffman Genome Compressor.");
            } else if (method == "huffman") {
                compressor = std::make_unique<HuffmanCompressor>();
                Logger::getInstance().log("Using Huffman Compressor.");
            } else if (method == "rle") {
                compressor = std::make_unique<RLEGenome>();
                Logger::getInstance().log("Using Run Length Encoding Compressor.");
            } else if (method == "combined") {
                compressor = std::make_unique<CombinedCompressor>();
                Logger::getInstance().log("Using Combined Compressor (RLE + Huffman).");
            } else {
                std::cerr << "Unknown method. Please choose huffmangenome, rle, combined, or huffman.\n";
                return 1;
            }

            // Perform decoding
           // std::cout << "Starting decompression...\n";
            compressor->decodeFromFile(inputFile, outputFile);
            //std::cout << "Decompression completed. Output file: " << outputFile << "\n";

            try {
    fs::path outputPath(outputFile);
    fs::path outputAbsPath = fs::absolute(outputPath);
    std::cout << "Decoded file saved at: " << outputAbsPath.string() << "\n";
} catch (const fs::filesystem_error& e) {
    std::cerr << "Error retrieving file path: " << e.what() << "\n";
    Logger::getInstance().log(std::string("Filesystem error: ") + e.what());
}

            // validate the decompressed file against the original (if original is known)

        }

    } catch (const CompressionException& ce) {
        Logger::getInstance().log(std::string("CompressionException: ") + ce.what());
        std::cerr << ce.what() << "\n";
        return 1;
    } catch (const std::exception& e) {
        Logger::getInstance().log(std::string("Exception: ") + e.what());
        std::cerr << "Error: " << e.what() << "\n";
        return 1;
    }

    return 0;
}
