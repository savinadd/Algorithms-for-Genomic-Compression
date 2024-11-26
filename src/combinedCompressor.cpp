#include "CombinedCompressor.h"
#include "Logger.h"
#include <cstdio> // For remove()

CombinedCompressor::CombinedCompressor() : metrics() {}

void CombinedCompressor::encodeFromFile(const std::string& inputFilename, const std::string& outputFilename) {
    try {
        Logger::getInstance().log("Starting Combined (RLE + Huffman) encoding...");

        // Step 1: RLE Encoding
        std::string rleOutputFilename = "temp_rle_output.bin";
        rleCompressor.encodeFromFile(inputFilename, rleOutputFilename);

        // Step 2: Huffman Encoding on RLE Output
        huffmanCompressor.encodeFromFile(rleOutputFilename, outputFilename);

        // Combine metrics
        CompressionMetrics rleMetrics = rleCompressor.getMetrics();
        CompressionMetrics huffmanMetrics = huffmanCompressor.getMetrics();

        // Update combined metrics
        metrics.addOriginalSize(rleMetrics.getOriginalSize());
        metrics.addCompressedSize(huffmanMetrics.getCompressedSize());

        // Clean up temporary file
        std::remove(rleOutputFilename.c_str());

        Logger::getInstance().log("Combined encoding completed.");
    } catch (const std::exception& e) {
        Logger::getInstance().log(std::string("Exception during Combined encoding: ") + e.what());
    }
}

void CombinedCompressor::decodeFromFile(const std::string& inputFilename, const std::string& outputFilename) {
    try {
        Logger::getInstance().log("Starting Combined (Huffman + RLE) decoding...");

        // Step 1: Huffman Decoding
        std::string rleOutputFilename = "temp_rle_decoded.bin";
        huffmanCompressor.decodeFromFile(inputFilename, rleOutputFilename);

        // Step 2: RLE Decoding
        rleCompressor.decodeFromFile(rleOutputFilename, outputFilename);

        // Clean up temporary file
        std::remove(rleOutputFilename.c_str());

        Logger::getInstance().log("Combined decoding completed.");
    } catch (const std::exception& e) {
        Logger::getInstance().log(std::string("Exception during Combined decoding: ") + e.what());
    }
}

CompressionMetrics CombinedCompressor::getMetrics() const {
    return metrics;
}

bool CombinedCompressor::validateDecodedFile(const std::string& originalFilename, const std::string& decodedFilename) {
    // Reuse the validation method from RLEGenome
    return rleCompressor.validateDecodedFile(originalFilename, decodedFilename);
}
