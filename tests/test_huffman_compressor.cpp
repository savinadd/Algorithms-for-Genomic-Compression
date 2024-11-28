#define CATCH_CONFIG_MAIN 
#include "catch.hpp"
#include "huffmanCompressor.h"
#include <fstream>
#include <string>
#include <cstdio> 
#include <logger.h>
#include<CompressionMetrics.h>

class HuffmanCompressorTestHelper {
public:
    static void writeToFile(const std::string& filename, const std::string& content) {
        std::ofstream file(filename, std::ios::binary);
        REQUIRE(file.is_open());
        file.write(content.data(), content.size());
        file.close();
    }

    static std::string readFromFile(const std::string& filename) {
        std::ifstream file(filename, std::ios::binary | std::ios::ate);
        REQUIRE(file.is_open());
        std::streamsize size = file.tellg();
        file.seekg(0, std::ios::beg);
        std::string content(size, '\0');
        file.read(&content[0], size);
        file.close();
        return content;
    }

    static void removeFile(const std::string& filename) {
        std::remove(filename.c_str());
    }
};

TEST_CASE("HuffmanCompressor: Encoding and Decoding", "[HuffmanCompressor]") {
    HuffmanCompressor compressor;

    const std::string inputFile = "test_input.txt";
    const std::string encodedFile = "test_encoded.bin";
    const std::string decodedFile = "test_decoded.txt";

    const std::string testContent = "AAAAGGGTCTCTCTCTAAA";

    HuffmanCompressorTestHelper::writeToFile(inputFile, testContent);

    REQUIRE_NOTHROW(compressor.encodeFromFile(inputFile, encodedFile));

    REQUIRE_NOTHROW(compressor.decodeFromFile(encodedFile, decodedFile));

    std::string decodedContent = HuffmanCompressorTestHelper::readFromFile(decodedFile);

    REQUIRE(decodedContent == testContent);

    // Cleanup
    HuffmanCompressorTestHelper::removeFile(inputFile);
    HuffmanCompressorTestHelper::removeFile(encodedFile);
    HuffmanCompressorTestHelper::removeFile(decodedFile);
}

TEST_CASE("HuffmanCompressor: Validate Decoded File", "[HuffmanCompressor]") {
    HuffmanCompressor compressor;

    const std::string inputFile = "original.txt";
    const std::string encodedFile = "encoded.bin";
    const std::string decodedFile = "decoded.txt";

    const std::string testContent = "AAAAAGGGGTTTCCCC";

    HuffmanCompressorTestHelper::writeToFile(inputFile, testContent);

    REQUIRE_NOTHROW(compressor.encodeFromFile(inputFile, encodedFile));

    REQUIRE_NOTHROW(compressor.decodeFromFile(encodedFile, decodedFile));

    REQUIRE(compressor.validateDecodedFile(inputFile, decodedFile) == true);

    HuffmanCompressorTestHelper::writeToFile(decodedFile, "corrupted content");

    REQUIRE(compressor.validateDecodedFile(inputFile, decodedFile) == false);

    // Cleanup
    HuffmanCompressorTestHelper::removeFile(inputFile);
    HuffmanCompressorTestHelper::removeFile(encodedFile);
    HuffmanCompressorTestHelper::removeFile(decodedFile);
}

TEST_CASE("HuffmanCompressor: Handle Empty File", "[HuffmanCompressor]") {
    HuffmanCompressor compressor;

    const std::string inputFile = "empty.txt";
    const std::string encodedFile = "empty_encoded.bin";
    const std::string decodedFile = "empty_decoded.txt";

    HuffmanCompressorTestHelper::writeToFile(inputFile, "");

    REQUIRE_NOTHROW(compressor.encodeFromFile(inputFile, encodedFile));

    REQUIRE_NOTHROW(compressor.decodeFromFile(encodedFile, decodedFile));

    std::string decodedContent = HuffmanCompressorTestHelper::readFromFile(decodedFile);
    REQUIRE(decodedContent.empty());

    // Cleanup
    HuffmanCompressorTestHelper::removeFile(inputFile);
    HuffmanCompressorTestHelper::removeFile(encodedFile);
    HuffmanCompressorTestHelper::removeFile(decodedFile);
}
