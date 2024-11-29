#include <gtest/gtest.h>
#include "../include/HuffmanGenome.h"
#include <fstream>
#include <logger.h>

TEST(HuffmanGenomeTest, ConstructorDestructor)
{
    EXPECT_NO_THROW(HuffmanGenome genome);
}

TEST(HuffmanGenomeTest, SaveAndLoadFrequencyMap)
{
    Logger::getInstance().enableLogging(false);
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

TEST(HuffmanGenomeTest, EncodeFromFile)
{
    Logger::getInstance().enableLogging(false);
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

TEST(HuffmanGenomeTest, DecodeFromFile)
{
    Logger::getInstance().enableLogging(false);
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

TEST(HuffmanGenomeTest, ValidateInputFile)
{
    Logger::getInstance().enableLogging(false);
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
