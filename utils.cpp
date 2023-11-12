//
// Created by 杨贺森 on 2023/11/1.
//
#include "utils.h"
#include "parser.h"
int symbolTableId = 0;
int currentSymbolTableId = 0;
int symbolId = 0;
int currentSymbolId = 0;
int currentFuncId = 0;

unordered_map<int, SymbolTable> symbolTableList;
unordered_map<int, Symbol> symbolList;

int getParamNumber(Token token) {
    std::string string = token.getName();
    int result = 0;
    for (int pos = 0; pos < string.length(); pos++) {
        if(string.at(pos) == '%' && string.at(pos+1) == 'd') {
            result++;
        }
    }
    return result;
}

void pushSymbolTable() {
    SymbolTable result = *new SymbolTable(symbolTableId, currentSymbolTableId);
    symbolTableList.insert(make_pair(symbolTableId, result));
    currentSymbolTableId = symbolTableId;
    symbolTableId++;
}

void popSymbolTable() {
    currentSymbolTableId = symbolTableList.at(currentSymbolTableId).fatherId;
}

int searchVar(const std::string& name, bool mode) {
    return searchSymbolTable(name, 0, mode) + searchSymbolTable(name, 1, mode) + searchSymbolTable(name, 2, mode) + 2;
}

int searchFunc(const std::string& name, bool mode) {
    return searchSymbolTable(name, -1, mode);
}

int searchSymbolTable(const std::string& name, int type, bool mode) {
    SymbolTable symbolTable = symbolTableList.at(currentSymbolTableId);
    if (mode) {
        while (true) {
            unordered_map<int, Symbol> directory = symbolTable.directory;
            auto iter = directory.begin();
            for (; iter != directory.end(); iter++) {
                if (iter->second.name == name && iter->second.type == type) {
                    return iter->second.id;
                }
            }
            if (symbolTable.fatherId == symbolTable.id) {
                break;
            }
            symbolTable = symbolTableList.at(symbolTable.fatherId);
        }
    } else {
        unordered_map<int, Symbol> directory = symbolTable.directory;
        auto iter = directory.begin();
        for (; iter != directory.end(); iter++) {
            if (iter->second.name == name && iter->second.type == type) {
                return iter->second.id;
            }
        }
    }
    return -1;
}

void pushSymbolFuncParam(const std::string& name, int type) {
    int id = searchFunc(name, true);
    if (id != -1) {
        symbolList.at(id).func_paramTypeList.push_back(type);
        symbolList.at(id).func_paraNum++;
    }
}

void pushSymbol(const std::string& name, int type, int is_const, const string& func_retype) {
    int retype = 0;
    if (func_retype == "INTTK")
        retype = 1;
    int id = symbolId++;
    Symbol symbol = *new Symbol(id, currentSymbolTableId, name, type, is_const, retype, 0);
    symbolTableList.at(currentSymbolTableId).directory.insert(make_pair(id, symbol));
    symbolList.insert(make_pair(id, symbol));
    currentSymbolId = id;
    if (type == -1) {
        currentFuncId = id;
    }
}

int getVarType(const std::string& name) {
    int id = searchVar(name, true);
    if (id != -1) {
        return symbolList.at(id).type;
    }
    return -1;
}

int getFuncType(const std::string& name) {
    int id = searchFunc(name, true);
    if (id != -1) {
        return symbolList.at(id).func_retype;
    }
    return -1;
}