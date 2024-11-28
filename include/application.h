#ifndef APPLICATION_H
#define APPLICATION_H

#include <memory>
#include <string>
#include "Compressor.h"

class Application {
public:
    Application(int argc, char** argv);

    int run();

private:
    void parseArguments();

    void displayMenu() const;
    std::string inputFile;   
    std::string outputFile; 
    std::string method;  
    bool useMenu;   


    std::unique_ptr<Compressor> compressor;


    int argc_;
    char** argv_;
};

#endif 