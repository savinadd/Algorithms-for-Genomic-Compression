// RLECompressionTest.cpp
#include <gtest/gtest.h>
#include "../include/RLEGenome.h"
#include <fstream>
#include <logger.h>

// Encapsulate the Test Fixture in an Anonymous Namespace
namespace {
    class SuppressOutputRLECompressionTest : public ::testing::Test {
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
TEST_F(SuppressOutputRLECompressionTest, Constructor)
{
    EXPECT_NO_THROW(RLEGenome genome);
}

TEST_F(SuppressOutputRLECompressionTest, ValidateInputFile)
{
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

TEST_F(SuppressOutputRLECompressionTest, EncodeFromFile)
{
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

TEST_F(SuppressOutputRLECompressionTest, DecodeFromFile)
{
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

TEST_F(SuppressOutputRLECompressionTest, CompressionMetrics)
{
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

TEST_F(SuppressOutputRLECompressionTest, ValidateDecodedFile)
{
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
