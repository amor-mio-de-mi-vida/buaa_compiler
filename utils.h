//
// Created by 杨贺森 on 2023/11/1.
//

#ifndef BUAA_COMPILER_UTILS_H
#define BUAA_COMPILER_UTILS_H

#include "lexer.h"
#include <unordered_map>
#include <utility>

class Symbol {
public:
    int id; // 当前单词对应的poi
    int tableId; // 当前单词所在的符号表编号
    std::string name; // 当前单词对应的字符串
    int type; // 0: a, 1: a[], 2: a[][], -1: func
    int is_const; // 1: const, 0: var
//    int value;
    int func_retype; // 0: void, 1: int
    int func_paraNum; //参数数量
    vector<int> func_paramTypeList; // 参数类型

    bool hasReturn; // 判断函数是否有返回语句。

    Symbol(int id, int tableId, std::string name, int type, int is_const, int func_retype, int func_paraNum) {
        this->id = id;
        this->tableId = tableId;
        this->name = std::move(name);
        this->type = type;
        this->is_const = is_const;
        this->func_retype = func_retype;
        this->func_paraNum = func_paraNum;
        this->func_paramTypeList = *new vector<int>;
        this->hasReturn = false;
    }
};

class SymbolTable {
public:
    int id; // 当前符号表的id。
    int fatherId; //当前符号表的父id。
    unordered_map<int, Symbol>  directory; // 符号表内容

    SymbolTable(int id, int fatherId) {
        this->id = id;
        this->fatherId = fatherId;
        this->directory = *new unordered_map<int, Symbol>();
    }
};

int getParamNumber(Token token);
void pushSymbolTable();
void popSymbolTable();
int searchSymbolTable(const std::string& name, int type, bool mode); // mod为True时在所有作用域下查找，mod为False时在当前作用域下查找
int searchVar(const std::string& name, bool mode);
int searchFunc(const std::string& name, bool mode);
void pushSymbolFuncParam(const std::string& name, int type);
void pushSymbol(const std::string& name, int type, int is_const, const string& func_retype);
int getVarType(const std::string& name);
int getFuncType(const std::string& name);

#endif //BUAA_COMPILER_UTILS_H
