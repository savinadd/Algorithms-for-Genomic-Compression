#!/bin/bash

# Exit immediately if a command exits with a non-zero status
set -e

#  ANSI escape codes
RED='\033[1;31m'
GREEN='\033[1;32m'
YELLOW='\033[1;33m'
BLUE='\033[1;34m'
MAGENTA='\033[1;35m'
CYAN='\033[1;36m'
NC='\033[0m' # No Color

print_styled() {
    local color="$1"
    local message="$2"
    echo -e "${color}${message}${NC}"
}

print_styled "$YELLOW" "Installation started..."

print_styled "$YELLOW" "Starting GenomeCompressor setup..."

print_styled "$YELLOW" "Checking for required tools..."

# Check for a compiler
if ! command -v g++ &> /dev/null && ! command -v clang++ &> /dev/null && ! command -v cl &> /dev/null; then
    print_styled "$RED" "No C++ compiler found. Please install g++, clang++, or MSVC."
    exit 1
fi

# Check for CMake
if ! command -v cmake &> /dev/null; then
    print_styled "$RED" "Error: CMake is not installed. Please install CMake (3.10 or newer) and try again."
    exit 1
fi

# Check for Make or Ninja
if ! command -v make &> /dev/null && ! command -v mingw32-make &> /dev/null && ! command -v ninja &> /dev/null; then
    print_styled "$RED" "Error: No build tool found (make, mingw32-make, or ninja). Please install one and try again."
    exit 1
fi

print_styled "$GREEN" "All required tools are installed!"

# Detect the operating system
OS_NAME=$(uname -s)

# Set executable extensions 
if [[ "$OS_NAME" == "MINGW"* || "$OS_NAME" == "MSYS"* || "$OS_NAME" == "CYGWIN"* ]]; then
    EXEC_EXT=".exe"
else
    EXEC_EXT=""
fi

# Run CMake 
print_styled "$YELLOW" "Configuring the project with CMake..."
cmake -S . -B build -G "Unix Makefiles" -Wno-dev
if [ $? -ne 0 ]; then
    print_styled "$RED" "Error: CMake configuration failed."
    exit 1
fi


# Build 
print_styled "$YELLOW" "Building the project..."
cmake --build build
if [ $? -ne 0 ]; then
    print_styled "$RED" "Error: Build failed."
    exit 1
fi

# compressor executable
print_styled "$YELLOW" "Verifying the 'compressor' executable..."
if [ ! -f "./compressor${EXEC_EXT}" ]; then
    print_styled "$RED" "Error: The 'compressor' executable was not created."
    exit 1
fi

# tests executable
print_styled "$YELLOW" "Verifying the 'tests' executable..."
if [ ! -f "./tests${EXEC_EXT}" ]; then
    print_styled "$RED" "Error: The 'tests' executable was not created."
    exit 1
fi

#  Add 'compressor' to the PATH
print_styled "$YELLOW" "Adding the 'compressor' executable to your PATH..."
INSTALL_DIR="/usr/local/bin"

if [ -w "$INSTALL_DIR" ]; then
    cp "./compressor${EXEC_EXT}" "$INSTALL_DIR/"
    print_styled "$GREEN" "The 'compressor' executable has been added to '$INSTALL_DIR'."
    print_styled "$GREEN" "You can now run 'compressor' from anywhere!"
else
    print_styled "$RED" "You do not have write permission for '$INSTALL_DIR'."
    print_styled "$RED" "To make 'compressor' globally accessible, run the following command manually:"
    echo -e "${RED}    sudo cp ./compressor${EXEC_EXT} $INSTALL_DIR/${NC}"
fi

# Run tests
print_styled "$YELLOW" "Running the tests to verify the setup..."
./tests${EXEC_EXT}
if [ $? -ne 0 ]; then
    print_styled "$RED" "Error: Tests failed."
    exit 1
fi

print_styled "$GREEN" "Setup complete! The project is ready to use."
print_styled "$MAGENTA" "To run the main program, use './compressor${EXEC_EXT}' from the project root or 'compressor' if added to PATH."
