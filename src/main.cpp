// main.cpp

#include <iostream>
#include "RLEGenome.h"
#include "HuffmanGenome.h"
#include "combinedCompressor.h"
#include "Logger.h"

int main(int argc, char* argv[]) {
    try {
        // File paths
        std::string inputFile = "genome_data.txt";

        // **RLE Compression and Decompression**
        Logger::getInstance().log("\nStarting RLE Compression...");

        std::string rleEncodedFile = "genome_data_rle_encoded.bin";
        std::string rleDecodedFile = "genome_data_rle_decoded.txt";

        RLEGenome rleCompressor;
        rleCompressor.encodeFromFile(inputFile, rleEncodedFile);
        rleCompressor.decodeFromFile(rleEncodedFile, rleDecodedFile);

        // Validate decoded data
        if (rleCompressor.validateDecodedFile(inputFile, rleDecodedFile)) {
            Logger::getInstance().log("RLE decoded data is valid.");
        } else {
            Logger::getInstance().log("RLE decoded data is invalid.");
        }

        // Get and print RLE compression metrics
        CompressionMetrics rleMetrics = rleCompressor.getMetrics();
        rleMetrics.printMetrics();

        Logger::getInstance().log("RLE Compression completed successfully.");

        // **Huffman Compression and Decompression**
        Logger::getInstance().log("\nStarting Huffman Compression...");

        std::string huffmanEncodedFile = "genome_data_huffman_encoded.bin";
        std::string huffmanDecodedFile = "genome_data_huffman_decoded.txt";

        HuffmanGenome huffmanCompressor;
        huffmanCompressor.encodeFromFile(inputFile, huffmanEncodedFile);
        huffmanCompressor.decodeFromFile(huffmanEncodedFile, huffmanDecodedFile);

        // Validate decoded data
        if (huffmanCompressor.validateDecodedFile(inputFile, huffmanDecodedFile)) {
            Logger::getInstance().log("Huffman decoded data is valid.");
        } else {
            Logger::getInstance().log("Huffman decoded data is invalid.");
        }

        // Get and print Huffman compression metrics
        CompressionMetrics huffmanMetrics = huffmanCompressor.getMetrics();
        huffmanMetrics.printMetrics();

        Logger::getInstance().log("Huffman Compression completed successfully.");

        // **Combined Compression and Decompression**
        Logger::getInstance().log("\nStarting Combined Compression...");

        std::string combinedEncodedFile = "genome_data_combined_encoded.bin";
        std::string combinedDecodedFile = "genome_data_combined_decoded.txt";

        CombinedCompressor combinedCompressor;
        combinedCompressor.encodeFromFile(inputFile, combinedEncodedFile);
        combinedCompressor.decodeFromFile(combinedEncodedFile, combinedDecodedFile);

        // Validate decoded data
        if (combinedCompressor.validateDecodedFile(inputFile, combinedDecodedFile)) {
            Logger::getInstance().log("Combined decoded data is valid.");
        } else {
            Logger::getInstance().log("Combined decoded data is invalid.");
        }

        // Get and print Combined compression metrics
        CompressionMetrics combinedMetrics = combinedCompressor.getMetrics();
        combinedMetrics.printMetrics();

        Logger::getInstance().log("Combined Compression completed successfully.");

    } catch (const std::exception& e) {
        Logger::getInstance().log(std::string("Exception: ") + e.what());
        return 1;
    }

    return 0;
}
