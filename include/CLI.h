#ifndef CLI_H
#define CLI_H

class GenomeCompressorCLI {
public:
    // Constructor
    GenomeCompressorCLI(int argc, char** argv);

    // Run the CLI application
    int run();

private:
    int argc_;
    char** argv_;

    // Additional private methods and members can be declared here
};

#endif // CLI_H
