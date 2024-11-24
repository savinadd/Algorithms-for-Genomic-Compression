// RLEGenome.cpp

#include "RLEGenome.h"
#include "Logger.h"
#include <fstream>
#include <sstream>
#include <bitset>
#include <cstring>
#include <stdexcept>
#include <vector>

const int COUNT_BITS = 16; // Using 16 bits for counts
const size_t BUFFER_SIZE = 65536; // 64 KB buffer

RLEGenome::RLEGenome() {
    metrics = CompressionMetrics();
}

size_t RLEGenome::getFileSize(const std::string& filename) {
    std::ifstream infile(filename, std::ios::binary | std::ios::ate);
    if (!infile) {
        throw std::runtime_error("Error: Unable to open file '" + filename + "' to get file size.");
    }
    return static_cast<size_t>(infile.tellg());
}

// Encode the genomic sequence using Run-Length Encoding from a file
void RLEGenome::encodeFromFile(const std::string& inputFilename, const std::string& outputFilename) {
    try {
        Logger::getInstance().log("Starting RLE encoding...");

        // Reset metrics
        metrics = CompressionMetrics();

        // Open input file
        std::ifstream infile(inputFilename, std::ios::binary);
        if (!infile) {
            throw std::runtime_error("Error: Unable to open input file '" + inputFilename + "'.");
        }

        // Open output file
        std::ofstream outfile(outputFilename, std::ios::binary);
        if (!outfile) {
            infile.close();
            throw std::runtime_error("Error: Unable to open output file '" + outputFilename + "'.");
        }

        // Get file size for progress tracking
        size_t fileSize = getFileSize(inputFilename);
        size_t totalBytesRead = 0;

        char currentChar = '\0';
        int count = 0;
        char buffer[BUFFER_SIZE];
        bool firstChar = true;

        while (infile.read(buffer, sizeof(buffer)) || infile.gcount()) {
            std::streamsize bytesRead = infile.gcount();
            totalBytesRead += bytesRead;

            for (std::streamsize i = 0; i < bytesRead; ++i) {
                char ch = buffer[i];

                // Validate input character
                if (ch != 'A' && ch != 'C' && ch != 'G' && ch != 'T') {
                    throw std::runtime_error("Error: Invalid character '" + std::string(1, ch) + "' in input file.");
                }

                if (firstChar) {
                    currentChar = ch;
                    count = 1;
                    firstChar = false;
                } else if (ch == currentChar) {
                    count++;
                    if (count > (1 << COUNT_BITS) - 1) {
                        throw std::runtime_error("Error: Run length too long for encoding.");
                    }
                } else {
                    // Write currentChar and count to output file
                    // Encode character as 2 bits
                    std::bitset<2> charBits;
                    switch (currentChar) {
                        case 'A': charBits = 0b00; break;
                        case 'C': charBits = 0b01; break;
                        case 'G': charBits = 0b10; break;
                        case 'T': charBits = 0b11; break;
                    }

                    // Encode count as fixed-size binary
                    std::bitset<COUNT_BITS> countBits(count);

                    // Write bits to output file
                    outfile.write((char*)&charBits, sizeof(charBits));
                    outfile.write((char*)&countBits, sizeof(countBits));

                    // Update metrics
                    metrics.addOriginalSize(count * 2); // Each nucleotide is 2 bits
                    metrics.addCompressedSize(2 + COUNT_BITS); // Character bits + count bits

                    // Reset for next character
                    currentChar = ch;
                    count = 1;
                }
            }

            // Log progress
            // int progress = static_cast<int>(100.0 * totalBytesRead / fileSize);
            // Logger::getInstance().log("RLE Encoding: " + std::to_string(progress) + "%");
        }

        // Write the last character and its count
        if (!firstChar) {
            // Encode character as 2 bits
            std::bitset<2> charBits;
            switch (currentChar) {
                case 'A': charBits = 0b00; break;
                case 'C': charBits = 0b01; break;
                case 'G': charBits = 0b10; break;
                case 'T': charBits = 0b11; break;
            }

            // Encode count as fixed-size binary
            std::bitset<COUNT_BITS> countBits(count);

            // Write bits to output file
            outfile.write((char*)&charBits, sizeof(charBits));
            outfile.write((char*)&countBits, sizeof(countBits));

            // Update metrics
            metrics.addOriginalSize(count * 2); // Each nucleotide is 2 bits
            metrics.addCompressedSize(2 + COUNT_BITS); // Character bits + count bits
        }

        infile.close();
        outfile.close();

        Logger::getInstance().log("RLE encoding completed.");
    } catch (const std::exception& e) {
        Logger::getInstance().log(std::string("Exception during RLE encoding: ") + e.what());
    }
}

