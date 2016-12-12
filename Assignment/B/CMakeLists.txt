cmake_minimum_required(VERSION 2.8)
SET(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++11 -Wall -pedantic -O3")

add_executable( SolverTest SolverTest.cpp)
add_executable( SolverAddressSanitizer SolverShort.cpp)
add_executable( SolverValgrind SolverShort.cpp)

SET_TARGET_PROPERTIES(SolverTest PROPERTIES COMPILE_FLAGS "-DNDEBUG")

SET_TARGET_PROPERTIES(SolverAddressSanitizer PROPERTIES COMPILE_FLAGS "-fsanitize=address -fno-omit-frame-pointer")
SET_TARGET_PROPERTIES(SolverAddressSanitizer PROPERTIES LINK_FLAGS "-fsanitize=address")
