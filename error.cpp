//
// Created by 杨贺森 on 2023/10/6.
//
#include "error.h"
#include <cstdio>
#include <fstream>


extern int rowNo;
extern ofstream fout;
extern ofstream ferr;
extern bool error;
extern int colNo;

void getLexerErrorMessage(const std::string& str) {
    fprintf(stderr, "Lexer error: %s at row: %d, column: %d\n", str.c_str(), rowNo, colNo);
}

void getParserErrorMessage(const std::string& str, Token& token, const char* funcname) {
    fprintf(stderr, "Parser error: %s at row: %d, column: %d, current function name is %s\n", str.c_str(), token.getRowNo(), token.getColNo(), funcname);

}

void printError(int num, char type) {
    if (error) {
        ferr << num << " " << type << endl;
    }
}

void checkInvalidChar(std::string str) {
    int pos = 1;
    for (; pos < str.length() - 1; pos++) {
        if (str[pos] == '%') {
            pos++;
            if (str[pos] != 'd') {
                printError(rowNo, 'a');
            }
        } else if (str[pos] == '\\') {
            pos++;
            if (str[pos] != 'n') {
                printError(rowNo, 'a');
            }
        } else if (str[pos] != 32 && str[pos] != 33 && !(str[pos] >= 40 && str[pos] <= 126)) {
            printError(rowNo, 'a');
        }
    }
}

