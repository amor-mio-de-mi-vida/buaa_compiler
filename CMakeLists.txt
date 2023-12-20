cmake_minimum_required(VERSION 3.26)
project(hw2)

set(CMAKE_CXX_STANDARD 17)

add_executable(hw2 main.cpp
        parser.cpp
        parser.h
        token.h
        token.cpp
        error.h)
