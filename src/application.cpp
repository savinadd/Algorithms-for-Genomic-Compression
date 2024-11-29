#include "application.h"
#include "CompressorFactory.h"
#include "CompressionException.h"
#include "Logger.h"
#include "FileValidator.h"
#include <iostream>
#include <filesystem>

namespace fs = std::filesystem;

Application::Application(int argc, char **argv)
    : argc_(argc), argv_(argv), argParser_(argc, argv),
      useMenu_(false), compressMode_(false), decompressMode_(false),
      validateMode_(false), inputFile_(""), outputFile_(""), method_(""),
      compressor(nullptr)
{
}

int Application::run()
{
    argParser_.parse();

    // Retrieve parsed values
    useMenu_ = argParser_.isUseMenu();
    compressMode_ = argParser_.isCompressMode();
    decompressMode_ = argParser_.isDecompressMode();
    validateMode_ = argParser_.isValidateMode();
    inputFile_ = argParser_.getInputFile();
    outputFile_ = argParser_.getOutputFile();
    method_ = argParser_.getMethod();

    if (useMenu_)
    {
        menu_.displayMenu();
        return 0;
    }

    try
    {
        if (compressMode_)
        {
            handleCompress();
        }
        else if (decompressMode_)
        {
            handleDecompress();
        }
        else
        {
            std::cerr << "Error: Invalid mode.\n";
            return 1;
        }
    }
    catch (const CompressionException &ce)
    {
        Logger::getInstance().log(std::string("CompressionException: ") + ce.what());
        std::cerr << ce.what() << "\n";
        return 1;
    }
    catch (const std::exception &e)
    {
        Logger::getInstance().log(std::string("Exception: ") + e.what());
        std::cerr << "Error: " << e.what() << "\n";
        return 1;
    }

    return 0;
}

void Application::handleCompress()
{
    // Initialize the appropriate compressor using the factory
    compressor = CompressorFactory::createCompressor(method_);

    compressor->encodeFromFile(inputFile_, outputFile_);

    compressor->getMetrics().printMetrics();

    if (validateMode_)
    {
        Logger::getInstance().log("Starting Huffman decoding...");

        // Define a temporary decoded file path
        fs::path outputPath(outputFile_);
        fs::path parentPath = outputPath.parent_path();
        fs::path filename = outputPath.stem(); // Remove extension
        fs::path tempDecodedFilePath = parentPath / (filename.string() + "_decoded" + outputPath.extension().string());

        std::string tempDecodedFile = tempDecodedFilePath.string();

        compressor->decodeFromFile(outputFile_, tempDecodedFile);
        Logger::getInstance().log("Decoding completed. Decoded file: " + tempDecodedFile);

        bool isValid = compressor->validateDecodedFile(inputFile_, tempDecodedFile);
        if (isValid)
        {
            Logger::getInstance().log("Validation successful: Decoded file matches the original.");
        }
        else
        {
            Logger::getInstance().log("Validation failed: Decoded file does not match the original.");
        }

        // try {
        //     fs::remove(tempDecodedFilePath);
        //     Logger::getInstance().log("Temporary decoded file removed: " + tempDecodedFile);
        // }
        // catch (const fs::filesystem_error& e) {
        //     std::cerr << "Error removing temporary file: " << e.what() << "\n";
        //     Logger::getInstance().log(std::string("Filesystem error while removing temp file: ") + e.what());
        // }

        compressor->getMetrics().printMetrics();
    }
}

void Application::handleDecompress()
{
    if (!FileValidator::fileExists(inputFile_))
    {
        std::cerr << "Error: Input file does not exist.\n";
        return;
    }

    compressor = CompressorFactory::createCompressor(method_);

    compressor->decodeFromFile(inputFile_, outputFile_);

    // compressor->getMetrics().printMetrics();
}