#ifndef BASE_COMMAND_H
#define BASE_COMMAND_H

#include <CLI11.hpp>
#include <string>

class BaseCommand {
public:
    virtual void execute() = 0;
    virtual ~BaseCommand() = default; /
};

#endif 
