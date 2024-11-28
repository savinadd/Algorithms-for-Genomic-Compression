#include "Application.h"
#include "CLI11.hpp"
#include "HuffmanGenome.h"
#include "HuffmanCompressor.h"
#include "RLEGenome.h"
#include "CombinedCompressor.h"
#include "Logger.h"
#include "rang.hpp"
#include <iostream>
#include <memory>
#include <string>

using namespace rang;

Application::Application(int argc, char** argv) 
    : argc_(argc), argv_(argv), compressor(nullptr), useMenu(false) {}

void Application::parseArguments() {
    CLI::App app{"Genomic Data Compressor"};

    app.get_formatter()->column_width(70);
    // Define CLI options
    app.add_flag("--menu", useMenu, "Display a welcome menu with usage instructions");
    app.add_option("-i,--input", inputFile, "Input file to compress")
        ->required(false)
        ->check(CLI::ExistingFile);
    app.add_option("-o,--output", outputFile, "Output file for the compressed data")
        ->required(false);
    app.add_option("-m,--method", method, "Compression method: huffman, rle, combined, default")
        ->required(false)
        ->check(CLI::IsMember({"huffman", "rle", "combined", "huffman_compressor"}));

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
    std::cout <<style::bold << fg::cyan << "This tool helps you compress genomic data using various algorithms.\n\n" << style::reset;
    std::cout <<style::bold << fg::cyan << "Usage Examples:\n" << style::reset;
    std::cout << "1. Compress a file using Huffman:\n";
    std::cout << "   compressor -i path/to/input/file -o outputfilename.fileformat -m huffman\n\n";
    std::cout << "2. Compress a file using Run-Length Encoding (RLE):\n";
    std::cout << "   compressor -i path/to/input/file -o outputfilename.fileformat -m rle\n\n";
    std::cout << "3. Compress a file using the combined RLE + Huffman method:\n";
    std::cout << "   compressor -i path/to/input/file -o outputfilename.fileformat -m combined\n\n";
    std::cout << "4. View this menu again:\n";
    std::cout << "   compressor --menu\n\n";
    std::cout << "5. View the help menu:\n";
    std::cout << "   compressor --help\n\n";
    std::cout << "Note:\n";
    std::cout << "- The input file (-i) must exist.\n";
    std::cout << "- The output file (-o) will be created if it doesn't exist.\n";
    std::cout << "- The method (-m) must be one of: huffman, rle, combined.\n";
    std::cout << "=============================================\n";
}

int Application::run() {
    // Step 1: Parse command-line arguments
    parseArguments();

    // If the menu flag is set, display the menu and exit
    if (useMenu) {
        displayMenu();
        return 0;
    }

    try {
        // Step 2: Validate required arguments
        if (inputFile.empty() || outputFile.empty() || method.empty()) {
            std::cerr << "Error: Missing required arguments.\n";
            std::cerr << "Run `compressor --menu` for usage instructions.\n";
            return 1;
        }

        // Step 3: Instantiate the appropriate compressor based on the method
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
            std::cerr << "Unknown method. Please choose huffman, rle, combined, or default.\n";
            return 1;
        }

        // Step 4: Perform encoding
        compressor->encodeFromFile(inputFile, outputFile);
        std::cout << method << " compression completed.\n";

        // Step 5: Display compression metrics
        compressor->validateDecodedFile(inputFile, outputFile);
        compressor->getMetrics().printMetrics();

    } catch (const std::exception& e) {
        // Log and display any exceptions that occur during processing
        Logger::getInstance().log(std::string("Error: ") + e.what());
        std::cerr << "Error: " << e.what() << "\n";
        return 1;
    }

    return 0;
}
