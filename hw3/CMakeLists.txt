cmake_minimum_required(VERSION 3.26)
project(hw3)

set(CMAKE_CXX_STANDARD 17)

add_executable(hw3 main.cpp
        error.h
        parser.cpp
        parser.h
        lexer.cpp
        lexer.h
        error.cpp)
