//
// Created by 杨贺森 on 2023/10/1.
//

#ifndef HW2_ERROR_H
#define HW2_ERROR_H

#include <iostream>
#include "lexer.h"

void getLexerErrorMessage(const std::string& str);
void getParserErrorMessage(const std::string& str, Token& token, const char* funcname);

void printError(int num, char type);
void checkInvalidChar(std::string str);

#endif //HW2_ERROR_H
