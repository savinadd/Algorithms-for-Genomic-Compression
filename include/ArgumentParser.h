#ifndef ARGUMENTPARSER_H
#define ARGUMENTPARSER_H

#include <string>
#include <CLI11.hpp>

class ArgumentParser {
public:
    ArgumentParser(int argc, char** argv);

    void parse();

    // Getters for parsed values
    bool isCompressMode() const;
    bool isDecompressMode() const;
    bool isValidateMode() const;
    bool isUseMenu() const;
    std::string getInputFile() const;
    std::string getOutputFile() const;
    std::string getMethod() const;

private:
    int argc_;
    char** argv_;

    bool compressMode_;
    bool decompressMode_;
    bool validateMode_;
    bool useMenu_;

    std::string inputFile_;
    std::string outputFile_;
    std::string method_;

    ArgumentParser(const ArgumentParser&) = delete;
    ArgumentParser& operator=(const ArgumentParser&) = delete;
};

#endif 
