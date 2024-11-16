#include "CLI.h"
#include "CLI11.hpp"
#include "HuffmanGenome.h"
#include "RLEGenome.h"

#include <iostream>
#include <fstream>
#include <string>

GenomeCompressorCLI::GenomeCompressorCLI(int argc, char** argv)
    : argc_(argc), argv_(argv) {}

int GenomeCompressorCLI::run() {
    CLI::App app{"GenomeCompressor CLI Tool"};

    // Variables for command-line options
    std::string algorithm = "all";
    std::string inputFile;
    std::string outputFile;
    bool encode = false;
    bool decode = false;

    // Set up options
    app.add_flag("-e,--encode", encode, "Encode a file");
    app.add_flag("-d,--decode", decode, "Decode a file");
    app.add_option("-a,--algorithm", algorithm, "Algorithm to use (huffman, rle, all)")
        ->check(CLI::IsMember({"huffman", "rle", "all"}));
    app.add_option("-i,--input", inputFile, "Input file path")->required();
    app.add_option("-o,--output", outputFile, "Output file path")->required();

    // Parse command-line arguments
    try {
        app.parse(argc_, argv_);
    } catch (const CLI::ParseError &e) {
        return app.exit(e);
    }

    // Display home page if no options are provided
    if (argc_ == 1) {
        std::cout << "----------------------------------------\n";
        std::cout << "Welcome to GenomeCompressor CLI Tool\n";
        std::cout << "----------------------------------------\n";
        std::cout << "This tool compresses and decompresses genomic data using Huffman and Run-Length Encoding algorithms.\n";
        std::cout << "You can encode or decode files in .txt or .fasta format.\n";
        std::cout << "Use -h or --help to see available options.\n";
        return 0;
    }

    // Validate that either encode or decode is selected
    if (!encode && !decode) {
        std::cerr << "Error: Please specify either --encode or --decode.\n";
        return 1;
    }

    // Read input file
    std::ifstream infile(inputFile);
    if (!infile) {
        std::cerr << "Error: Unable to open input file '" << inputFile << "'.\n";
        return 1;
    }

    // Read the sequence from the file
    std::string sequence;
    if (inputFile.substr(inputFile.find_last_of(".") + 1) == "fasta") {
        // Handle FASTA files
        std::string line;
        while (std::getline(infile, line)) {
            if (line.empty() || line[0] == '>') {
                continue; // Skip headers and empty lines
            }
            sequence += line;
        }
    } else {
        // Assume plain text file
        std::getline(infile, sequence, '\0'); // Read entire file
    }
    infile.close();

    // Perform encoding or decoding based on user input
    if (encode) {
        // Encoding logic
        // You can modularize this further by creating private methods like `encodeSequence`
        if (algorithm == "huffman" || algorithm == "all") {
            HuffmanGenome huffman;
            huffman.encode(sequence);
            std::string encodedData = huffman.getEncodedSequence();

            // Save encoded data and frequency map
            std::ofstream outfile(outputFile + "_huffman.bin", std::ios::binary);
            if (!outfile) {
                std::cerr << "Error: Unable to open output file '" << outputFile << "_huffman.bin'.\n";
                return 1;
            }
            outfile << encodedData;
            outfile.close();

            // Save the frequency map
            huffman.saveFrequencyMap(outputFile + "_huffman_freq.txt");

            double compressionRatio = huffman.getCompressionRatio();
            std::cout << "Huffman Encoding Complete.\n";
            std::cout << "Compression Ratio: " << compressionRatio << "\n";
        }

        if (algorithm == "rle" || algorithm == "all") {
            RLEGenome rle;
            std::string encodedData = rle.encode(sequence);

            if (encodedData.empty()) {
                std::cerr << "RLE Encoding failed.\n";
                return 1;
            }

            std::ofstream outfile(outputFile + "_rle.txt");
            if (!outfile) {
                std::cerr << "Error: Unable to open output file '" << outputFile << "_rle.txt'.\n";
                return 1;
            }
            outfile << encodedData;
            outfile.close();

            double compressionRatio = rle.getCompressionRatio(sequence, encodedData);
            std::cout << "RLE Encoding Complete.\n";
            std::cout << "Compression Ratio: " << compressionRatio << "\n";
        }
    } else if (decode) {
        // Decoding logic
        if (algorithm == "huffman" || algorithm == "all") {
            HuffmanGenome huffman;

            // Load frequency map
            huffman.loadFrequencyMap(inputFile + "_freq.txt");

            // Decode the data
            huffman.encode(sequence); // Build the tree based on loaded frequency map
            std::string decodedData = huffman.decode(sequence);

            if (decodedData.empty()) {
                std::cerr << "Huffman Decoding failed.\n";
                return 1;
            }

            // Save decoded data
            std::ofstream outfile(outputFile + "_huffman_decoded.txt");
            if (!outfile) {
                std::cerr << "Error: Unable to open output file '" << outputFile << "_huffman_decoded.txt'.\n";
                return 1;
            }
            outfile << decodedData;
            outfile.close();

            std::cout << "Huffman Decoding Complete.\n";
        }

        if (algorithm == "rle" || algorithm == "all") {
            RLEGenome rle;
            std::string decodedData = rle.decode(sequence);

            if (decodedData.empty()) {
                std::cerr << "RLE Decoding failed.\n";
                return 1;
            }

            // Save decoded data
            std::ofstream outfile(outputFile + "_rle_decoded.txt");
            if (!outfile) {
                std::cerr << "Error: Unable to open output file '" << outputFile << "_rle_decoded.txt'.\n";
                return 1;
            }
            outfile << decodedData;
            outfile.close();

            std::cout << "RLE Decoding Complete.\n";
        }
    }

    return 0;
}
