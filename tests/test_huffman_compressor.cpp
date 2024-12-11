// HuffmanCompressorTest.cpp
#include <gtest/gtest.h>
#include "../include/HuffmanCompressor.h"
#include <fstream>
#include <string>
#include <logger.h>

// Encapsulate the Test Fixture in an Anonymous Namespace
namespace {
    class SuppressOutputHuffmanCompressorTest : public ::testing::Test {
    protected:
        std::streambuf* original_cout;
        std::streambuf* original_cerr;
        std::ofstream null_stream;

        void SetUp() override {
            // Disable logging before any test code runs
            Logger::getInstance().enableLogging(false);

            // Open the null device based on the operating system
        #ifdef _WIN32
            null_stream.open("nul");
        #else
            null_stream.open("/dev/null");
        #endif
            if (!null_stream.is_open()) {
                FAIL() << "Failed to open null device for output suppression.";
            }

            // Redirect std::cout and std::cerr to the null device
            original_cout = std::cout.rdbuf(null_stream.rdbuf());
            original_cerr = std::cerr.rdbuf(null_stream.rdbuf());
        }

        void TearDown() override {
            // Restore the original buffers
            std::cout.rdbuf(original_cout);
            std::cerr.rdbuf(original_cerr);

            // Close the null device
            null_stream.close();
        }
    };
}

// Test Cases Modified to Use TEST_F with Unique Fixture
TEST_F(SuppressOutputHuffmanCompressorTest, Constructor)
{
    EXPECT_NO_THROW(HuffmanCompressor compressor);
    EXPECT_NO_THROW(HuffmanCompressor());
}

TEST_F(SuppressOutputHuffmanCompressorTest, SaveAndLoadFrequencyMap)
{
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

TEST_F(SuppressOutputHuffmanCompressorTest, EncodeFromFile)
{
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

TEST_F(SuppressOutputHuffmanCompressorTest, DecodeFromFile)
{
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
    std::ostringstream content;
    content << decompressed.rdbuf();
    EXPECT_EQ(content.str(), "AAAABBBCCD");

    // Clean up test files
    std::remove(inputFile.c_str());
    std::remove(compressedFile.c_str());
    std::remove((compressedFile + ".freq").c_str()); // Frequency map file
    std::remove(decompressedFile.c_str());
}

TEST_F(SuppressOutputHuffmanCompressorTest, ValidateDecodedFile)
{
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
