# CMAKE generated file: DO NOT EDIT!
# Generated by "Unix Makefiles" Generator, CMake Version 3.30

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
CMAKE_COMMAND = /opt/homebrew/Cellar/cmake/3.30.3/bin/cmake

# The command to remove a file.
RM = /opt/homebrew/Cellar/cmake/3.30.3/bin/cmake -E rm -f

# Escaping for special characters.
EQUALS = =

# The top-level source directory on which CMake was run.
CMAKE_SOURCE_DIR = /Users/catkin/Study/compilers

# The top-level build directory on which CMake was run.
CMAKE_BINARY_DIR = /Users/catkin/Study/compilers/build

# Include any dependencies generated for this target.
include test/CMakeFiles/test_scanner.dir/depend.make
# Include any dependencies generated by the compiler for this target.
include test/CMakeFiles/test_scanner.dir/compiler_depend.make

# Include the progress variables for this target.
include test/CMakeFiles/test_scanner.dir/progress.make

# Include the compile flags for this target's objects.
include test/CMakeFiles/test_scanner.dir/flags.make

test/CMakeFiles/test_scanner.dir/test_scanner.cc.o: test/CMakeFiles/test_scanner.dir/flags.make
test/CMakeFiles/test_scanner.dir/test_scanner.cc.o: /Users/catkin/Study/compilers/test/test_scanner.cc
test/CMakeFiles/test_scanner.dir/test_scanner.cc.o: test/CMakeFiles/test_scanner.dir/compiler_depend.ts
	@$(CMAKE_COMMAND) -E cmake_echo_color "--switch=$(COLOR)" --green --progress-dir=/Users/catkin/Study/compilers/build/CMakeFiles --progress-num=$(CMAKE_PROGRESS_1) "Building CXX object test/CMakeFiles/test_scanner.dir/test_scanner.cc.o"
	cd /Users/catkin/Study/compilers/build/test && /Applications/Xcode.app/Contents/Developer/Toolchains/XcodeDefault.xctoolchain/usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -MD -MT test/CMakeFiles/test_scanner.dir/test_scanner.cc.o -MF CMakeFiles/test_scanner.dir/test_scanner.cc.o.d -o CMakeFiles/test_scanner.dir/test_scanner.cc.o -c /Users/catkin/Study/compilers/test/test_scanner.cc

test/CMakeFiles/test_scanner.dir/test_scanner.cc.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color "--switch=$(COLOR)" --green "Preprocessing CXX source to CMakeFiles/test_scanner.dir/test_scanner.cc.i"
	cd /Users/catkin/Study/compilers/build/test && /Applications/Xcode.app/Contents/Developer/Toolchains/XcodeDefault.xctoolchain/usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -E /Users/catkin/Study/compilers/test/test_scanner.cc > CMakeFiles/test_scanner.dir/test_scanner.cc.i

test/CMakeFiles/test_scanner.dir/test_scanner.cc.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color "--switch=$(COLOR)" --green "Compiling CXX source to assembly CMakeFiles/test_scanner.dir/test_scanner.cc.s"
	cd /Users/catkin/Study/compilers/build/test && /Applications/Xcode.app/Contents/Developer/Toolchains/XcodeDefault.xctoolchain/usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -S /Users/catkin/Study/compilers/test/test_scanner.cc -o CMakeFiles/test_scanner.dir/test_scanner.cc.s

# Object files for target test_scanner
test_scanner_OBJECTS = \
"CMakeFiles/test_scanner.dir/test_scanner.cc.o"

# External object files for target test_scanner
test_scanner_EXTERNAL_OBJECTS =

test/test_scanner: test/CMakeFiles/test_scanner.dir/test_scanner.cc.o
test/test_scanner: test/CMakeFiles/test_scanner.dir/build.make
test/test_scanner: lib/liblox.a
test/test_scanner: lib/libgtest.a
test/test_scanner: test/CMakeFiles/test_scanner.dir/link.txt
	@$(CMAKE_COMMAND) -E cmake_echo_color "--switch=$(COLOR)" --green --bold --progress-dir=/Users/catkin/Study/compilers/build/CMakeFiles --progress-num=$(CMAKE_PROGRESS_2) "Linking CXX executable test_scanner"
	cd /Users/catkin/Study/compilers/build/test && $(CMAKE_COMMAND) -E cmake_link_script CMakeFiles/test_scanner.dir/link.txt --verbose=$(VERBOSE)
	cd /Users/catkin/Study/compilers/build/test && /opt/homebrew/Cellar/cmake/3.30.3/bin/cmake -D TEST_TARGET=test_scanner -D TEST_EXECUTABLE=/Users/catkin/Study/compilers/build/test/test_scanner -D TEST_EXECUTOR= -D TEST_WORKING_DIR=/Users/catkin/Study/compilers/build/test -D "TEST_EXTRA_ARGS=--gtest_output=xml:/Users/catkin/Study/compilers/build/test/test_scanner.xml;--gtest_catch_exceptions=0" -D "TEST_PROPERTIES=TIMEOUT;120" -D TEST_PREFIX= -D TEST_SUFFIX= -D TEST_FILTER= -D NO_PRETTY_TYPES=FALSE -D NO_PRETTY_VALUES=FALSE -D TEST_LIST=test_scanner_TESTS -D CTEST_FILE=/Users/catkin/Study/compilers/build/test/test_scanner[1]_tests.cmake -D TEST_DISCOVERY_TIMEOUT=120 -D TEST_XML_OUTPUT_DIR= -P /opt/homebrew/Cellar/cmake/3.30.3/share/cmake/Modules/GoogleTestAddTests.cmake

# Rule to build all files generated by this target.
test/CMakeFiles/test_scanner.dir/build: test/test_scanner
.PHONY : test/CMakeFiles/test_scanner.dir/build

test/CMakeFiles/test_scanner.dir/clean:
	cd /Users/catkin/Study/compilers/build/test && $(CMAKE_COMMAND) -P CMakeFiles/test_scanner.dir/cmake_clean.cmake
.PHONY : test/CMakeFiles/test_scanner.dir/clean

test/CMakeFiles/test_scanner.dir/depend:
	cd /Users/catkin/Study/compilers/build && $(CMAKE_COMMAND) -E cmake_depends "Unix Makefiles" /Users/catkin/Study/compilers /Users/catkin/Study/compilers/test /Users/catkin/Study/compilers/build /Users/catkin/Study/compilers/build/test /Users/catkin/Study/compilers/build/test/CMakeFiles/test_scanner.dir/DependInfo.cmake "--color=$(COLOR)"
.PHONY : test/CMakeFiles/test_scanner.dir/depend
