# Minimum required version of CMake
cmake_minimum_required(VERSION 3.10)

# Project name and C++ standard
project(GenomeCompressor LANGUAGES CXX)
set(CMAKE_CXX_STANDARD 17)

# Include directory for header-only libraries
include_directories(include)

# Automatically find all source files in src/ directory
file(GLOB COMPRESSOR_SOURCES src/*.cpp)

# Exclude main.cpp from the test executable
list(REMOVE_ITEM COMPRESSOR_SOURCES ${CMAKE_SOURCE_DIR}/src/main.cpp)

# Set the output directory for executables to the root directory
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_SOURCE_DIR})

# Add the main application executable
add_executable(compressor src/main.cpp ${COMPRESSOR_SOURCES})

# Enable testing
enable_testing()

# Add GoogleTest
include(FetchContent)
FetchContent_Declare(
  googletest
  URL https://github.com/google/googletest/archive/release-1.12.1.zip
)

# Prevent GoogleTest from overriding compiler/linker options
set(gtest_force_shared_crt ON CACHE BOOL "" FORCE)

# Fetch and make GoogleTest available
FetchContent_MakeAvailable(googletest)

# Add test executable
file(GLOB TEST_SOURCES tests/*.cpp)
add_executable(tests ${TEST_SOURCES} ${COMPRESSOR_SOURCES})

# Set the output directory for the tests binary to the root folder
set_target_properties(tests PROPERTIES RUNTIME_OUTPUT_DIRECTORY ${CMAKE_SOURCE_DIR})

# Link test executable to GoogleTest
target_link_libraries(tests gtest gtest_main)

# Register tests with CTest
add_test(NAME RunTests COMMAND tests)
