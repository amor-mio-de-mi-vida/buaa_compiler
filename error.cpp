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

extern int currentSymbolTableId;
extern int currentSymbolId;
extern int currentFuncId;
extern std::vector<Token>::iterator iter;
extern std::vector<Token> TokenList;
extern bool ir;

extern unordered_map<int, SymbolTable> symbolTableList;
extern unordered_map<int, Symbol> symbolList;

void getLexerErrorMessage(const std::string& str) {
    if (debug && error)
        fprintf(stderr, "Lexer error: %s at row: %d, column: %d\n", str.c_str(), rowNo, colNo);
}

void getParserErrorMessage(const std::string& str, Token& token, const char* funcname) {
    if (debug && error)
        fprintf(stderr, "Parser error: %s at row: %d, column: %d, current function name is %s\n", str.c_str(), token.getRowNo(), token.getColNo(), funcname);
}

void printError(int num, char type, const char*statement) {
    ir = false;
    if (error) {
        if (debug)
            cout << "In function " << statement << " error number: " << type << " in row " << num << endl;
        ferr << num << " " << type << endl;
    }
}

void checkInvalidChar(int row, std::string str) {
    bool flag = false;
    int pos = 1;
    for (; pos < str.length() - 1; pos++) {
        if (str[pos] == '%') {
            pos++;
            if (str[pos] != 'd') {
                flag = true;
            }
        } else if (str[pos] == '\\') {
            pos++;
            if (str[pos] != 'n') {
                flag = true;
            }
        } else if (str[pos] != 32 && str[pos] != 33 && !(str[pos] >= 40 && str[pos] <= 126)) {
            flag = true;
        }
    }
    if (flag)
        printError(row, 'a', "parsePrintfStmt");
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

bool checkName(int row, const char* str, const std::string& name) {
    if (error) {
        int id = searchSymbolTable(name, false);
        if (id != -1) {
            printError(row, 'b', str);
            return true;
        }
    }
    return false;
}

bool checkVarSymbol(int row, const char* str, const std::string& name, int type, int is_const) {
    return checkSymbol(row, str, name, type, is_const, "");
}

bool checkFuncSymbol(int row, const char* str, const std::string& name, const std::string& func_retype) {
    return checkSymbol(row, str, name, -1, 0, func_retype);
}

// 检查变量名重定义
bool checkSymbol(int row, const char* str, const std::string& name, int type, int is_const, const std::string& func_retype) {
    if (searchSymbolTable(name, false) != -1) {
        printError(row, 'b', str);
        return true;
    } else {
        pushSymbol(name, type, is_const, func_retype);
        return false;
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
        if (index < func.func_paramTypeList.size() && func.func_paramTypeList[index] != type) {
            printError(row, 'e', str);
        }
    }
}

void skip(int type) {
    if (type == 0) {
        return;
    } else {
        while (iter != TokenList.end()) {
            if (iter->getType() == "VOIDTK") {
                break;
            }
            if (iter->getType() == "INTTK") {
                if (iter + 1 == TokenList.end()) {
                    iter += 1;
                    break;
                } else {
                    if (iter+2 == TokenList.end()) {
                        iter += 2;
                        break;
                    } else if ((iter+2)->getType() == "LPARENT") {
                        break;
                    }
                }
            }
            iter++;
        }
    }
}