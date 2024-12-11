// CombinedCompressorTest.cpp
#include <gtest/gtest.h>
#include "../include/CombinedCompressor.h"
#include <fstream>
#include <random>
#include <logger.h>

// Encapsulate the Test Fixture in an Anonymous Namespace
namespace {
    class SuppressOutputCombinedCompressorTest : public ::testing::Test {
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

// Helper Function (No changes needed)
void generateLargeRedundantFile(const std::string &filename, size_t size)
{
    std::ofstream file(filename);
    for (size_t i = 0; i < size; ++i)
    {
        file << "ACGT";
    }
    file.close();
}

// Modified Tests Using TEST_F with Unique Fixture
TEST_F(SuppressOutputCombinedCompressorTest, Constructor)
{
    EXPECT_NO_THROW(CombinedCompressor compressor);
}

TEST_F(SuppressOutputCombinedCompressorTest, ValidateInputFile)
{
    CombinedCompressor compressor;

    std::string validFile = "valid_large_test.txt";
    generateLargeRedundantFile(validFile, 100000);

    EXPECT_TRUE(compressor.validateInputFile(validFile));

    std::remove(validFile.c_str());
}

TEST_F(SuppressOutputCombinedCompressorTest, EncodeLargeFile)
{
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

TEST_F(SuppressOutputCombinedCompressorTest, DecodeLargeFile)
{
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

    // EXPECT_EQ(content.str(), originalContent.str());

    std::remove(inputFile.c_str());
    std::remove(compressedFile.c_str());
    std::remove(decompressedFile.c_str());
}

TEST_F(SuppressOutputCombinedCompressorTest, CompressionMetricsLargeFile)
{
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

TEST_F(SuppressOutputCombinedCompressorTest, ValidateDecodedFileLarge)
{
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
