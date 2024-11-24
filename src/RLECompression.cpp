// RLEGenome.cpp

#include "RLEGenome.h"
#include <sstream>
#include <fstream>
#include <bitset>
#include <cstring>
#include <iostream>
#include <vector>

const int COUNT_BITS = 16; // Using 16 bits for counts
const size_t BUFFER_SIZE = 1024 * 1024; // 1 MB buffer size

RLEGenome::RLEGenome() : metrics() {}

// Encode the genomic sequence using Run-Length Encoding from a file
void RLEGenome::encodeFromFile(const std::string& inputFilename, const std::string& outputFilename) {
    // Reset metrics
    metrics = CompressionMetrics();

    // Open input file
    std::ifstream infile(inputFilename, std::ios::binary);
    if (!infile) {
        std::cerr << "Error: Unable to open input file '" << inputFilename << "'." << std::endl;
        return;
    }

    // Open output file
    std::ofstream outfile(outputFilename, std::ios::binary);
    if (!outfile) {
        std::cerr << "Error: Unable to open output file '" << outputFilename << "'." << std::endl;
        infile.close();
        return;
    }

    char currentChar = '\0';
    int count = 0;
    char buffer[BUFFER_SIZE];
    bool firstChar = true;

    while (infile.read(buffer, sizeof(buffer)) || infile.gcount()) {
        std::streamsize bytesRead = infile.gcount();
        for (std::streamsize i = 0; i < bytesRead; ++i) {
            char ch = buffer[i];

            // Validate input character
            if (ch != 'A' && ch != 'C' && ch != 'G' && ch != 'T') {
                std::cerr << "Error: Invalid character '" << ch << "' in input file." << std::endl;
                infile.close();
                outfile.close();
                return;
            }

            if (firstChar) {
                currentChar = ch;
                count = 1;
                firstChar = false;
            } else if (ch == currentChar) {
                count++;
                if (count > (1 << COUNT_BITS) - 1) {
                    std::cerr << "Error: Run length too long for encoding." << std::endl;
                    infile.close();
                    outfile.close();
                    return;
                }
            } else {
                // Write currentChar and count to output file
                // Encode character as 2 bits
                unsigned char charBits = 0;
                switch (currentChar) {
                    case 'A': charBits = 0b00; break;
                    case 'C': charBits = 0b01; break;
                    case 'G': charBits = 0b10; break;
                    case 'T': charBits = 0b11; break;
                }

                // Write charBits and count to output file
                outfile.put(charBits);
                outfile.write(reinterpret_cast<char*>(&count), sizeof(count));

                // Update metrics
                metrics.addOriginalSize(count * 2); // Each nucleotide is 2 bits
                metrics.addCompressedSize(2 + COUNT_BITS); // Character bits + count bits

                // Reset for next character
                currentChar = ch;
                count = 1;
            }
        }
    }

    // Write the last character and its count
    if (!firstChar) {
        // Encode character as 2 bits
        unsigned char charBits = 0;
        switch (currentChar) {
            case 'A': charBits = 0b00; break;
            case 'C': charBits = 0b01; break;
            case 'G': charBits = 0b10; break;
            case 'T': charBits = 0b11; break;
        }

        // Write charBits and count to output file
        outfile.put(charBits);
        outfile.write(reinterpret_cast<char*>(&count), sizeof(count));

        // Update metrics
        metrics.addOriginalSize(count * 2); // Each nucleotide is 2 bits
        metrics.addCompressedSize(2 + COUNT_BITS); // Character bits + count bits
    }

    infile.close();
    outfile.close();
}

// Decode the encoded sequence back to its original form from a file
void RLEGenome::decodeFromFile(const std::string& inputFilename, const std::string& outputFilename) {
    // Open input file
    std::ifstream infile(inputFilename, std::ios::binary);
    if (!infile) {
        std::cerr << "Error: Unable to open input file '" << inputFilename << "'." << std::endl;
        return;
    }

    // Open output file
    std::ofstream outfile(outputFilename, std::ios::binary);
    if (!outfile) {
        std::cerr << "Error: Unable to open output file '" << outputFilename << "'." << std::endl;
        infile.close();
        return;
    }

    while (!infile.eof()) {
        unsigned char charBits;
        int count;

        // Read character bits
        infile.read(reinterpret_cast<char*>(&charBits), sizeof(charBits));
        if (infile.gcount() != sizeof(charBits)) break; // End of file

        // Read count
        infile.read(reinterpret_cast<char*>(&count), sizeof(count));
        if (infile.gcount() != sizeof(count)) {
            std::cerr << "Error: Incomplete count in input file." << std::endl;
            infile.close();
            outfile.close();
            return;
        }

        char currentChar;
        if (charBits == 0b00) currentChar = 'A';
        else if (charBits == 0b01) currentChar = 'C';
        else if (charBits == 0b10) currentChar = 'G';
        else if (charBits == 0b11) currentChar = 'T';
        else {
            std::cerr << "Error: Invalid character bits in input file." << std::endl;
            infile.close();
            outfile.close();
            return;
        }

        if (count <= 0) {
            std::cerr << "Error: Invalid count in input file." << std::endl;
            infile.close();
            outfile.close();
            return;
        }

        // Write to output file
        std::string sequence(count, currentChar);
        outfile.write(sequence.c_str(), count);
    }

    infile.close();
    outfile.close();
}

// Validate the decoded file against the original
bool RLEGenome::validateDecodedFile(const std::string& originalFilename, const std::string& decodedFilename) {
    try {
        const size_t BUFFER_SIZE = 65536; // 64 KB buffer
        std::ifstream originalFile(originalFilename, std::ios::binary);
        std::ifstream decodedFile(decodedFilename, std::ios::binary);

        if (!originalFile.is_open()) {
            std::cerr << "Error: Unable to open original file '" << originalFilename << "'." << std::endl;
            return false;
        }
        if (!decodedFile.is_open()) {
            std::cerr << "Error: Unable to open decoded file '" << decodedFilename << "'." << std::endl;
            return false;
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
                std::cerr << "Error: Files have different sizes." << std::endl;
                return false;
            }

            // If no bytes were read, we've reached EOF for both files
            if (originalBytesRead == 0) {
                break;
            }

            // Compare the buffers
            if (std::memcmp(originalBuffer.data(), decodedBuffer.data(), static_cast<size_t>(originalBytesRead)) != 0) {
                std::cerr << "Error: Files differ." << std::endl;
                return false;
            }
        }

        std::cout << "Validation successful. Decoded file matches the original." << std::endl;
        return true;
    } catch (const std::exception& e) {
        std::cerr << "Exception during validation: " << e.what() << std::endl;
        return false;
    }
}

CompressionMetrics RLEGenome::getMetrics() const {
    return metrics;
}

// Remove or comment out these methods if they're not used
/*
std::string RLEGenome::encode(const std::string& sequence) {
    // If this method is not used, you can remove it or return an empty string
    return "";
}

std::string RLEGenome::decode(const std::string& encodedSequence) {
    // If this method is not used, you can remove it or return an empty string
    return "";
}
*/

