#include "CLI.h"

int main(int argc, char** argv) {
    GenomeCompressorCLI cli(argc, argv);
    return cli.run();
}
