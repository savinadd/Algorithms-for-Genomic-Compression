#include "CLIMenu.h"
#include "rang.hpp"
#include <iostream>

using namespace rang;

void CLIMenu::displayMenu() const
{
    std::cout << "*************************************************************************************\n";
    std::cout << style::bold << fg::cyan << "                    Welcome to the Genomic Data Compressor\n"
              << style::reset;
    std::cout << "*************************************************************************************\n";
    std::cout << style::bold << fg::cyan << "This tool helps you compress and decompress genomic data using various algorithms.\n\n"
              << style::reset;
    std::cout << style::bold << fg::cyan << "Usage Examples:\n"
              << style::reset;
    std::cout << "1. Compress a file using the Huffman Genome Compressor:\n";
    std::cout << "   compressor -c -i path/to/input/file.txt -o outputfilename.huffg -m huffmangenome\n\n";
    std::cout << "2. Decompress a file using the Huffman Genome Compressor:\n";
    std::cout << "   compressor -d -i genomeDataTest.bin -o decoded_genomeDataTest.txt -m huffmangenome\n\n";
    std::cout << "3. Compress a file using Run-Length Encoding (RLE):\n";
    std::cout << "   compressor -c -i path/to/input/file.txt -o outputfilename.rle -m rle\n\n";
    std::cout << "4. Compress a file using the combined RLE + Huffman method:\n";
    std::cout << "   compressor -c -i path/to/input/file.txt -o outputfilename.combined -m combined\n\n";
    std::cout << "5. View this menu again:\n";
    std::cout << "   compressor --menu\n\n";
    std::cout << "6. View the help menu:\n";
    std::cout << "   compressor --help\n\n";
    std::cout << "Note:\n";
    std::cout << "- The input file (-i) must exist and have a .txt extension for compression.\n";
    std::cout << "- The output file (-o) will be created if it doesn't exist.\n";
    std::cout << "- The method (-m) must be one of: huffmangenome, rle, combined, huffman.\n";
    std::cout << "- For decompression, ensure that the frequency map file (inputFile.freq) exists.\n";
    std::cout << "=============================================\n";
}
