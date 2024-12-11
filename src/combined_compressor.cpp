#include "CombinedCompressor.h"
#include "Logger.h"
#include <cstdio>
#include <stdexcept>
#include <RLEGenome.h>
#include <CompressionException.h>

CombinedCompressor::CombinedCompressor() : rleCompressor(), huffmanCompressor(), metrics() {}

bool CombinedCompressor::validateInputFile(const std::string &inputFilename) const
{
    return rleCompressor.validateInputFile(inputFilename);
}
void CombinedCompressor::encodeFromFile(const std::string &inputFilename, const std::string &outputFilename)
{
    try
    {
        Logger::getInstance().log("Starting Combined (RLE + Huffman) encoding...");

        if (!validateInputFile(inputFilename))
        {
            Logger::getInstance().log("Combined Compression aborted due to input file validation failure.");
            return;
        }

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
    }
    catch (const CompressionException &ce)
    {
        Logger::getInstance().log(std::string("CompressionException during Combined Compression: ") + ce.what());
        std::cerr << ce.what() << "\n";
    }
    catch (const std::exception &e)
    {
        Logger::getInstance().log(std::string("Exception during Combined Compression: ") + e.what());
        std::cerr << "An unexpected error occurred: " << e.what() << "\n";
    }
}

void CombinedCompressor::decodeFromFile(const std::string &inputFilename, const std::string &outputFilename)
{
    try
    {
        Logger::getInstance().log("Starting Combined (Huffman + RLE) decoding...");

        // Step 1: Huffman Decoding
        std::string rleOutputFilename = "temp_rle_decoded.bin";
        huffmanCompressor.decodeFromFile(inputFilename, rleOutputFilename);

        // Step 2: RLE Decoding
        rleCompressor.decodeFromFile(rleOutputFilename, outputFilename);

        // Clean up temporary file
        std::remove(rleOutputFilename.c_str());

        Logger::getInstance().log("Combined decoding completed.");
    }
    catch (const CompressionException &ce)
    {
        Logger::getInstance().log(std::string("CompressionException during Combined Decompression: ") + ce.what());
        std::cerr << ce.what() << "\n";
    }
    catch (const std::exception &e)
    {
        Logger::getInstance().log(std::string("Exception during Combined Decompression: ") + e.what());
        std::cerr << "An unexpected error occurred: " << e.what() << "\n";
    }
}

CompressionMetrics CombinedCompressor::getMetrics() const
{
    return metrics;
}

bool CombinedCompressor::validateDecodedFile(const std::string &originalFilename, const std::string &decodedFilename)
{
    return rleCompressor.validateDecodedFile(originalFilename, decodedFilename);
}
