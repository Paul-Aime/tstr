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

# Include any dependencies generated for this target.
include libs/rtaudio-5.1.0/tests/CMakeFiles/testall.dir/depend.make

# Include the progress variables for this target.
include libs/rtaudio-5.1.0/tests/CMakeFiles/testall.dir/progress.make

# Include the compile flags for this target's objects.
include libs/rtaudio-5.1.0/tests/CMakeFiles/testall.dir/flags.make

libs/rtaudio-5.1.0/tests/CMakeFiles/testall.dir/testall.cpp.o: libs/rtaudio-5.1.0/tests/CMakeFiles/testall.dir/flags.make
libs/rtaudio-5.1.0/tests/CMakeFiles/testall.dir/testall.cpp.o: ../libs/rtaudio-5.1.0/tests/testall.cpp
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --progress-dir=/home/paul/Documents/git_projects/tstr/build/CMakeFiles --progress-num=$(CMAKE_PROGRESS_1) "Building CXX object libs/rtaudio-5.1.0/tests/CMakeFiles/testall.dir/testall.cpp.o"
	cd /home/paul/Documents/git_projects/tstr/build/libs/rtaudio-5.1.0/tests && /usr/bin/c++  $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -o CMakeFiles/testall.dir/testall.cpp.o -c /home/paul/Documents/git_projects/tstr/libs/rtaudio-5.1.0/tests/testall.cpp

libs/rtaudio-5.1.0/tests/CMakeFiles/testall.dir/testall.cpp.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Preprocessing CXX source to CMakeFiles/testall.dir/testall.cpp.i"
	cd /home/paul/Documents/git_projects/tstr/build/libs/rtaudio-5.1.0/tests && /usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -E /home/paul/Documents/git_projects/tstr/libs/rtaudio-5.1.0/tests/testall.cpp > CMakeFiles/testall.dir/testall.cpp.i

libs/rtaudio-5.1.0/tests/CMakeFiles/testall.dir/testall.cpp.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Compiling CXX source to assembly CMakeFiles/testall.dir/testall.cpp.s"
	cd /home/paul/Documents/git_projects/tstr/build/libs/rtaudio-5.1.0/tests && /usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -S /home/paul/Documents/git_projects/tstr/libs/rtaudio-5.1.0/tests/testall.cpp -o CMakeFiles/testall.dir/testall.cpp.s

libs/rtaudio-5.1.0/tests/CMakeFiles/testall.dir/testall.cpp.o.requires:

.PHONY : libs/rtaudio-5.1.0/tests/CMakeFiles/testall.dir/testall.cpp.o.requires

libs/rtaudio-5.1.0/tests/CMakeFiles/testall.dir/testall.cpp.o.provides: libs/rtaudio-5.1.0/tests/CMakeFiles/testall.dir/testall.cpp.o.requires
	$(MAKE) -f libs/rtaudio-5.1.0/tests/CMakeFiles/testall.dir/build.make libs/rtaudio-5.1.0/tests/CMakeFiles/testall.dir/testall.cpp.o.provides.build
.PHONY : libs/rtaudio-5.1.0/tests/CMakeFiles/testall.dir/testall.cpp.o.provides

libs/rtaudio-5.1.0/tests/CMakeFiles/testall.dir/testall.cpp.o.provides.build: libs/rtaudio-5.1.0/tests/CMakeFiles/testall.dir/testall.cpp.o


# Object files for target testall
testall_OBJECTS = \
"CMakeFiles/testall.dir/testall.cpp.o"

# External object files for target testall
testall_EXTERNAL_OBJECTS =

libs/rtaudio-5.1.0/tests/testall: libs/rtaudio-5.1.0/tests/CMakeFiles/testall.dir/testall.cpp.o
libs/rtaudio-5.1.0/tests/testall: libs/rtaudio-5.1.0/tests/CMakeFiles/testall.dir/build.make
libs/rtaudio-5.1.0/tests/testall: libs/rtaudio-5.1.0/librtaudio.so.6.1.0
libs/rtaudio-5.1.0/tests/testall: /usr/lib/x86_64-linux-gnu/libasound.so
libs/rtaudio-5.1.0/tests/testall: libs/rtaudio-5.1.0/tests/CMakeFiles/testall.dir/link.txt
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --bold --progress-dir=/home/paul/Documents/git_projects/tstr/build/CMakeFiles --progress-num=$(CMAKE_PROGRESS_2) "Linking CXX executable testall"
	cd /home/paul/Documents/git_projects/tstr/build/libs/rtaudio-5.1.0/tests && $(CMAKE_COMMAND) -E cmake_link_script CMakeFiles/testall.dir/link.txt --verbose=$(VERBOSE)

# Rule to build all files generated by this target.
libs/rtaudio-5.1.0/tests/CMakeFiles/testall.dir/build: libs/rtaudio-5.1.0/tests/testall

.PHONY : libs/rtaudio-5.1.0/tests/CMakeFiles/testall.dir/build

libs/rtaudio-5.1.0/tests/CMakeFiles/testall.dir/requires: libs/rtaudio-5.1.0/tests/CMakeFiles/testall.dir/testall.cpp.o.requires

.PHONY : libs/rtaudio-5.1.0/tests/CMakeFiles/testall.dir/requires

libs/rtaudio-5.1.0/tests/CMakeFiles/testall.dir/clean:
	cd /home/paul/Documents/git_projects/tstr/build/libs/rtaudio-5.1.0/tests && $(CMAKE_COMMAND) -P CMakeFiles/testall.dir/cmake_clean.cmake
.PHONY : libs/rtaudio-5.1.0/tests/CMakeFiles/testall.dir/clean

libs/rtaudio-5.1.0/tests/CMakeFiles/testall.dir/depend:
	cd /home/paul/Documents/git_projects/tstr/build && $(CMAKE_COMMAND) -E cmake_depends "Unix Makefiles" /home/paul/Documents/git_projects/tstr /home/paul/Documents/git_projects/tstr/libs/rtaudio-5.1.0/tests /home/paul/Documents/git_projects/tstr/build /home/paul/Documents/git_projects/tstr/build/libs/rtaudio-5.1.0/tests /home/paul/Documents/git_projects/tstr/build/libs/rtaudio-5.1.0/tests/CMakeFiles/testall.dir/DependInfo.cmake --color=$(COLOR)
.PHONY : libs/rtaudio-5.1.0/tests/CMakeFiles/testall.dir/depend

