# CMAKE generated file: DO NOT EDIT!
# Generated by "Unix Makefiles" Generator, CMake Version 3.31

# Delete rule output on recipe failure.
.DELETE_ON_ERROR:

#=============================================================================
# Special targets provided by cmake.

# Disable implicit rules so canonical targets will work.
.SUFFIXES:

# Disable VCS-based implicit rules.
% : %,v

# Disable VCS-based implicit rules.
% : RCS/%

# Disable VCS-based implicit rules.
% : RCS/%,v

# Disable VCS-based implicit rules.
% : SCCS/s.%

# Disable VCS-based implicit rules.
% : s.%

.SUFFIXES: .hpux_make_needs_suffix_list

# Command-line flag to silence nested $(MAKE).
$(VERBOSE)MAKESILENT = -s

#Suppress display of executed commands.
$(VERBOSE).SILENT:

# A target that is always out of date.
cmake_force:
.PHONY : cmake_force

#=============================================================================
# Set environment variables for the build.

# The shell in which to execute make rules.
SHELL = /bin/sh

# The CMake executable.
CMAKE_COMMAND = "C:/Program Files/CMake/bin/cmake.exe"

# The command to remove a file.
RM = "C:/Program Files/CMake/bin/cmake.exe" -E rm -f

# Escaping for special characters.
EQUALS = =

# The top-level source directory on which CMake was run.
CMAKE_SOURCE_DIR = C:/Users/Savina/Desktop/Optimized-Algorithms-for-Genomic-Compression

# The top-level build directory on which CMake was run.
CMAKE_BINARY_DIR = C:/Users/Savina/Desktop/Optimized-Algorithms-for-Genomic-Compression/build

# Include any dependencies generated for this target.
include CMakeFiles/runTests.dir/depend.make
# Include any dependencies generated by the compiler for this target.
include CMakeFiles/runTests.dir/compiler_depend.make

# Include the progress variables for this target.
include CMakeFiles/runTests.dir/progress.make

# Include the compile flags for this target's objects.
include CMakeFiles/runTests.dir/flags.make

CMakeFiles/runTests.dir/codegen:
.PHONY : CMakeFiles/runTests.dir/codegen

CMakeFiles/runTests.dir/src/huffmanGenetic.cpp.obj: CMakeFiles/runTests.dir/flags.make
CMakeFiles/runTests.dir/src/huffmanGenetic.cpp.obj: CMakeFiles/runTests.dir/includes_CXX.rsp
CMakeFiles/runTests.dir/src/huffmanGenetic.cpp.obj: C:/Users/Savina/Desktop/Optimized-Algorithms-for-Genomic-Compression/src/huffmanGenetic.cpp
CMakeFiles/runTests.dir/src/huffmanGenetic.cpp.obj: CMakeFiles/runTests.dir/compiler_depend.ts
	@$(CMAKE_COMMAND) -E cmake_echo_color "--switch=$(COLOR)" --green --progress-dir=C:/Users/Savina/Desktop/Optimized-Algorithms-for-Genomic-Compression/build/CMakeFiles --progress-num=$(CMAKE_PROGRESS_1) "Building CXX object CMakeFiles/runTests.dir/src/huffmanGenetic.cpp.obj"
	C:/Users/Savina/Desktop/w64devkit/bin/g++.exe $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -MD -MT CMakeFiles/runTests.dir/src/huffmanGenetic.cpp.obj -MF CMakeFiles/runTests.dir/src/huffmanGenetic.cpp.obj.d -o CMakeFiles/runTests.dir/src/huffmanGenetic.cpp.obj -c C:/Users/Savina/Desktop/Optimized-Algorithms-for-Genomic-Compression/src/huffmanGenetic.cpp

CMakeFiles/runTests.dir/src/huffmanGenetic.cpp.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color "--switch=$(COLOR)" --green "Preprocessing CXX source to CMakeFiles/runTests.dir/src/huffmanGenetic.cpp.i"
	C:/Users/Savina/Desktop/w64devkit/bin/g++.exe $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -E C:/Users/Savina/Desktop/Optimized-Algorithms-for-Genomic-Compression/src/huffmanGenetic.cpp > CMakeFiles/runTests.dir/src/huffmanGenetic.cpp.i

CMakeFiles/runTests.dir/src/huffmanGenetic.cpp.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color "--switch=$(COLOR)" --green "Compiling CXX source to assembly CMakeFiles/runTests.dir/src/huffmanGenetic.cpp.s"
	C:/Users/Savina/Desktop/w64devkit/bin/g++.exe $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -S C:/Users/Savina/Desktop/Optimized-Algorithms-for-Genomic-Compression/src/huffmanGenetic.cpp -o CMakeFiles/runTests.dir/src/huffmanGenetic.cpp.s

