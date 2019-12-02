# Made based on : https://medium.com/heuristics/c-application-development-part-3-cmakelists-txt-from-scratch-7678253e5e24
# Might also read : https://medium.com/@onur.dundar1/cmake-tutorial-585dd180109b
# And read that to use with vscode debugger : https://medium.com/audelabs/c-development-using-visual-studio-code-cmake-and-lldb-d0f13d38c563


# --- cmake version to be used
cmake_minimum_required( VERSION 3.0 )


# --- project name
project( tstr )


# --- flags
# This section is to tell CMake which compiler and compiler
# version you wish to build your project with. If you don’t
# set anything, it will pick the best fit on its own.

set( CMAKE_BUILD_TYPE Debug )


# --- files
# In this section we basically specify all the files and
# club then into sensible variable names like source,
# include, etc. It is just to ease things out, but if you
# wish you can totally skip this section and use the file
# names directly instead of the variables

set( SOURCE_MAIN "main_t2.cpp" )

set( SOURCE_DIR "./src" )
set( INCLUDE_DIR "./include" )
set( SOURCE "${SOURCE_DIR}/${SOURCE_MAIN}" "${SOURCE_DIR}/func.cpp" "${SOURCE_DIR}/somefunc.cpp" )
set( RTAUDIO_DIR "./libs/rtaudio-5.1.0" )

get_filename_component( ROOT_DIR_NAME "${PROJECT_SOURCE_DIR}" NAME )


# --- include
# This command is used to specify the path of the include
# directories that you want the compiler to look into while
# searching for header files while compiling your code.
# This will also include the header files from 3rd party
# libraries.

include_directories( ${INCLUDE_DIR} ${SOURCE_DIR} ${RTAUDIO_DIR} )


# --- target
# This is the part where we tell CMake the name of the
# output file, in our case we wish to name it as ${ROOT_DIR_NAME}.
# Whatever files names follow after that are basically your
# source files same way as you do while compiling them
# manually.

add_executable( ${ROOT_DIR_NAME} ${SOURCE} )


# --- external libs
# This part is what we call linking in compilation terms.
# So what you have done is you have included the header 
# files of these 3rd party libraries and now you need to
# tell the compiler where exactly are these libraries
# located.

# Add directories with a CMakeLists.txt file
add_subdirectory( ${RTAUDIO_DIR} )

target_link_libraries( ${ROOT_DIR_NAME} rtaudio )