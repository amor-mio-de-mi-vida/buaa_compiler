//
// Created by 杨贺森 on 2023/10/1.
//

#ifndef HW2_ERROR_H
#define HW2_ERROR_H

#include <iostream>
#include "lexer.h"

void getLexerErrorMessage(const std::string& str);
void getParserErrorMessage(const std::string& str, Token& token, const char* funcname);

void printError(int num, char type, const char* statement);
void checkInvalidChar(int row, std::string str);
void checkConst(const std::string& name, int row, const char* str);
void checkVarSymbol(int row, const char* str, const std::string& name, int type, int is_const);
void checkFuncSymbol(int row, const char* str, const std::string& name, const std::string& func_retype);
void checkSymbol(int row, const char* str, const std::string& name, int type, int is_const, const std::string& func_retype);
void checkVarDefine(int row, const char* str, const std::string& name);
void checkFuncDefine(int row, const char* str, const std::string& name);
void checkReturn(int row, const char* str, int retype);
void checkHasReturn(int row, const char* str);
void checkParaNum(int row, const char* str, const std::string& name, int num);
void checkParaType(int row, const char* str, const std::string& funcname, int index, int type);

#endif //HW2_ERROR_H
