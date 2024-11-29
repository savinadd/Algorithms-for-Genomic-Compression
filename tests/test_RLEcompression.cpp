#include <gtest/gtest.h>
#include "../include/RLEGenome.h"
#include <fstream>
#include <logger.h>

TEST(RLEGenomeTest, Constructor)
{
    EXPECT_NO_THROW(RLEGenome genome);
}

TEST(RLEGenomeTest, ValidateInputFile)
{
    Logger::getInstance().enableLogging(false);
    RLEGenome genome;

    std::string validFile = "valid_test.txt";
    std::ofstream valid(validFile);
    valid << "ACGTACGT";
    valid.close();

    std::string invalidFile = "invalid_test.txt";
    std::ofstream invalid(invalidFile);
    invalid << "ACGT1234";
    invalid.close();

    std::string invalidExtFile = "invalid_test.bin";
    std::ofstream invalidExt(invalidExtFile);
    invalidExt << "ACGTACGT";
    invalidExt.close();

    EXPECT_TRUE(genome.validateInputFile(validFile));
    EXPECT_FALSE(genome.validateInputFile(invalidFile));
    EXPECT_FALSE(genome.validateInputFile(invalidExtFile));

    std::remove(validFile.c_str());
    std::remove(invalidFile.c_str());
    std::remove(invalidExtFile.c_str());
}

TEST(RLEGenomeTest, EncodeFromFile)
{
    Logger::getInstance().enableLogging(false);
    RLEGenome genome;

    std::string inputFile = "test_input.txt";
    std::ofstream input(inputFile);
    input << "AAAACCCGGGTTT";
    input.close();

    std::string outputFile = "test_output.rle";

    EXPECT_NO_THROW(genome.encodeFromFile(inputFile, outputFile));

    std::ifstream output(outputFile);
    EXPECT_TRUE(output.is_open());
    output.close();

    std::remove(inputFile.c_str());
    std::remove(outputFile.c_str());
}

TEST(RLEGenomeTest, DecodeFromFile)
{
    Logger::getInstance().enableLogging(false);
    RLEGenome genome;

    std::string inputFile = "test_input.txt";
    std::ofstream input(inputFile);
    input << "AAAACCCGGGTTT";
    input.close();

    std::string compressedFile = "test_output.rle";
    std::string decompressedFile = "test_decoded.txt";

    EXPECT_NO_THROW(genome.encodeFromFile(inputFile, compressedFile));
    EXPECT_NO_THROW(genome.decodeFromFile(compressedFile, decompressedFile));

    std::ifstream decompressed(decompressedFile);
    std::ostringstream content;
    content << decompressed.rdbuf();
    EXPECT_EQ(content.str(), "AAAACCCGGGTTT");

    std::remove(inputFile.c_str());
    std::remove(compressedFile.c_str());
    std::remove(decompressedFile.c_str());
}

TEST(RLEGenomeTest, CompressionMetrics)
{
    Logger::getInstance().enableLogging(false);
    RLEGenome genome;

    std::string inputFile = "test_input.txt";
    std::ofstream input(inputFile);
    input << "AAAACCCGGGTTT";
    input.close();

    std::string outputFile = "test_output.rle";

    EXPECT_NO_THROW(genome.encodeFromFile(inputFile, outputFile));

    CompressionMetrics metrics = genome.getMetrics();
    EXPECT_GT(metrics.getOriginalSize(), 0);
    EXPECT_GT(metrics.getCompressedSize(), 0);
    EXPECT_LE(metrics.getCompressedSize(), metrics.getOriginalSize());

    std::remove(inputFile.c_str());
    std::remove(outputFile.c_str());
}

TEST(RLEGenomeTest, ValidateDecodedFile)
{
    Logger::getInstance().enableLogging(false);
    RLEGenome genome;

    std::string inputFile = "test_input.txt";
    std::ofstream input(inputFile);
    input << "AAAACCCGGGTTT";
    input.close();

    std::string compressedFile = "test_output.rle";
    std::string decompressedFile = "test_decoded.txt";

    EXPECT_NO_THROW(genome.encodeFromFile(inputFile, compressedFile));
    EXPECT_NO_THROW(genome.decodeFromFile(compressedFile, decompressedFile));

    EXPECT_TRUE(genome.validateDecodedFile(inputFile, decompressedFile));

    std::remove(inputFile.c_str());
    std::remove(compressedFile.c_str());
    std::remove(decompressedFile.c_str());
}