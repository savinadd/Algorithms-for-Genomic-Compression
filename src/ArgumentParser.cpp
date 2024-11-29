#include "ArgumentParser.h"
#include <iostream>

ArgumentParser::ArgumentParser(int argc, char** argv)
    : argc_(argc), argv_(argv), compressMode_(false), decompressMode_(false),
      validateMode_(false), useMenu_(false), inputFile_(""), outputFile_(""), method_("") {}

void ArgumentParser::parse() {
    CLI::App app{"Genomic Data Compressor: Compresses or decompresses data using Huffman coding, Run Length Encoding, and other variations"};

    app.get_formatter()->column_width(70);

    auto compress = app.add_flag("-c,--compress", compressMode_, "Compression mode: Compress the input file.");
    auto decompress = app.add_flag("-d,--decompress", decompressMode_, "Decompression mode: Decompress the input file.");
    auto validate = app.add_flag("--validate", validateMode_, "Validation mode, used with Compression mode: Automatically validate compression integrity.");
    auto menu = app.add_flag("--menu", useMenu_, "Display a welcome menu with usage instructions");

    // Define mutual exclusivity: --menu cannot be used with -c or -d
    menu->excludes(compress);
    menu->excludes(decompress);

    // Define CLI options without required constraints
    app.add_option("-i,--input", inputFile_, "Input file for compression or decompression")
        ->check(CLI::ExistingFile);

    app.add_option("-o,--output", outputFile_, "Output file for the compressed or decompressed data");

    app.add_option("-m,--method", method_, "Compression method: huffmangenome, rle, combined, huffman")
        ->check(CLI::IsMember({"huffmangenome", "rle", "combined", "huffman"}));

    app.footer("Examples:\n"
               "  Compress using Huffman Genome Compressor:\n"
               "    compressor -c -i genome_data.txt -o genomeDataTest.bin -m huffmangenome\n\n"
               "  Decompress using Huffman Genome Compressor:\n"
               "    compressor -d -i genomeDataTest.bin -o decoded_genomeDataTest.txt -m huffmangenome\n\n"
               "  Compress using Run-Length Encoding (RLE):\n"
               "    compressor -c -i genome_data.txt -o genomeDataTest.rle -m rle\n\n"
               "  Compress using Combined RLE + Huffman:\n"
               "    compressor -c -i genome_data.txt -o genomeDataTest.combined -m combined\n\n"
               "  Display the menu:\n"
               "    compressor --menu\n\n"
               "  View the help menu:\n"
               "    compressor --help\n");

    try {
        app.parse(argc_, argv_);
    }
    catch (const CLI::ParseError &e) {
        exit(app.exit(e));
    }

    if (useMenu_) {
        // When --menu is used, no need for other options
        return;
    }

    if (compressMode_ || decompressMode_) {
        if (inputFile_.empty()) {
            std::cerr << "Error: --input is required when using -c or -d.\n";
            std::cerr << "Run `compressor --help` for more information.\n";
            exit(1);
        }

        if (outputFile_.empty()) {
            std::cerr << "Error: --output is required when using -c or -d.\n";
            std::cerr << "Run `compressor --help` for more information.\n";
            exit(1);
        }

        if (method_.empty()) {
            std::cerr << "Error: --method is required when using -c or -d.\n";
            std::cerr << "Run `compressor --help` for more information.\n";
            exit(1);
        }
    }
    else {
        std::cerr << "Error: Must specify either compression (-c) or decompression (-d) mode.\n";
        std::cerr << "Run `compressor --menu` for usage instructions.\n";
        exit(1);
    }
}

bool ArgumentParser::isCompressMode() const { return compressMode_; }
bool ArgumentParser::isDecompressMode() const { return decompressMode_; }
bool ArgumentParser::isValidateMode() const { return validateMode_; }
bool ArgumentParser::isUseMenu() const { return useMenu_; }
std::string ArgumentParser::getInputFile() const { return inputFile_; }
std::string ArgumentParser::getOutputFile() const { return outputFile_; }
std::string ArgumentParser::getMethod() const { return method_; }
