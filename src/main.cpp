// main.cpp

#include <iostream>
#include "HuffmanGenome.h"
#include "RLEGenome.h"
#include "Logger.h"

int main(int argc, char* argv[]) {
    try {
        // For simplicity, define file paths directly
        std::string inputFile = "genome_data.txt";
        std::string huffmanEncodedFile = "genome_data_huffman_encoded.bin";
        std::string huffmanDecodedFile = "genome_data_huffman_decoded.txt";
        std::string rleEncodedFile = "genome_data_rle_encoded.bin";
        std::string rleDecodedFile = "genome_data_rle_decoded.txt";
        std::string frequencyMapFile = "frequency_map.txt";

        Logger::getInstance().log("Starting compression...");

        // Huffman Encoding and Decoding
        HuffmanGenome huffman;
        huffman.encodeFromFile(inputFile, huffmanEncodedFile);
        huffman.saveFrequencyMap(frequencyMapFile);

        HuffmanGenome huffmanDecoder;
        huffmanDecoder.loadFrequencyMap(frequencyMapFile);
        huffmanDecoder.decodeFromFile(huffmanEncodedFile, huffmanDecodedFile);

        // Validate decoded data
        if (huffman.validateDecodedFile(inputFile, huffmanDecodedFile)) {
            Logger::getInstance().log("Huffman decoded data is valid.");
        } else {
            Logger::getInstance().log("Huffman decoded data is invalid.");
        }

        // Get and print Huffman compression metrics
        CompressionMetrics huffmanMetrics = huffman.getMetrics();
        huffmanMetrics.printMetrics();

        // RLE Encoding and Decoding
        RLEGenome rle;
        rle.encodeFromFile(inputFile, rleEncodedFile);
        rle.decodeFromFile(rleEncodedFile, rleDecodedFile);

        // Validate decoded data
        if (rle.validateDecodedFile(inputFile, rleDecodedFile)) {
            Logger::getInstance().log("RLE decoded data is valid.");
        } else {
            Logger::getInstance().log("RLE decoded data is invalid.");
        }

        // Get and print RLE compression metrics
        CompressionMetrics rleMetrics = rle.getMetrics();
        rleMetrics.printMetrics();

        Logger::getInstance().log("Compression completed successfully.");

    } catch (const std::exception& e) {
        Logger::getInstance().log(std::string("Exception: ") + e.what());
        return 1;
    }

    return 0;
}
