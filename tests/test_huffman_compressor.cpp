#include <gtest/gtest.h>
#include "../include/HuffmanCompressor.h"
#include <fstream>
#include <string>
#include <logger.h>

// Test constructor
TEST(HuffmanCompressorTest, Constructor)
{
    HuffmanCompressor compressor;
    EXPECT_NO_THROW(HuffmanCompressor());
}

// Test frequency map saving and loading
TEST(HuffmanCompressorTest, SaveAndLoadFrequencyMap)
{
    Logger::getInstance().enableLogging(false);
    HuffmanCompressor compressor;

    // Set up a fake frequency map
    compressor.frequencyMap['A'] = 5;
    compressor.frequencyMap['B'] = 3;
    compressor.frequencyMap['C'] = 8;

    // Save frequency map to a file
    std::string freqFile = "test.freq";
    EXPECT_NO_THROW(compressor.saveFrequencyMap(freqFile));

    // Load the frequency map
    HuffmanCompressor newCompressor;
    EXPECT_NO_THROW(newCompressor.loadFrequencyMap(freqFile));

    // Check that the frequency map was loaded correctly
    EXPECT_EQ(newCompressor.frequencyMap['A'], 5);
    EXPECT_EQ(newCompressor.frequencyMap['B'], 3);
    EXPECT_EQ(newCompressor.frequencyMap['C'], 8);

    // Clean up test file
    std::remove(freqFile.c_str());
}

// Test encoding from file
TEST(HuffmanCompressorTest, EncodeFromFile)
{
    Logger::getInstance().enableLogging(false);
    HuffmanCompressor compressor;

    // Create a temporary input file
    std::string inputFile = "test_input.txt";
    std::ofstream input(inputFile);
    input << "AAAABBBCCD"; // Simple test data
    input.close();

    // Output compressed file
    std::string outputFile = "test_output.huff";

    // Encode the file
    EXPECT_NO_THROW(compressor.encodeFromFile(inputFile, outputFile));

    // Check that the output file exists
    std::ifstream output(outputFile);
    EXPECT_TRUE(output.is_open());
    output.close();

    // Clean up test files
    std::remove(inputFile.c_str());
    std::remove(outputFile.c_str());
    std::remove((outputFile + ".freq").c_str()); // Frequency map file
}

// Test decoding from file
TEST(HuffmanCompressorTest, DecodeFromFile)
{
    Logger::getInstance().enableLogging(false);
    HuffmanCompressor compressor;

    // Create a temporary input file
    std::string inputFile = "test_input.txt";
    std::ofstream input(inputFile);
    input << "AAAABBBCCD"; // Simple test data
    input.close();

    // Compressed and decompressed file
    std::string compressedFile = "test_output.huff";
    std::string decompressedFile = "test_output_decoded.txt";

    // Encode and decode the file
    EXPECT_NO_THROW(compressor.encodeFromFile(inputFile, compressedFile));
    EXPECT_NO_THROW(compressor.decodeFromFile(compressedFile, decompressedFile));

    // Check that the decompressed file matches the original
    std::ifstream decompressed(decompressedFile);
    std::ostringstream decompressedContent;
    decompressedContent << decompressed.rdbuf();
    EXPECT_EQ(decompressedContent.str(), "AAAABBBCCD");

    // Clean up test files
    std::remove(inputFile.c_str());
    std::remove(compressedFile.c_str());
    std::remove((compressedFile + ".freq").c_str()); // Frequency map file
    std::remove(decompressedFile.c_str());
}

// Test validation of decoded file
TEST(HuffmanCompressorTest, ValidateDecodedFile)
{
    Logger::getInstance().enableLogging(false);
    HuffmanCompressor compressor;

    // Create a temporary input file
    std::string inputFile = "test_input.txt";
    std::ofstream input(inputFile);
    input << "AAAABBBCCD"; // Simple test data
    input.close();

    // Compressed and decompressed file
    std::string compressedFile = "test_output.huff";
    std::string decompressedFile = "test_output_decoded.txt";

    // Encode, decode, and validate
    EXPECT_NO_THROW(compressor.encodeFromFile(inputFile, compressedFile));
    EXPECT_NO_THROW(compressor.decodeFromFile(compressedFile, decompressedFile));
    EXPECT_TRUE(compressor.validateDecodedFile(inputFile, decompressedFile));

    // Clean up test files
    std::remove(inputFile.c_str());
    std::remove(compressedFile.c_str());
    std::remove((compressedFile + ".freq").c_str()); // Frequency map file
    std::remove(decompressedFile.c_str());
}
