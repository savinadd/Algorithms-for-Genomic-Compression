#include <gtest/gtest.h>
#include "../include/CombinedCompressor.h"
#include <fstream>
#include <random>
#include <logger.h>

void generateLargeRedundantFile(const std::string &filename, size_t size)
{
    std::ofstream file(filename);
    for (size_t i = 0; i < size; ++i)
    {
        file << "ACGT";
    }
    file.close();
}

TEST(CombinedCompressorTest, Constructor)
{
    EXPECT_NO_THROW(CombinedCompressor compressor);
}

TEST(CombinedCompressorTest, ValidateInputFile)
{
    Logger::getInstance().enableLogging(false);
    CombinedCompressor compressor;

    std::string validFile = "valid_large_test.txt";
    generateLargeRedundantFile(validFile, 100000);

    EXPECT_TRUE(compressor.validateInputFile(validFile));

    std::remove(validFile.c_str());
}

TEST(CombinedCompressorTest, EncodeLargeFile)
{
    Logger::getInstance().enableLogging(false);
    CombinedCompressor compressor;
    std::string inputFile = "large_test_input.txt";
    generateLargeRedundantFile(inputFile, 50000);
    std::string outputFile = "large_test_output.combined";

    EXPECT_NO_THROW(compressor.encodeFromFile(inputFile, outputFile));

    std::ifstream output(outputFile);
    EXPECT_TRUE(output.is_open());
    output.close();

    std::remove(inputFile.c_str());
    std::remove(outputFile.c_str());
}

TEST(CombinedCompressorTest, DecodeLargeFile)
{
    Logger::getInstance().enableLogging(false);
    CombinedCompressor compressor;

    std::string inputFile = "large_test_input.txt";
    generateLargeRedundantFile(inputFile, 50000);

    std::string compressedFile = "large_test_output.combined";
    std::string decompressedFile = "large_test_decoded.txt";

    EXPECT_NO_THROW(compressor.encodeFromFile(inputFile, compressedFile));
    EXPECT_NO_THROW(compressor.decodeFromFile(compressedFile, decompressedFile));

    std::ifstream decompressed(decompressedFile);
    std::ostringstream content;
    content << decompressed.rdbuf();
    decompressed.close();

    std::ifstream original(inputFile);
    std::ostringstream originalContent;
    originalContent << original.rdbuf();
    original.close();

    //    EXPECT_EQ(content.str(), originalContent.str());

    std::remove(inputFile.c_str());
    std::remove(compressedFile.c_str());
    std::remove(decompressedFile.c_str());
}

TEST(CombinedCompressorTest, CompressionMetricsLargeFile)
{
    Logger::getInstance().enableLogging(false);
    CombinedCompressor compressor;

    std::string inputFile = "large_test_input.txt";
    generateLargeRedundantFile(inputFile, 50000);

    std::string outputFile = "large_test_output.combined";
    EXPECT_NO_THROW(compressor.encodeFromFile(inputFile, outputFile));

    CompressionMetrics metrics = compressor.getMetrics();
    EXPECT_GT(metrics.getOriginalSize(), 0);
    EXPECT_GT(metrics.getCompressedSize(), 0);
    EXPECT_LE(metrics.getCompressedSize(), metrics.getOriginalSize());
    std::remove(inputFile.c_str());
    std::remove(outputFile.c_str());
}

TEST(CombinedCompressorTest, ValidateDecodedFileLarge)
{
    Logger::getInstance().enableLogging(false);
    CombinedCompressor compressor;

    std::string inputFile = "large_test_input.txt";
    generateLargeRedundantFile(inputFile, 50000);
    std::string compressedFile = "large_test_output.combined";
    std::string decompressedFile = "large_test_decoded.txt";

    EXPECT_NO_THROW(compressor.encodeFromFile(inputFile, compressedFile));
    EXPECT_NO_THROW(compressor.decodeFromFile(compressedFile, decompressedFile));

    EXPECT_TRUE(compressor.validateDecodedFile(inputFile, decompressedFile));

    std::remove(inputFile.c_str());
    std::remove(compressedFile.c_str());
    std::remove(decompressedFile.c_str());
}
