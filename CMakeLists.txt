cmake_minimum_required(VERSION 3.26)
project(buaa_compiler)

set(CMAKE_CXX_STANDARD 17)

add_executable(buaa_compiler main.cpp
        error.h
        parser.cpp
        parser.h
        lexer.cpp
        lexer.h
        error.cpp
        utils.cpp
        utils.h
        llvm_ir.cpp
        llvm_ir.h
        generate_ir.h
        generate_ir.cpp
        ir.h
        ir.cpp
        mips.h
        mips.cpp)
