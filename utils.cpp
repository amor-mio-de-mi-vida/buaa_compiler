//
// Created by 杨贺森 on 2023/11/1.
//
#include "utils.h"
#include "lexer.h"
#include "parser.h"

int symbolTableId = 0;
int currentSymbolTableId = 0;
int symbolId = 0;
int currentSymbolId = 0;
int currentFuncId = 0;
int registerTableId = 0;
int currentRegisterTableId = 0;


unordered_map<int, SymbolTable> symbolTableList;
unordered_map<int, Symbol> symbolList;
unordered_map<int, RegisterTable> registerTableList;
unordered_map<int, Register> registerList;


void Type::pushBoundary(int bound) {
    dim++;
    this->boundary.push_back(bound);
}

string Type::to_str() const {
    Type result = Type(this->id, 0);
    string type_str;
    if (this->id == -2) {
        type_str = "i1";
    } else if (this->id == -1) {
        type_str = "void";
    } else if (this->id == 0) {
        type_str = "i32";
    } else {
        type_str = "not valid type_id";
    }

    if (this->id == 0) {
        if (this->dim == 0) {
            return type_str;
        }
        if (this->dim == 1) {
            if (this->boundary.empty()) {
                return "i32*";
            } else if (this->boundary.size() == 1) {
                return "[" + to_string(this->boundary.at(0)) + " x " + type_str + "]";
            }
        } else if (this->dim == 2) {
            if (this->boundary.size() == 1) {
                return "[" + to_string(this->boundary.at(0)) + " x " + type_str + "] *";
            } else if (this->boundary.size() == 2) {
                return "[" + to_string(this->boundary.at(0)) + " x [" + to_string(this->boundary.at(1)) + " x " + type_str + "]]";
            }
        }
    } else {
        return type_str;
    }

    return "not valid type";
}



//string typeToString(const Type& type) {
//    if (type.id == -2) {
//        return "i1";
//    } if (type.id == -1) {
//        return "void";
//    } else if (type.id == 0 && type.dim == 0) {
//        return "i32";
//    } else if (type.id == 0 && type.dim == 1) {
//        if (type.boundary.size() == 1) {
//            return "[" + to_string(type.boundary.at(0)) + " x i32]";
//        } else {
//            return "i32*";
//        }
//    } else if (type.id == 0 && type.dim == 2) {
//        if (type.boundary.size() == 2) {
//            return "[" + to_string(type.boundary.at(0)) + " x [" + to_string(type.boundary.at(1)) + " x i32]]";
//        } else if (type.boundary.size() == 1) {
//            return "[" + to_string(type.boundary.at(0)) + " x i32]*";
//        } else if (type.boundary.empty()) {
//            return "int32**";
//        }
//    }
//    return "";
//}

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
    return searchSymbolTable(name, mode);
}

int searchFunc(const std::string& name, bool mode) {
    return searchSymbolTable(name, -1, mode);
}

int searchSymbolTable(const std::string& name, bool mode) {
    SymbolTable symbolTable = symbolTableList.at(currentSymbolTableId);
    if (mode) {
        while (true) {
            unordered_map<int, Symbol> directory = symbolTable.directory;
            auto iter = directory.begin();
            for (; iter != directory.end(); iter++) {
                if (iter->second.name == name) {
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
            if (iter->second.name == name) {
                return iter->second.id;
            }
        }
    }
    return -1;
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

string Register::to_string() const {
    if (this->hasValue) {
        return this->name;
    } else if (this->isGlobal) {
        return "";
    } else {
        return this->name;
    }
}


void pushRegisterTable() {
    RegisterTable result = *new RegisterTable(registerTableId, currentRegisterTableId);
    registerTableList.insert(make_pair(registerTableId, result));
    currentRegisterTableId = registerTableId;
    registerTableId++;
}

void popRegisterTable() {
    currentRegisterTableId = registerTableList.at(currentRegisterTableId).fatherId;
}

bool isGlobalDecl() {
    return currentRegisterTableId == registerTableList.at(currentRegisterTableId).fatherId;
}

void pushRegister(const string& name, const Register& result) {
    registerTableList.at(currentRegisterTableId).directory.insert(make_pair(name, result));
}
void pushGlobalRegister(const string& name, const Register& result) {
    registerTableList.at(0).directory.insert(make_pair(name, result));
}

bool isGlobalRegister(const string& name) {
    RegisterTable registerTable = registerTableList.at(currentRegisterTableId);
    while (true) {
        unordered_map<string, Register> directory = registerTable.directory;
        auto iter = directory.begin();
        iter = directory.find(name);
        if (iter != directory.end() && registerTable.id == 0) {
            return true;
        } else if (iter != directory.end() && registerTable.id != 0) {
            return false;
        }

        if (registerTable.fatherId == registerTable.id) {
            break;
        }
        registerTable = registerTableList.at(registerTable.fatherId);
    }
    return false;
//    auto iter = registerTableList.at(0).directory.find(name);
//    if (iter != registerTableList.at(0).directory.end()) {
//        return true;
//    }
//    return false;
}

Register searchRegister(const string& name) {
    RegisterTable registerTable = registerTableList.at(currentRegisterTableId);
    while (true) {
        unordered_map<string, Register> directory = registerTable.directory;
        auto iter = directory.begin();
        iter = directory.find(name);
        if (iter != directory.end())
            return iter->second;

        if (registerTable.fatherId == registerTable.id) {
            break;
        }
        registerTable = registerTableList.at(registerTable.fatherId);
    }
    return *new Register(-1);
}

int stringToType(const string& str) {
    if (str == "VOIDTK") {
        return 0;
    } else if (str == "INTTK") {
        return 1;
    }
    return 0;
}

bool isGlobalVar(const string& name) {
    RegisterTable registerTable = registerTableList.at(0);
    unordered_map<string, Register> directory = registerTable.directory;
    auto iter = directory.begin();
    iter = directory.find(name);
    if (iter != directory.end())
        return true;
    return false;
}

void assignRegister(const Register& old_reg, const Register& new_reg) {
    RegisterTable registerTable = registerTableList.at(currentRegisterTableId);
    while (true) {
        auto iter = registerTable.directory.begin();
        for (; iter != registerTable.directory.end(); iter++) {
            if (iter->second.id == old_reg.id) {
                iter->second = new_reg;
                return;
            }
        }

        if (registerTable.fatherId == registerTable.id) {
            break;
        }
        registerTable = registerTableList.at(registerTable.fatherId);
    }
}