void RLEGenome::decodeFromFile(const std::string& inputFilename, const std::string& outputFilename) {
    try {
        Logger::getInstance().log("Starting RLE decoding...");

        // Open input file
        std::ifstream infile(inputFilename, std::ios::binary);
        if (!infile) {
            throw std::runtime_error("Error: Unable to open input file '" + inputFilename + "'.");
        }

        // Open output file
        std::ofstream outfile(outputFilename, std::ios::binary);
        if (!outfile) {
            infile.close();
            throw std::runtime_error("Error: Unable to open output file '" + outputFilename + "'.");
        }

        Logger::getInstance().log("Decoding...");

        while (!infile.eof()) {
            std::bitset<2> charBits;
            std::bitset<COUNT_BITS> countBits;

            // Read character bits
            infile.read((char*)&charBits, sizeof(charBits));
            if (infile.gcount() != sizeof(charBits)) break; // End of file

            // Read count bits
            infile.read((char*)&countBits, sizeof(countBits));
            if (infile.gcount() != sizeof(countBits)) {
                throw std::runtime_error("Error: Incomplete count bits in input file.");
            }

            char currentChar;
            if (charBits == 0b00) currentChar = 'A';
            else if (charBits == 0b01) currentChar = 'C';
            else if (charBits == 0b10) currentChar = 'G';
            else if (charBits == 0b11) currentChar = 'T';
            else {
                throw std::runtime_error("Error: Invalid character bits in input file.");
            }

            int count = static_cast<int>(countBits.to_ulong());
            if (count <= 0) {
                throw std::runtime_error("Error: Invalid count in input file.");
            }

            // Write to output file
            std::string sequence(count, currentChar);
            outfile.write(sequence.c_str(), count);
        }

        infile.close();
        outfile.close();

        Logger::getInstance().log("RLE decoding completed.");
    } catch (const std::exception& e) {
        Logger::getInstance().log(std::string("Exception during RLE decoding: ") + e.what());
    }
}

std::string RLEGenome::encode(const std::string& sequence) {
    // Implement if needed, include exception handling
    return "";
}

std::string RLEGenome::decode(const std::string& encodedSequence) {
    // Implement if needed, include exception handling
    return "";
}

CompressionMetrics RLEGenome::getMetrics() const {
    return metrics;
}

bool RLEGenome::validateDecodedFile(const std::string& originalFilename, const std::string& decodedFilename) {
    try {
        Logger::getInstance().log("Validating decoded file...");

        const size_t BUFFER_SIZE = 65536; // 64 KB buffer
        std::ifstream originalFile(originalFilename, std::ios::binary);
        std::ifstream decodedFile(decodedFilename, std::ios::binary);

        if (!originalFile.is_open()) {
            throw std::runtime_error("Error: Unable to open original file '" + originalFilename + "'.");
        }
        if (!decodedFile.is_open()) {
            throw std::runtime_error("Error: Unable to open decoded file '" + decodedFilename + "'.");
        }

        // Allocate buffers on the heap
        std::vector<char> originalBuffer(BUFFER_SIZE);
        std::vector<char> decodedBuffer(BUFFER_SIZE);

        while (true) {
            // Read from original file
            originalFile.read(originalBuffer.data(), BUFFER_SIZE);
            std::streamsize originalBytesRead = originalFile.gcount();

            // Read from decoded file
            decodedFile.read(decodedBuffer.data(), BUFFER_SIZE);
            std::streamsize decodedBytesRead = decodedFile.gcount();

            // Check if bytes read are equal
            if (originalBytesRead != decodedBytesRead) {
                Logger::getInstance().log("Error: Files have different sizes.");
                return false;
            }

            // If no bytes were read, we've reached EOF for both files
            if (originalBytesRead == 0) {
                break;
            }

            // Compare the buffers
            if (std::memcmp(originalBuffer.data(), decodedBuffer.data(), static_cast<size_t>(originalBytesRead)) != 0) {
                Logger::getInstance().log("Error: Files differ.");
                return false;
            }
        }

        Logger::getInstance().log("Validation successful. Decoded file matches the original.");
        return true;
    } catch (const std::exception& e) {
        Logger::getInstance().log(std::string("Exception during validation: ") + e.what());
        return false;
    }
}
