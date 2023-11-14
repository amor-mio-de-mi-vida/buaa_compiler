//
// Created by 杨贺森 on 2023/10/6.
//
#include "error.h"
#include "lexer.h"
#include "utils.h"
#include <cstdio>
#include <fstream>


extern int rowNo;
extern ofstream fout;
extern ofstream ferr;
extern bool error;
extern bool debug;
extern int colNo;
extern int isDup;

extern int currentSymbolTableId;
extern int currentSymbolId;
extern int currentFuncId;

extern unordered_map<int, SymbolTable> symbolTableList;
extern unordered_map<int, Symbol> symbolList;

void getLexerErrorMessage(const std::string& str) {
    if (debug)
        fprintf(stderr, "Lexer error: %s at row: %d, column: %d\n", str.c_str(), rowNo, colNo);
}

void getParserErrorMessage(const std::string& str, Token& token, const char* funcname) {
    if (debug)
        fprintf(stderr, "Parser error: %s at row: %d, column: %d, current function name is %s\n", str.c_str(), token.getRowNo(), token.getColNo(), funcname);
}

void printError(int num, char type, const char*statement) {
    if (error) {
        if (debug)
            cout << "In function " << statement << " error number: " << type << " in row " << num << endl;
        ferr << num << " " << type << endl;
    }
}

void checkInvalidChar(int row, std::string str) {
    int pos = 1;
    for (; pos < str.length() - 1; pos++) {
        if (str[pos] == '%') {
            pos++;
            if (str[pos] != 'd') {
                printError(row, 'a', "parsePrintfStmt");
            }
        } else if (str[pos] == '\\') {
            pos++;
            if (str[pos] != 'n') {
                printError(row, 'a', "parsePrintfStmt");
            }
        } else if (str[pos] != 32 && str[pos] != 33 && !(str[pos] >= 40 && str[pos] <= 126)) {
            printError(row, 'a', "parsePrintfStmt");
        }
    }
}

void checkConst(const std::string& name, int row, const char* str) {
    int id = searchVar(name, true);
    if (id != -1) {
        Symbol symbol = symbolList.at(id);
        if (symbol.is_const) {
            printError(row, 'h', str);
        }
    }
}

void checkVarSymbol(int row, const char* str, const std::string& name, int type, int is_const) {
    checkSymbol(row, str, name, type, is_const, "");
}

void checkFuncSymbol(int row, const char* str, const std::string& name, const std::string& func_retype) {
    checkSymbol(row, str, name, -1, 0, func_retype);
}

// 检查变量名重定义
void checkSymbol(int row, const char* str, const std::string& name, int type, int is_const, const std::string& func_retype) {
    if (searchSymbolTable(name, type, false) != -1) {
        printError(row, 'b', str);
        isDup++;
    } else {
        pushSymbol(name, type, is_const, func_retype);
    }
}

void checkVarDefine(int row, const char* str, const std::string& name) {
    if (searchVar(name, true) == -1) {
        printError(row, 'c', str);
    }
}

void checkFuncDefine(int row, const char* str, const std::string& name) {
    if (searchFunc(name, true) == -1) {
        printError(row, 'c', str);
    }
}

void checkReturn(int row, const char* str, int retype) {
    Symbol currentFunc = symbolList.at(currentFuncId);
    if (currentFunc.func_retype == 0 && retype != currentFunc.func_retype) { // 只考虑无返回值的函数的匹配情况
        printError(row, 'f', str);
    }
    symbolList.at(currentFuncId).hasReturn = true;
}

void checkHasReturn(int row, const char* str) {
    Symbol currentFunc = symbolList.at(currentFuncId);
    if (currentFunc.func_retype && !currentFunc.hasReturn) {
        printError(row, 'g', str);
    }
}

void checkParaNum(int row, const char* str, const std::string& name, int num) {
    int id = searchFunc(name, true);
    if (id != -1) {
        Symbol func = symbolList.at(id);
        if (num != func.func_paraNum) {
            printError(row, 'd', str);
        }
    }
}

void checkParaType(int row, const char* str, const std::string& funcname, int index, int type) {
    int id = searchFunc(funcname, true);
    if (id != -1) {
        Symbol func = symbolList.at(id);
        if (func.func_paramTypeList[index] != type) {
            printError(row, 'e', str);
        }
    }
}