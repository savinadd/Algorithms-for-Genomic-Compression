#include "RLEGenome.h"
#include <fstream>
#include <iostream>
#include <vector>
#include <cstring>
#include <FileValidator.h>
#include <logger.h>

const int COUNT_BITS = 16; 
const size_t BUFFER_SIZE = 1024 * 1024; 

RLEGenome::RLEGenome() : metrics() {}

bool RLEGenome::validateInputFile(const std::string& inputFilename) const {

    if (!FileValidator::hasTxtExtension(inputFilename)) {
        Logger::getInstance().log("Validation Error: File '" + inputFilename + "' does not have a .txt extension.");
        std::cerr << "Error: Unsupported file format. Only .txt files are allowed.\n";
        return false;
    }

    if (!FileValidator::fileExists(inputFilename)) {
        Logger::getInstance().log("Validation Error: File '" + inputFilename + "' does not exist.");
        std::cerr << "Error: File does not exist.\n";
        return false;
    }

    if (!FileValidator::hasValidGenomeData(inputFilename)) {
        Logger::getInstance().log("Validation Error: File '" + inputFilename + "' contains invalid characters.");
        std::cerr << "Error: File contains invalid characters. Only A, C, G, T are allowed.\n";
        return false;
    }

    return true;
}

void RLEGenome::encodeFromFile(const std::string& inputFilename, const std::string& outputFilename) {

    metrics = CompressionMetrics();

     if (!validateInputFile(inputFilename)) {
            Logger::getInstance().log("Encoding aborted due to input file validation failure.");
            return;
    }

    std::ifstream infile(inputFilename, std::ios::binary);
    if (!infile) {
        std::cerr << "Error: Unable to open input file '" << inputFilename << "'." << std::endl;
        return;
    }


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
                unsigned char charBits = 0;
                switch (currentChar) {
                    case 'A': charBits = 0b00; break;
                    case 'C': charBits = 0b01; break;
                    case 'G': charBits = 0b10; break;
                    case 'T': charBits = 0b11; break;
                }

                outfile.put(charBits);
                outfile.write(reinterpret_cast<char*>(&count), sizeof(count));

                // Update metrics
                metrics.addOriginalSize(count * 8); // Each nucleotide is 2 bits (represented as 8 bits here for simplicity)
                metrics.addCompressedSize(2 + COUNT_BITS); // Character bits + count bits

                currentChar = ch;
                count = 1;
            }
        }
    }

    if (!firstChar) {

        unsigned char charBits = 0;
        switch (currentChar) {
            case 'A': charBits = 0b00; break;
            case 'C': charBits = 0b01; break;
            case 'G': charBits = 0b10; break;
            case 'T': charBits = 0b11; break;
        }

        outfile.put(charBits);
        outfile.write(reinterpret_cast<char*>(&count), sizeof(count));

        metrics.addOriginalSize(count * 8); // Each nucleotide is 2 bits (represented as 8 bits here for simplicity)
        metrics.addCompressedSize(2 + COUNT_BITS); // Character bits + count bits
    }

    infile.close();
    outfile.close();
}

void RLEGenome::decodeFromFile(const std::string& inputFilename, const std::string& outputFilename) {

    std::ifstream infile(inputFilename, std::ios::binary);
    if (!infile) {
        std::cerr << "Error: Unable to open input file '" << inputFilename << "'." << std::endl;
        return;
    }

    std::ofstream outfile(outputFilename, std::ios::binary);
    if (!outfile) {
        std::cerr << "Error: Unable to open output file '" << outputFilename << "'." << std::endl;
        infile.close();
        return;
    }

    while (!infile.eof()) {
        unsigned char charBits;
        int count;


        infile.read(reinterpret_cast<char*>(&charBits), sizeof(charBits));
        if (infile.gcount() != sizeof(charBits)) break; 


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

        std::string sequence(count, currentChar);
        outfile.write(sequence.c_str(), count);
    }

    infile.close();
    outfile.close();
}

CompressionMetrics RLEGenome::getMetrics() const {
    return metrics;
}

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

            originalFile.read(originalBuffer.data(), BUFFER_SIZE);
            std::streamsize originalBytesRead = originalFile.gcount();


            decodedFile.read(decodedBuffer.data(), BUFFER_SIZE);
            std::streamsize decodedBytesRead = decodedFile.gcount();

            if (originalBytesRead != decodedBytesRead) {
                std::cerr << "Error: Files have different sizes." << std::endl;
                return false;
            }


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
