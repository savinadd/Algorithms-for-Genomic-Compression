#include "CompressorFactory.h"
#include "HuffmanGenome.h"
#include "HuffmanCompressor.h"
#include "RLEGenome.h"
#include "CombinedCompressor.h"
#include <iostream>

std::unique_ptr<Compressor> CompressorFactory::createCompressor(const std::string& method)
{
    if (method == "huffmangenome")
    {
        return std::make_unique<HuffmanGenome>();
    }
    else if (method == "huffman")
    {
        return std::make_unique<HuffmanCompressor>();
    }
    else if (method == "rle")
    {
        return std::make_unique<RLEGenome>();
    }
    else if (method == "combined")
    {
        return std::make_unique<CombinedCompressor>();
    }
    else
    {
        std::cerr << "Unknown method: " << method << ". Please choose huffmangenome, rle, combined, or huffman.\n";
        exit(1);
    }
}
