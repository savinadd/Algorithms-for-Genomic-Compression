// HuffmanGenomeTest.cpp
#include <gtest/gtest.h>
#include "../include/HuffmanGenome.h"
#include <fstream>
#include <logger.h>

// Encapsulate the Test Fixture in an Anonymous Namespace
namespace {
    class SuppressOutputHuffmanGenomeTest : public ::testing::Test {
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
TEST_F(SuppressOutputHuffmanGenomeTest, ConstructorDestructor)
{
    EXPECT_NO_THROW(HuffmanGenome genome);
}

TEST_F(SuppressOutputHuffmanGenomeTest, SaveAndLoadFrequencyMap)
{
    HuffmanGenome genome;

    genome.frequencyMap[HuffmanGenome::A] = 5;
    genome.frequencyMap[HuffmanGenome::C] = 3;
    genome.frequencyMap[HuffmanGenome::G] = 8;
    genome.frequencyMap[HuffmanGenome::T] = 2;

    std::string freqFile = "test.freq";
    EXPECT_NO_THROW(genome.saveFrequencyMap(freqFile));

    HuffmanGenome newGenome;
    EXPECT_NO_THROW(newGenome.loadFrequencyMap(freqFile));

    EXPECT_EQ(newGenome.frequencyMap[HuffmanGenome::A], 5);
    EXPECT_EQ(newGenome.frequencyMap[HuffmanGenome::C], 3);
    EXPECT_EQ(newGenome.frequencyMap[HuffmanGenome::G], 8);
    EXPECT_EQ(newGenome.frequencyMap[HuffmanGenome::T], 2);

    std::remove(freqFile.c_str());
}

TEST_F(SuppressOutputHuffmanGenomeTest, EncodeFromFile)
{
    HuffmanGenome genome;

    std::string inputFile = "test_input.txt";
    std::ofstream input(inputFile);
    input << "ACGTACGT";
    input.close();

    std::string outputFile = "test_output.huff";

    EXPECT_NO_THROW(genome.encodeFromFile(inputFile, outputFile));

    std::ifstream output(outputFile);
    EXPECT_TRUE(output.is_open());
    output.close();

    std::remove(inputFile.c_str());
    std::remove(outputFile.c_str());
    std::remove((outputFile + ".freq").c_str());
}

TEST_F(SuppressOutputHuffmanGenomeTest, DecodeFromFile)
{
    HuffmanGenome genome;

    std::string inputFile = "test_input.txt";
    std::ofstream input(inputFile);
    input << "ACGTACGT";
    input.close();

    std::string compressedFile = "test_output.huff";
    std::string decompressedFile = "test_decoded.txt";

    EXPECT_NO_THROW(genome.encodeFromFile(inputFile, compressedFile));
    EXPECT_NO_THROW(genome.decodeFromFile(compressedFile, decompressedFile));

    std::ifstream decompressed(decompressedFile);
    std::ostringstream content;
    content << decompressed.rdbuf();
    EXPECT_EQ(content.str(), "ACGTACGT");

    std::remove(inputFile.c_str());
    std::remove(compressedFile.c_str());
    std::remove((compressedFile + ".freq").c_str());
    std::remove(decompressedFile.c_str());
}

TEST_F(SuppressOutputHuffmanGenomeTest, ValidateInputFile)
{
    HuffmanGenome genome;

    std::string validFile = "valid_test.txt";
    std::ofstream valid(validFile);
    valid << "ACGTACGT";
    valid.close();

    std::string invalidFile = "invalid_test.txt";
    std::ofstream invalid(invalidFile);
    invalid << "ACGT1234";
    invalid.close();

    EXPECT_TRUE(genome.validateInputFile(validFile));
    EXPECT_FALSE(genome.validateInputFile(invalidFile));

    std::remove(validFile.c_str());
    std::remove(invalidFile.c_str());
}
