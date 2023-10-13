//
// Created by 杨贺森 on 2023/10/6.
//
#include "error.h"
#include <cstdio>
#include <fstream>


extern int rowNo;
extern ofstream fout;
extern int colNo;

void getLexerErrorMessage(const std::string& str) {
    fprintf(stderr, "Lexer error: %s at row: %d, column: %d\n", str.c_str(), rowNo, colNo);
}

void getParserErrorMessage(const std::string& str, Token& token, const char* funcname) {
    fprintf(stderr, "Parser error: %s at row: %d, column: %d, current function name is %s\n", str.c_str(), token.getRowNo(), token.getColNo(), funcname);

}