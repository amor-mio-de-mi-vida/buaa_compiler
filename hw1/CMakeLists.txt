cmake_minimum_required(VERSION 3.26)
project(hw1)

set(CMAKE_CXX_STANDARD 14)

include_directories(.)

add_executable(hw1
        generate.cpp
        generate.h
        output/testfile1.c
        output/testfile2.c)
