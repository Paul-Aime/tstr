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
CMAKE_SOURCE_DIR = /home/paul/Documents/git_projects/tstr

# The top-level build directory on which CMake was run.
CMAKE_BINARY_DIR = /home/paul/Documents/git_projects/tstr/build

# Utility rule file for ContinuousConfigure.

# Include the progress variables for this target.
include libs/rtaudio-5.1.0/CMakeFiles/ContinuousConfigure.dir/progress.make

libs/rtaudio-5.1.0/CMakeFiles/ContinuousConfigure:
	cd /home/paul/Documents/git_projects/tstr/build/libs/rtaudio-5.1.0 && /usr/bin/ctest -D ContinuousConfigure

ContinuousConfigure: libs/rtaudio-5.1.0/CMakeFiles/ContinuousConfigure
ContinuousConfigure: libs/rtaudio-5.1.0/CMakeFiles/ContinuousConfigure.dir/build.make

.PHONY : ContinuousConfigure

# Rule to build all files generated by this target.
libs/rtaudio-5.1.0/CMakeFiles/ContinuousConfigure.dir/build: ContinuousConfigure

.PHONY : libs/rtaudio-5.1.0/CMakeFiles/ContinuousConfigure.dir/build

libs/rtaudio-5.1.0/CMakeFiles/ContinuousConfigure.dir/clean:
	cd /home/paul/Documents/git_projects/tstr/build/libs/rtaudio-5.1.0 && $(CMAKE_COMMAND) -P CMakeFiles/ContinuousConfigure.dir/cmake_clean.cmake
.PHONY : libs/rtaudio-5.1.0/CMakeFiles/ContinuousConfigure.dir/clean

libs/rtaudio-5.1.0/CMakeFiles/ContinuousConfigure.dir/depend:
	cd /home/paul/Documents/git_projects/tstr/build && $(CMAKE_COMMAND) -E cmake_depends "Unix Makefiles" /home/paul/Documents/git_projects/tstr /home/paul/Documents/git_projects/tstr/libs/rtaudio-5.1.0 /home/paul/Documents/git_projects/tstr/build /home/paul/Documents/git_projects/tstr/build/libs/rtaudio-5.1.0 /home/paul/Documents/git_projects/tstr/build/libs/rtaudio-5.1.0/CMakeFiles/ContinuousConfigure.dir/DependInfo.cmake --color=$(COLOR)
.PHONY : libs/rtaudio-5.1.0/CMakeFiles/ContinuousConfigure.dir/depend

