#ifndef CLI_MANAGER_H
#define CLI_MANAGER_H

#include <CLI11.hpp>
#include "BaseCommand.h"
#include <memory>
#include <vector>
#include <string>

class CLIManager {
public:
    CLIManager(int argc, char** argv);
    void setupCommands();

    int run();

private:
    CLI::App app; 
    int argc_;
    char** argv_;
    std::vector<std::unique_ptr<BaseCommand>> commands; 
    std::string configPath;
};

#endif 
