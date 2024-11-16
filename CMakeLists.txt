cmake_minimum_required(VERSION 3.10)
project(OptimizedAlgorithmsForGenomicCompression)

# Add Google Test
add_subdirectory(googletest)

# Specify source files
set(SOURCE_FILES src/huffmanGenetic.cpp tests/test_huffmanGenetic.cpp)

# Executable for running tests
add_executable(runTests ${SOURCE_FILES})

# Include directories
target_include_directories(runTests PRIVATE include)

# Link Google Test library
target_link_libraries(runTests gtest gtest_main)

# Enable C++11 standard
set_property(TARGET runTests PROPERTY CXX_STANDARD 11)
