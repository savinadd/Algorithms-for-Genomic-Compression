#!/bin/bash

echo "Starting GenomeCompressor setup..."

echo "Checking for required tools..."

# Check for a C++ compiler
if ! command -v g++ &> /dev/null && ! command -v clang++ &> /dev/null && ! command -v cl &> /dev/null; then
    echo "Error: No C++ compiler found. Please install g++, clang++, or MSVC."
    exit 1
fi

# Check for CMake
if ! command -v cmake &> /dev/null; then
    echo "Error: CMake is not installed. Please install CMake (3.10 or newer) and try again."
    exit 1
fi

# Check for Make
if ! command -v make &> /dev/null && ! command -v mingw32-make &> /dev/null && ! command -v ninja &> /dev/null; then
    echo "Error: No build tool found (make, mingw32-make, or ninja). Please install one and try again."
    exit 1
fi

echo "All required tools are installed!"

# Run CMake to configure the project
echo "Configuring the project with CMake..."
cmake -S . -B build -G "Unix Makefiles"
if [ $? -ne 0 ]; then
    echo "Error: CMake configuration failed."
    exit 1
fi

#  Build the project
echo "Building the project..."
cmake --build build
if [ $? -ne 0 ]; then
    echo "Error: Build failed."
    exit 1
fi

# Verify the 'compressor' executable
echo "Verifying the 'compressor' executable..."
if [ ! -f "./compressor" ]; then
    echo "Error: The 'compressor' executable was not created."
    exit 1
fi
echo "Verifying the 'tests' executable..."
if [ ! -f "./tests" ]; then
    echo "Error: The 'tests' executable was not created."
    exit 1
fi


#  Optional - Add 'compressor' to the PATH
echo "Adding the 'compressor' executable to your PATH..."
INSTALL_DIR="/usr/local/bin"

if [ -w "$INSTALL_DIR" ]; then
    cp ./compressor "$INSTALL_DIR/"
    echo "The 'compressor' executable has been added to '$INSTALL_DIR'."
    echo "You can now run 'compressor' from anywhere!"
else
    echo "You do not have write permission for '$INSTALL_DIR'."
    echo "To make 'compressor' globally accessible, run the following command manually:"
    echo "    sudo cp ./compressor $INSTALL_DIR/"
fi

#  Run tests
echo "Running the tests to verify the setup..."
./tests
if [ $? -ne 0 ]; then
    echo "Error: Tests failed."
    exit 1
fi

#  Completion message
echo "Setup complete! The project is ready to use."
echo "To run the main program, use './compressor' from the project root or 'compressor' if added to PATH."
