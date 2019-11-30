# Made based on : https://medium.com/heuristics/c-application-development-part-3-cmakelists-txt-from-scratch-7678253e5e24
# Might also read : https://medium.com/@onur.dundar1/cmake-tutorial-585dd180109b


# --- cmake version to be used
cmake_minimum_required( VERSION 3.0 )


# --- project name
project( tstr )


# --- flags
# This section is to tell CMake which compiler and compiler
# version you wish to build your project with. If you don’t
# set anything, it will pick the best fit on its own.


# --- files
# In this section we basically specify all the files and
# club then into sensible variable names like source,
# include, etc. It is just to ease things out, but if you
# wish you can totally skip this section and use the file
# names directly instead of the variables

set(RTAUDIO_DIR "./libs/rtaudio-5.1.0")

# --- include
# This command is used to specify the path of the include
# directories that you want the compiler to look into while
# searching for header files while compiling your code.
# This will also include the header files from 3rd party
# libraries.

include_directories( ./include ./src ${RTAUDIO_DIR} )


# --- target
# This is the part where we tell CMake the name of the
# output file, in our case we wish to name it as reverb.
# Whatever files names follow after that are basically your
# source files same way as you do while compiling them
# manually.

add_executable( reverb ./src/main.cpp )


# --- external libs
# This part is what we call linking in compilation terms.
# So what you have done is you have included the header 
# files of these 3rd party libraries and now you need to
# tell the compiler where exactly are these libraries
# located.

add_subdirectory( ${RTAUDIO_DIR} )
target_link_libraries( reverb rtaudio )