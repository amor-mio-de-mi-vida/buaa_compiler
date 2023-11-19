//
// Created by 杨贺森 on 2023/11/1.
//

#ifndef BUAA_COMPILER_UTILS_H
#define BUAA_COMPILER_UTILS_H

#include "lexer.h"
#include <unordered_map>
#include <utility>
#include <string>

class Type {
public:
    int id; // 0为int
    int dim;
    vector<int> boundary;
    Type(int id, int dim) {
        this->id = id;
        this->dim = dim;
        this->boundary = *new vector<int>();
    }
    void pushBoundary(int bound);
};

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

class Register {
public:
    int id;
    string name;
    string addr;
    int value;
    bool hasValue;
    Type type = Type(0,0);
    bool isGlobal;
    bool isConst;

    Register(int id, const string& name, const string& addr, int value, bool hasValue, int type_id, int dim, bool isGlobal, bool isConst) {
        this->id = id;
        this->name = name;
        this->addr = addr;
        this->value = value;
        this->hasValue = hasValue;
        this->type = *new Type(type_id, dim); // 0为int
        this->isGlobal = isGlobal;
        this->isConst = isConst;
    }
    Register(int num) {
        this->id = 0;
        this->name = std::to_string(num);
        this->addr = "";
        this->value = num;
        this->hasValue = true;
        this->type = *new Type(0,0);
        this->isGlobal = false;
        this->isConst = true;
    }
    string to_string() const;
};

class RegisterTable {
public:
    int id;
    int fatherId;
    unordered_map<string, Register> directory;
    RegisterTable(int id, int fatherId) {
        this->id = id;
        this->fatherId = fatherId;
        this->directory = *new unordered_map<string, Register>();
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
int searchSymbolTable(const std::string& name, bool mode);


void pushRegisterTable();
void popRegisterTable();
bool isGlobalDecl();
void pushRegister(const string& name, const Register& result);
string typeToString(const Type& type);
int stringToType(const string& str);
void searchFuncReturn(int id, int value);
Register searchRegister(const string& name);
void pushGlobalRegister(const string& name, const Register& result);
bool isGlobalVar(const string& name);
bool isGlobalRegister(const string& name);


#endif //BUAA_COMPILER_UTILS_H
