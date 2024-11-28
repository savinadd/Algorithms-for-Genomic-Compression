#ifndef FILEVALIDATOR_H
#define FILEVALIDATOR_H

#include <string>
#include <algorithm>
#include <fstream>
#include <cctype>

class FileValidator {
public:
    static bool hasTxtExtension(const std::string& filename) {
        if (filename.length() < 4) return false; 
        std::string ext = filename.substr(filename.length() - 4);
        std::transform(ext.begin(), ext.end(), ext.begin(), ::tolower);
        return ext == ".txt";
    }

    static bool fileExists(const std::string& filename) {
        std::ifstream infile(filename);
        return infile.good();
    }

    static bool hasValidGenomeData(const std::string& filename) {
        std::ifstream infile(filename, std::ios::binary);
        if (!infile.is_open()) {
            return false;
        }

        char ch;
        while (infile.get(ch)) {
            if (ch == '\n' || ch == '\r') {
                continue; 
            }
            char upperCh = std::toupper(static_cast<unsigned char>(ch));
            if (upperCh != 'A' && upperCh != 'C' && upperCh != 'G' && upperCh != 'T') {
                return false;
            }
        }

        infile.close();
        return true;
    }
};

#endif
