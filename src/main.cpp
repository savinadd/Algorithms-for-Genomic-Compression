#include <iostream>
#include <fstream>
#include <string>

// Include your classes
#include "HuffmanGenome.h"
#include "RLEGenome.h"

// Function prototypes
void displayWelcomeScreen();
void displayMenu();
std::string uploadFile();
std::string chooseAlgorithm();
void compressFile(const std::string& sequence, const std::string& algorithm);

int main() {
    displayWelcomeScreen();

    std::string sequence;
    std::string algorithm;

    while (true) {
        displayMenu();

        int choice;
        std::cin >> choice;
        std::cin.ignore(); // Clear the newline character from the buffer

        switch (choice) {
            case 1:
                sequence = uploadFile();
                break;
            case 2:
                algorithm = chooseAlgorithm();
                break;
            case 3:
                if (sequence.empty()) {
                    std::cerr << "Error: No file uploaded. Please upload a file first.\n";
                } else if (algorithm.empty()) {
                    std::cerr << "Error: No algorithm selected. Please choose a compression algorithm.\n";
                } else {
                    compressFile(sequence, algorithm);
                }
                break;
            case 4:
                std::cout << "Exiting the application. Goodbye!\n";
                return 0;
            default:
                std::cerr << "Invalid choice. Please try again.\n";
                break;
        }

        std::cout << "\n";
    }

    return 0;
}

// Function definitions

void displayWelcomeScreen() {
    std::cout << "=============================\n";
    std::cout << "   GenomeCompressor Tool\n";
    std::cout << "=============================\n\n";
    std::cout << "Welcome! This tool compresses genomic data using Huffman and Run-Length Encoding algorithms.\n\n";
}

void displayMenu() {
    std::cout << "Please select an option:\n";
    std::cout << "1. Upload a file\n";
    std::cout << "2. Choose compression algorithm\n";
    std::cout << "3. Compress the file\n";
    std::cout << "4. Exit\n";
    std::cout << "Enter your choice: ";
}

std::string uploadFile() {
    //std::string filePath;
    std::cout << "Enter the file path to upload: ";
    std::cin.ignore(); // Clear the newline character from the buffer
    //std::getline(std::cin, filePath);

    std::string filePath = "C:/Users/Savina/Desktop/test.txt";

    // Read the file content
    std::ifstream infile(filePath);
    if (!infile) {
        std::cerr << "Error: Unable to open file '" << filePath << "'.\n";
        return "";
    }

    std::string sequence;
    std::string line;

    // Check file extension
    std::string extension = filePath.substr(filePath.find_last_of(".") + 1);

    if (extension == "fasta") {
        // Handle FASTA files
        while (std::getline(infile, line)) {
            if (line.empty() || line[0] == '>') {
                continue; // Skip headers and empty lines
            }
            sequence += line;
        }
    } else {
        // Assume plain text file
        while (std::getline(infile, line)) {
            sequence += line;
        }
    }

    infile.close();
    std::cout << "File uploaded successfully.\n";
    return sequence;
}

std::string chooseAlgorithm() {
    std::cout << "Please select a compression algorithm:\n";
    std::cout << "1. Huffman Compression\n";
    std::cout << "2. Run-Length Encoding (RLE)\n";
    std::cout << "Enter your choice: ";

    int choice;
    std::cin >> choice;
    std::cin.ignore(); // Clear the newline character from the buffer

    if (choice == 1) {
        std::cout << "Huffman Compression selected.\n";
        return "huffman";
    } else if (choice == 2) {
        std::cout << "Run-Length Encoding selected.\n";
        return "rle";
    } else {
        std::cerr << "Invalid choice. Please try again.\n";
        return "";
    }
}

void compressFile(const std::string& sequence, const std::string& algorithm) {
    if (sequence.empty()) {
        std::cerr << "Error: No file uploaded.\n";
        return;
    }

    if (algorithm == "huffman") {
        HuffmanGenome huffman;
        huffman.encode(sequence);
        std::string encodedData = huffman.getEncodedSequence();

        // Save the encoded data to a file
        std::string outputFile = "compressed_huffman.txt";
        std::ofstream outfile(outputFile);
        if (!outfile) {
            std::cerr << "Error: Unable to open output file '" << outputFile << "'.\n";
            return;
        }
        outfile << encodedData;
        outfile.close();

        // Save the frequency map
        huffman.saveFrequencyMap("frequency_map.txt");

        double compressionRatio = huffman.getCompressionRatio();
        std::cout << "Huffman Compression Complete.\n";
        std::cout << "Compression Ratio: " << compressionRatio << "\n";
        std::cout << "Compressed data saved to '" << outputFile << "'.\n";

    } else if (algorithm == "rle") {
        RLEGenome rle;
        std::string encodedData = rle.encode(sequence);

        if (encodedData.empty()) {
            std::cerr << "RLE Compression failed.\n";
            return;
        }

        // Save the encoded data to a file
        std::string outputFile = "compressed_rle.txt";
        std::ofstream outfile(outputFile);
        if (!outfile) {
            std::cerr << "Error: Unable to open output file '" << outputFile << "'.\n";
            return;
        }
        outfile << encodedData;
        outfile.close();

        double compressionRatio = rle.getCompressionRatio(sequence, encodedData);
        std::cout << "RLE Compression Complete.\n";
        std::cout << "Compression Ratio: " << compressionRatio << "\n";
        std::cout << "Compressed data saved to '" << outputFile << "'.\n";

    } else {
        std::cerr << "Error: No algorithm selected.\n";
    }
}
