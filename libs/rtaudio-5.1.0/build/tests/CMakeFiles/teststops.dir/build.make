# CMAKE generated file: DO NOT EDIT!
# Generated by "Unix Makefiles" Generator, CMake Version 3.10

# Delete rule output on recipe failure.
.DELETE_ON_ERROR:


#=============================================================================
# Special targets provided by cmake.

# Disable implicit rules so canonical targets will work.
.SUFFIXES:


# Remove some rules from gmake that .SUFFIXES does not remove.
SUFFIXES =

.SUFFIXES: .hpux_make_needs_suffix_list


# Suppress display of executed commands.
$(VERBOSE).SILENT:


# A target that is always out of date.
cmake_force:

.PHONY : cmake_force

#=============================================================================
# Set environment variables for the build.

# The shell in which to execute make rules.
SHELL = /bin/sh

# The CMake executable.
CMAKE_COMMAND = /usr/bin/cmake

# The command to remove a file.
RM = /usr/bin/cmake -E remove -f

# Escaping for special characters.
EQUALS = =

# The top-level source directory on which CMake was run.
CMAKE_SOURCE_DIR = /home/paul/Documents/git_projects/tstr/libs/rtaudio-5.1.0

# The top-level build directory on which CMake was run.
CMAKE_BINARY_DIR = /home/paul/Documents/git_projects/tstr/libs/rtaudio-5.1.0/build

# Include any dependencies generated for this target.
include tests/CMakeFiles/teststops.dir/depend.make

# Include the progress variables for this target.
include tests/CMakeFiles/teststops.dir/progress.make

# Include the compile flags for this target's objects.
include tests/CMakeFiles/teststops.dir/flags.make

tests/CMakeFiles/teststops.dir/teststops.cpp.o: tests/CMakeFiles/teststops.dir/flags.make
tests/CMakeFiles/teststops.dir/teststops.cpp.o: ../tests/teststops.cpp
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --progress-dir=/home/paul/Documents/git_projects/tstr/libs/rtaudio-5.1.0/build/CMakeFiles --progress-num=$(CMAKE_PROGRESS_1) "Building CXX object tests/CMakeFiles/teststops.dir/teststops.cpp.o"
	cd /home/paul/Documents/git_projects/tstr/libs/rtaudio-5.1.0/build/tests && /usr/bin/c++  $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -o CMakeFiles/teststops.dir/teststops.cpp.o -c /home/paul/Documents/git_projects/tstr/libs/rtaudio-5.1.0/tests/teststops.cpp

tests/CMakeFiles/teststops.dir/teststops.cpp.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Preprocessing CXX source to CMakeFiles/teststops.dir/teststops.cpp.i"
	cd /home/paul/Documents/git_projects/tstr/libs/rtaudio-5.1.0/build/tests && /usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -E /home/paul/Documents/git_projects/tstr/libs/rtaudio-5.1.0/tests/teststops.cpp > CMakeFiles/teststops.dir/teststops.cpp.i

tests/CMakeFiles/teststops.dir/teststops.cpp.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Compiling CXX source to assembly CMakeFiles/teststops.dir/teststops.cpp.s"
	cd /home/paul/Documents/git_projects/tstr/libs/rtaudio-5.1.0/build/tests && /usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -S /home/paul/Documents/git_projects/tstr/libs/rtaudio-5.1.0/tests/teststops.cpp -o CMakeFiles/teststops.dir/teststops.cpp.s

tests/CMakeFiles/teststops.dir/teststops.cpp.o.requires:

.PHONY : tests/CMakeFiles/teststops.dir/teststops.cpp.o.requires

tests/CMakeFiles/teststops.dir/teststops.cpp.o.provides: tests/CMakeFiles/teststops.dir/teststops.cpp.o.requires
	$(MAKE) -f tests/CMakeFiles/teststops.dir/build.make tests/CMakeFiles/teststops.dir/teststops.cpp.o.provides.build
.PHONY : tests/CMakeFiles/teststops.dir/teststops.cpp.o.provides

tests/CMakeFiles/teststops.dir/teststops.cpp.o.provides.build: tests/CMakeFiles/teststops.dir/teststops.cpp.o


# Object files for target teststops
teststops_OBJECTS = \
"CMakeFiles/teststops.dir/teststops.cpp.o"

# External object files for target teststops
teststops_EXTERNAL_OBJECTS =

tests/teststops: tests/CMakeFiles/teststops.dir/teststops.cpp.o
tests/teststops: tests/CMakeFiles/teststops.dir/build.make
tests/teststops: librtaudio.so.6.1.0
tests/teststops: /usr/lib/x86_64-linux-gnu/libasound.so
tests/teststops: tests/CMakeFiles/teststops.dir/link.txt
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --bold --progress-dir=/home/paul/Documents/git_projects/tstr/libs/rtaudio-5.1.0/build/CMakeFiles --progress-num=$(CMAKE_PROGRESS_2) "Linking CXX executable teststops"
	cd /home/paul/Documents/git_projects/tstr/libs/rtaudio-5.1.0/build/tests && $(CMAKE_COMMAND) -E cmake_link_script CMakeFiles/teststops.dir/link.txt --verbose=$(VERBOSE)

# Rule to build all files generated by this target.
tests/CMakeFiles/teststops.dir/build: tests/teststops

.PHONY : tests/CMakeFiles/teststops.dir/build

tests/CMakeFiles/teststops.dir/requires: tests/CMakeFiles/teststops.dir/teststops.cpp.o.requires

.PHONY : tests/CMakeFiles/teststops.dir/requires

tests/CMakeFiles/teststops.dir/clean:
	cd /home/paul/Documents/git_projects/tstr/libs/rtaudio-5.1.0/build/tests && $(CMAKE_COMMAND) -P CMakeFiles/teststops.dir/cmake_clean.cmake
.PHONY : tests/CMakeFiles/teststops.dir/clean

tests/CMakeFiles/teststops.dir/depend:
	cd /home/paul/Documents/git_projects/tstr/libs/rtaudio-5.1.0/build && $(CMAKE_COMMAND) -E cmake_depends "Unix Makefiles" /home/paul/Documents/git_projects/tstr/libs/rtaudio-5.1.0 /home/paul/Documents/git_projects/tstr/libs/rtaudio-5.1.0/tests /home/paul/Documents/git_projects/tstr/libs/rtaudio-5.1.0/build /home/paul/Documents/git_projects/tstr/libs/rtaudio-5.1.0/build/tests /home/paul/Documents/git_projects/tstr/libs/rtaudio-5.1.0/build/tests/CMakeFiles/teststops.dir/DependInfo.cmake --color=$(COLOR)
.PHONY : tests/CMakeFiles/teststops.dir/depend

