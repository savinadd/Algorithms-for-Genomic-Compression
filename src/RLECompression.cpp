#include "../include/RLEGenome.h"
#include <sstream>
#include <cctype> // For isdigit

// Encode the genomic sequence using Run-Length Encoding
std::string RLEGenome::encode(const std::string& sequence) {
    if (sequence.empty()) return "";

    std::ostringstream encoded;
    char currentChar = sequence[0];
    int count = 1;

    for (size_t i = 1; i < sequence.length(); ++i) {
        // Validate input character
        if (currentChar != 'A' && currentChar != 'C' && currentChar != 'G' && currentChar != 'T') {
            std::cerr << "Error: Invalid character '" << currentChar << "' in sequence." << std::endl;
            return "";
        }

        if (sequence[i] == currentChar) {
            count++;
        } else {
            encoded << currentChar << count;
            currentChar = sequence[i];
            count = 1;
        }
    }
    // Add the last character and its count
    encoded << currentChar << count;

    return encoded.str();
}

// Decode the encoded sequence back to its original form
std::string RLEGenome::decode(const std::string& encodedSequence) {
    if (encodedSequence.empty()) return "";

    std::ostringstream decoded;
    size_t i = 0;

    while (i < encodedSequence.length()) {
        char currentChar = encodedSequence[i];

        // Validate character
        if (currentChar != 'A' && currentChar != 'C' && currentChar != 'G' && currentChar != 'T') {
            std::cerr << "Error: Invalid character '" << currentChar << "' in encoded sequence." << std::endl;
            return "";
        }
        i++;
        if (i >= encodedSequence.length() || !std::isdigit(encodedSequence[i])) {
            std::cerr << "Error: Expected digit after character '" << currentChar << "'." << std::endl;
            return "";
        }

        int count = 0;

        // Convert the numeric characters to an integer (handles multi-digit counts)
        while (i < encodedSequence.length() && std::isdigit(encodedSequence[i])) {
            count = count * 10 + (encodedSequence[i] - '0');
            i++;
        }

        if (count <= 0) {
            std::cerr << "Error: Invalid count '" << count << "' for character '" << currentChar << "'." << std::endl;
            return "";
        }

        decoded << std::string(count, currentChar);
    }

    return decoded.str();
}

// Calculate the compression ratio
double RLEGenome::getCompressionRatio(const std::string& originalSequence, const std::string& encodedSequence) {
    if (originalSequence.empty() || encodedSequence.empty()) return 0.0;

    int originalSize = originalSequence.length() * 2; // 2 bits per base
    int encodedSize = 0;

    // Calculate the size of the encoded sequence in bits
    for (size_t i = 0; i < encodedSequence.length(); ++i) {
        // Each character is a nucleotide (2 bits)
        if (encodedSequence[i] == 'A' || encodedSequence[i] == 'C' || encodedSequence[i] == 'G' || encodedSequence[i] == 'T') {
            encodedSize += 2;
        }
        // Each digit is a count; we need to calculate its bit size
        else if (std::isdigit(encodedSequence[i])) {
            // Assuming counts can be stored efficiently, e.g., using variable-length encoding
            // For simplicity, let's assume each digit is represented in 4 bits (since decimal digits 0-9 can be represented in 4 bits)
            encodedSize += 4;
        }
    }

    if (encodedSize == 0) return 0.0;
    return static_cast<double>(originalSize) / encodedSize;
}