CMakeFiles/runTests.dir/tests/test_huffmanGenetic.cpp.obj: CMakeFiles/runTests.dir/flags.make
CMakeFiles/runTests.dir/tests/test_huffmanGenetic.cpp.obj: CMakeFiles/runTests.dir/includes_CXX.rsp
CMakeFiles/runTests.dir/tests/test_huffmanGenetic.cpp.obj: C:/Users/Savina/Desktop/Optimized-Algorithms-for-Genomic-Compression/tests/test_huffmanGenetic.cpp
CMakeFiles/runTests.dir/tests/test_huffmanGenetic.cpp.obj: CMakeFiles/runTests.dir/compiler_depend.ts
	@$(CMAKE_COMMAND) -E cmake_echo_color "--switch=$(COLOR)" --green --progress-dir=C:/Users/Savina/Desktop/Optimized-Algorithms-for-Genomic-Compression/build/CMakeFiles --progress-num=$(CMAKE_PROGRESS_2) "Building CXX object CMakeFiles/runTests.dir/tests/test_huffmanGenetic.cpp.obj"
	C:/Users/Savina/Desktop/w64devkit/bin/g++.exe $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -MD -MT CMakeFiles/runTests.dir/tests/test_huffmanGenetic.cpp.obj -MF CMakeFiles/runTests.dir/tests/test_huffmanGenetic.cpp.obj.d -o CMakeFiles/runTests.dir/tests/test_huffmanGenetic.cpp.obj -c C:/Users/Savina/Desktop/Optimized-Algorithms-for-Genomic-Compression/tests/test_huffmanGenetic.cpp

CMakeFiles/runTests.dir/tests/test_huffmanGenetic.cpp.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color "--switch=$(COLOR)" --green "Preprocessing CXX source to CMakeFiles/runTests.dir/tests/test_huffmanGenetic.cpp.i"
	C:/Users/Savina/Desktop/w64devkit/bin/g++.exe $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -E C:/Users/Savina/Desktop/Optimized-Algorithms-for-Genomic-Compression/tests/test_huffmanGenetic.cpp > CMakeFiles/runTests.dir/tests/test_huffmanGenetic.cpp.i

CMakeFiles/runTests.dir/tests/test_huffmanGenetic.cpp.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color "--switch=$(COLOR)" --green "Compiling CXX source to assembly CMakeFiles/runTests.dir/tests/test_huffmanGenetic.cpp.s"
	C:/Users/Savina/Desktop/w64devkit/bin/g++.exe $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -S C:/Users/Savina/Desktop/Optimized-Algorithms-for-Genomic-Compression/tests/test_huffmanGenetic.cpp -o CMakeFiles/runTests.dir/tests/test_huffmanGenetic.cpp.s

# Object files for target runTests
runTests_OBJECTS = \
"CMakeFiles/runTests.dir/src/huffmanGenetic.cpp.obj" \
"CMakeFiles/runTests.dir/tests/test_huffmanGenetic.cpp.obj"

# External object files for target runTests
runTests_EXTERNAL_OBJECTS =

runTests.exe: CMakeFiles/runTests.dir/src/huffmanGenetic.cpp.obj
runTests.exe: CMakeFiles/runTests.dir/tests/test_huffmanGenetic.cpp.obj
runTests.exe: CMakeFiles/runTests.dir/build.make
runTests.exe: lib/libgtest.a
runTests.exe: lib/libgtest_main.a
runTests.exe: lib/libgtest.a
runTests.exe: CMakeFiles/runTests.dir/linkLibs.rsp
runTests.exe: CMakeFiles/runTests.dir/objects1.rsp
	@$(CMAKE_COMMAND) -E cmake_echo_color "--switch=$(COLOR)" --green --bold --progress-dir=C:/Users/Savina/Desktop/Optimized-Algorithms-for-Genomic-Compression/build/CMakeFiles --progress-num=$(CMAKE_PROGRESS_3) "Linking CXX executable runTests.exe"
	"C:/Program Files/CMake/bin/cmake.exe" -E rm -f CMakeFiles/runTests.dir/objects.a
	C:/Users/Savina/Desktop/w64devkit/bin/ar.exe qc CMakeFiles/runTests.dir/objects.a @CMakeFiles/runTests.dir/objects1.rsp
	C:/Users/Savina/Desktop/w64devkit/bin/g++.exe -g -Wl,--whole-archive CMakeFiles/runTests.dir/objects.a -Wl,--no-whole-archive -o runTests.exe -Wl,--out-implib,librunTests.dll.a -Wl,--major-image-version,0,--minor-image-version,0 @CMakeFiles/runTests.dir/linkLibs.rsp

# Rule to build all files generated by this target.
CMakeFiles/runTests.dir/build: runTests.exe
.PHONY : CMakeFiles/runTests.dir/build

CMakeFiles/runTests.dir/clean:
	$(CMAKE_COMMAND) -P CMakeFiles/runTests.dir/cmake_clean.cmake
.PHONY : CMakeFiles/runTests.dir/clean

CMakeFiles/runTests.dir/depend:
	$(CMAKE_COMMAND) -E cmake_depends "Unix Makefiles" C:/Users/Savina/Desktop/Optimized-Algorithms-for-Genomic-Compression C:/Users/Savina/Desktop/Optimized-Algorithms-for-Genomic-Compression C:/Users/Savina/Desktop/Optimized-Algorithms-for-Genomic-Compression/build C:/Users/Savina/Desktop/Optimized-Algorithms-for-Genomic-Compression/build C:/Users/Savina/Desktop/Optimized-Algorithms-for-Genomic-Compression/build/CMakeFiles/runTests.dir/DependInfo.cmake "--color=$(COLOR)"
.PHONY : CMakeFiles/runTests.dir/depend

