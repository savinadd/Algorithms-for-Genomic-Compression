#include "CLIManager.h"
#include "CompressCommand.h"
#include "DecompressCommand.h"
#include "Logger.h"
#include <iostream>

CLIManager::CLIManager(int argc, char** argv) 
    : app("Optimized Genomic Compression Tool", "1.0.0"), argc_(argc), argv_(argv) {}

void CLIManager::setupCommands() {
    commands.emplace_back(std::make_unique<CompressCommand>(app));
    commands.emplace_back(std::make_unique<DecompressCommand>(app));

    bool version_flag = false;

    app.add_flag("-V,--version", version_flag, "Display the application's version.");


    app.add_option("-c,--config", configPath, "Path to configuration file")
        ->check(CLI::ExistingFile);

    app.callback([&]() {
        if (version_flag) {
            std::cout << "Optimized Genomic Compression Tool, Version 1.0.0\n";
            exit(0); 
        }
    });
}

int CLIManager::run() {
    try {
        setupCommands();
        CLI11_PARSE(app, argc_, argv_);
    }
    catch (const CLI::ParseError &e) {
        return app.exit(e);
    }
    return 0;
}
