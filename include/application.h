#ifndef APPLICATION_H
#define APPLICATION_H

#include <memory>
#include <string>
#include "Compressor.h"
#include "CompressionMetrics.h"
#include "ArgumentParser.h"
#include "CLIMenu.h"

class Application {
public:
    Application(int argc, char** argv);
    int run();

private:
    int argc_;
    char** argv_;

    std::unique_ptr<Compressor> compressor;

    bool useMenu_;
    bool compressMode_;
    bool decompressMode_;
    bool validateMode_;

    std::string inputFile_;
    std::string outputFile_;
    std::string method_;

    ArgumentParser argParser_;
    CLIMenu menu_;

    Application(const Application&) = delete;
    Application& operator=(const Application&) = delete;

    void handleCompress();
    void handleDecompress();
};

#endif
