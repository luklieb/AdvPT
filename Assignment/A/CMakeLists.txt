# How to use CMake on Linux with GNU Makefile backend:
#   - Use a separate build folder for intermediate files
#       mkdir build && cd build
#       cmake ..
#       make
#   - you can configure the project for Debug or Release mode by running a cmake GUI in the build folder:
#       cd build
#     then either
#       ccmake . 
#     or 
#       cmake-gui .
#
# There are more generators available, for example for Eclipse, Visual Studio, Ninja, ....
# You can list all available generators with "cmake -G"
#

# add this line to prevent CMake warning: it specifies the minimum cmake version which is required 
# to build this project
#cmake_minimum_required(VERSION 3.1)  

# enable c++11 support
set (CMAKE_CXX_STANDARD 11)

set(CMAKE_CXX_FLAGS "-g -Wall -pedantic -std=c++11 -lstdc++")

# add a single executable called "vector" consisting of a single source file vector.cpp
add_executable(matrix MatrixTest.cpp MatrixProduct.cpp)



