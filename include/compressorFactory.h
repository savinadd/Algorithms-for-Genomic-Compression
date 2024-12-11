#ifndef COMPRESSORFACTORY_H
#define COMPRESSORFACTORY_H

#include <memory>
#include <string>
#include "Compressor.h"

class CompressorFactory {
public:
    static std::unique_ptr<Compressor> createCompressor(const std::string& method);
};

#endif
