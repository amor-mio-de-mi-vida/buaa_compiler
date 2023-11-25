//
// Created by 杨贺森 on 2023/11/14.
//
#include <iostream>
#include "lexer.h"
#include "parser.h"
#include "llvm_ir.h"
#include "utils.h"
#include "generate_ir.h"
#include <cassert>

using namespace std;

int registers = 0;

extern bool ir;
extern std::vector<Token>::iterator iter;
extern std::vector<Token> TokenList;
extern unordered_map<int, Register> registerList;
extern unordered_map<string, Register> globalFuncList;
extern unordered_map<int, RegisterTable> registerTableList;
extern int currentFuncReturnType;
bool Return = false;
bool globalDeclare = true;
vector<string> currentStmtEnd;
vector<string> currentForEnd;

Register getNewRegister(int num) {
    return *new Register(num);
}

Register getNewRegister(bool hasValue, int value, bool isGlobal, bool isConst, int type_id, int dim) {
    if (hasValue) {
        return *new Register(value);
    } else {
        string name = "%temp" + to_string(registers);
        Register result = *new Register(registers, name, name, 0, false, type_id, dim, isGlobal, isConst);
        registerList.insert(make_pair(registers++, result));
        return result;
    }
}

void allocRegister(const Register& ptr) {
    if (ptr.type.id == 0 && ptr.type.dim == 0) {
        string str = "    " + ptr.name + " = alloca i32\n";
        printllvm(str);
    } else if (ptr.type.id == 0 && ptr.type.dim == 1) {
        string str = "    " + ptr.name + " = alloca i32*\n";
        printllvm(str);
    } else if (ptr.type.id == 0 && ptr.type.dim == 2) {
        string str = "    " + ptr.name + " = alloca [" + to_string(ptr.type.boundary.at(0)) + " x i32]*\n";
        printllvm(str);
    }
}

Register allocRegister(const Type& type) {
    Register ptr = Register(0);
    ptr = getNewRegister(false, 0, false, false, type.id, type.dim);
    ptr.type.boundary = type.boundary;
    string str = "    "+ ptr.name + " = alloca " + type.to_str() + "\n";
    printllvm(str);
    return ptr;
}

void storeInitial(const Register& basePtr, vector<Register> value) {
    if (!value.empty()) {
        if (basePtr.type.dim == 0) {
            storeRegister(basePtr, value.at(0));
        } else if (basePtr.type.dim == 1) {
            int boundary = basePtr.type.boundary.at(0);
            for (int index = (int) value.size(); index < boundary; index++) {
                value.emplace_back(0);
            }
            for (int i = 0; i < boundary; i++) {
                Register ptr = printllvmGetElementPtr(basePtr, i);
                storeRegister(ptr, value.at(i));
            }
        } else if (basePtr.type.dim == 2) {
            int boundary_x = basePtr.type.boundary.at(0);
            int boundary_y = basePtr.type.boundary.at(1);
            int boundary = boundary_x * boundary_y;
            for (int index = (int) value.size(); index < boundary; index++) {
                value.emplace_back(0);
            }
            for (int i = 0; i < boundary_x; i++) {
                for (int j = 0; j < boundary_y; j++) {
                    if (i * boundary_y + j < value.size()) {
                        Register ptr = printllvmGetElementPtr(basePtr, i, j);
                        storeRegister(ptr, value.at(i * boundary_y + j));
                    }
                }
            }
        }
    }
}

void storeRegister(const Register& ptr, const Register& value) {  // ptr是基地址
    string str;
    if (ptr.type.dim == 2 && ptr.type.boundary.size() == 1) {
        str = "    store [" + to_string(ptr.type.boundary.at(0)) +  " x i32]* " + value.name + ", [" + to_string(ptr.type.boundary.at(0)) + " x i32]* * " + ptr.name + "\n";
    } else {
        str = "    store " + value.type.to_str() + " " + value.name + ", " + ptr.type.to_str() + "* " +
                     ptr.addr + "\n";
    }
    printllvm(str);
}

void loadRegister(const Register& ptr, Register& result) {
    string str;
    if (!(ptr.id==0 && ptr.name == "-1")) {
        str = "    " + result.name + " = load " + result.type.to_str() + ", " +
                     ptr.type.to_str() + "* " + ptr.addr + "\n";
    }
    printllvm(str);
}


void generateCompUnit() {
    iter = TokenList.begin();

    pushRegisterTable();
    while (isToken("VOIDTK",false, false, __FUNCTION__) || isToken("CONSTTK",false, false, __FUNCTION__) || isToken("INTTK",false, false, __FUNCTION__)) {
        if (isToken("VOIDTK", false, false, __FUNCTION__)) {
            currentFuncReturnType = 0;
            generateFuncDef();
            globalDeclare = true;
        }
        else if (isToken("CONSTTK", false, false,__FUNCTION__)) {
            generateDecl();
        } else if (isToken("INTTK", false, false,__FUNCTION__)) {
            if (preReadToken(1)!= nullptr && preReadToken(1)->getType() == "MAINTK") {
                currentFuncReturnType = 1;
                generateMainFuncDef();
                globalDeclare = true;
            } else if (preReadToken(1)!= nullptr && preReadToken(1)->getType() == "IDENFR") {
                if (preReadToken(2)!= nullptr && preReadToken(2)->getType() == "LPARENT") {
                    currentFuncReturnType = 1;
                    generateFuncDef();
                    globalDeclare = true;
                } else if (preReadToken(2)!= nullptr && (preReadToken(2)->getType() == "LBRACK"
                                                         || preReadToken(2)->getType() == "SEMICN" || preReadToken(2)->getType() == "COMMA"
                                                         || preReadToken(2)->getType() == "ASSIGN")) {
                    generateDecl();
                } else {
                    assert(0);
                }
            } else {
                assert(0);
            }
        }
    }
    if (iter != TokenList.end()) {
        assert(0);
    }
    popRegisterTable();

}


void generateMainFuncDef() {
    vector<Register> params;
    Register global = *new Register(registers++, "" ,"@main", 0, false, 0, 0, true, true);
    pushRegister("main", global);
    if (isToken("INTTK", true, true, __FUNCTION__)) {
        if (isToken("MAINTK", true, true, __FUNCTION__)) {
            if (isToken("LPARENT", true, true, __FUNCTION__)) {
                if (isToken("RPARENT", true, true, __FUNCTION__)) {
                    printllvmFuncDef(1, "main", params);
                    pushRegisterTable();
                    globalDeclare = false;
                    generateBlock(true);
                    popRegisterTable();
                } else {
                    assert(0);
                }
            } else {
                assert(0);
            }
        } else {
            assert(0);
        }
    } else {
        assert(0);
    }

    printllvm("}\n\n");
}

void generateBlock(bool funcBlock) {
    if (isToken("LBRACE", true, true, __FUNCTION__)) {
        while (!isToken("RBRACE", true, false, __FUNCTION__)) {
            bool isReturn = iter->getType() == "RETURNTK";
            generateBlockItem();
            if (funcBlock && isLastStmt() && !isReturn && currentFuncReturnType == 0) {
                printllvm("    ret void\n");
            }
        }
    } else {
        assert(0);
    }
}

void generateBlockItem() {
    if (isToken("CONSTTK", false, false, __FUNCTION__) || isToken("INTTK", false, false, __FUNCTION__)) {
        generateDecl();
    } else {
        generateStmt();
    }
}

void generateStmt() {
    if (isToken("SEMICN", false, false, __FUNCTION__)) {
        generateExpStmt();
    } else if (isToken("LBRACE", false, false, __FUNCTION__)) {
        generateBlockStmt();
    } else if (isToken("IFTK", false, false, __FUNCTION__)) {
        generateIfStmt();
    } else if (isToken("FORTK", false, false, __FUNCTION__)) {
        generateForstmt();
    } else if (isToken("BREAKTK",false, false, __FUNCTION__)) {
        generateBreakStmt();
    } else if (isToken("CONTINUETK", false, false, __FUNCTION__)) {
        generateContinueStmt();
    } else if (isToken("RETURNTK", false, false, __FUNCTION__)) {
        generateReturnStmt();
    } else if (isToken("PRINTFTK", false, false, __FUNCTION__)) {
        generatePrintfStmt();
    } else {
        if (isToken("IDENFR", false, false, __FUNCTION__)) {
            if (preReadToken(1) != nullptr && preReadToken(1)->getType() == "LPARENT") {
                generateExpStmt();
            } else {
                auto it = iter;
                bool mod = ir;
                ir = false;
                generateLVal(true);
                if (isToken("ASSIGN", true, false, __FUNCTION__)) {
                    ir = mod;
                    if (isToken("GETINTTK", false, false, __FUNCTION__)) {
                        iter = it;
                        generateGetintStmt();
                    } else {
                        iter = it;
                        generateAssignStmt();
                    }
                } else {   // 回溯
                    ir = mod;
                    iter = it;
                    generateExpStmt();
                }
            }
        } else {
            generateExpStmt();
        }
    }
}

void generateReturnStmt() {
    Return = true;
    Register result = Register(0);
    Register func = Register(0);
    if (isToken("RETURNTK", true, true, __FUNCTION__)) {
        if (isToken("SEMICN", true, false, __FUNCTION__)) {
            printllvmReturnStmt(0, getNewRegister(0));
        } else {
            if (preReadExp()) {
                result = generateExp();
            }
            if (isToken("SEMICN", true, true, __FUNCTION__)) {
            } else {
                assert(0);
            }
            printllvmReturnStmt(1, result);
            return;
        }
    } else {
        assert(0);
    }
}

Register generateExp() {
    Register name = generateAddExp();
    return name;
}

Register generateAddExp() {
    Type type = Type(0,0);
    Register left = generateMulExp();

    while (preReadToken(1)!=nullptr && (isToken("PLUS", true, false, __FUNCTION__) || isToken("MINU", true, false, __FUNCTION__))) {
        string op = (iter - 1)->getType();
        Register right = generateMulExp();
        Register result = getNewRegister(false, 0, false, false, type.id, type.dim);
        printllvmcalculate(result, left, right, op);
        left = result;
    }

//    if (isToken("PLUS", true, false, __FUNCTION__)) {
//        Register right = generateAddExp();
//        result = getNewRegister(false,0,false, false, type.id, type.dim);
//        printllvmcalculate(result, left, right, "PLUS");
//        return result;
//    } else if (isToken("MINU", true, false, __FUNCTION__)) {
//        Register right = generateAddExp();
//        result = getNewRegister(false,0,false, false, type.id, type.dim);
//        printllvmcalculate(result, left, right, "MINU");
//        return result;
//    }

    return left;
}

Register generateMulExp() {
    Type type = Type(0,0);
    Register left = generateUnaryExp();

    while (preReadToken(1) != nullptr && (isToken("MULT", true, false, __FUNCTION__ )
    || isToken("DIV", true, false, __FUNCTION__ ) || isToken("MOD", true, false, __FUNCTION__ ))) {
        string op = (iter - 1)->getType();
        Register right = generateUnaryExp();
        Register result = getNewRegister(false, 0, false, false, type.id, type.dim);
        printllvmcalculate(result, left, right, op);
        left = result;
    }

//    if(isToken("MULT", true, false, __FUNCTION__ )) {
//        Register right = generateMulExp();
//        result = getNewRegister(false,0,false, false, type.id, type.dim);
//        printllvmcalculate(result, left, right, "MULT");
//        return result;
//    } else if (isToken("DIV", true, false, __FUNCTION__ )) {
//        Register right = generateMulExp();
//        result = getNewRegister(false,0,false, false, type.id, type.dim);
//        printllvmcalculate(result, left, right, "DIV");
//        return result;
//    } else if (isToken("MOD", true, false, __FUNCTION__ )) {
//        Register right = generateMulExp();
//        result = getNewRegister(false,0,false, false, type.id, type.dim);
//        printllvmcalculate(result, left, right, "MOD");
//        return result;
//    }
    return left;
}

Register generateUnaryExp() {
    Type type = Type(0,0);
    Register result = Register(0);
    if (isToken("PLUS", false, false, __FUNCTION__) || isToken("MINU", false, false, __FUNCTION__) || isToken("NOT", false, false, __FUNCTION__)) {
        if (isToken("PLUS", true, false, __FUNCTION__)) {
            result = generateUnaryExp();
        }
        if (isToken("MINU", true, false, __FUNCTION__)) {
            Register temp = generateUnaryExp();
            result = getNewRegister(false, 0, false, false, type.id, type.dim);
            printllvmcalculate(result, Register(0), temp, "MINU");
        }
        if (isToken("NOT", true, false, __FUNCTION__)) {
            Register temp_i32 = generateUnaryExp(); // temp是i32类型的
            Register temp_i1 = getNewRegister(false, 0, false,false, -2, 0);
            printllvmCompare(temp_i1, temp_i32, Register(0), "NEQ");
            temp_i32 = printllvmZext(temp_i1);
            result = getNewRegister(false, 0, false, true, -2, type.dim);
            printllvmCompare(result, Register(1), temp_i32, "NOT");
            if (result.type.id == -2) {
                result = printllvmZext(result);
            }
        }
    } else if (isToken("IDENFR", false, false, __FUNCTION__)) {
        if (preReadToken(1) != nullptr && preReadToken(1)->getType() == "LPARENT") {
            result = generateFuncCall();
        } else {
            result = generatePrimaryExp();
        }
    } else {
        result = generatePrimaryExp();
    }
    return result;
}

Register generatePrimaryExp() {
    Register result = Register(0);
    if(isToken("LPARENT", true, false, __FUNCTION__)) {
        result = generateExp();
        if (isToken("RPARENT", true, true, __FUNCTION__)) {
        } else {
            assert(0);
        }
    } else if(isToken("IDENFR", false, false, __FUNCTION__)) {
        result = generateLVal(false);
    } else {
        result = generateNumber();
    }
    return result;
}

Register generateNumber() {
    if (isToken("INTCON", true, true, __FUNCTION__)) {
        return getNewRegister(stoi((iter-1)->getName()));
    } else {
        assert(0);
    }
}

void generateDecl() {
    if(isToken("CONSTTK", false, false, __FUNCTION__)) {
        generateConstDecl();
        return;
    } else {
        generateVarDecl();
        return;
    }
}

void generateConstDecl() {
    if (isToken("CONSTTK", true, true, __FUNCTION__ )) {
        int type_id = getType();
        generateConstDef(type_id);    // constDef 可能为nullptr
        while (isToken("COMMA", true, false, __FUNCTION__)) {
            generateConstDef(type_id);    // constDef 可能为nullptr
        }
        if (isToken("SEMICN", true, true, __FUNCTION__)) {
        } else {
            assert(0);
        }
    } else {
        assert(0);
    }
}

int getType() {
    if (isToken("INTTK", true, true, __FUNCTION__)) {
        return 0;
    } else {
        assert(0);
    }
}

void generateConstDef(int type_id) {
    Type type = Type(type_id, 0);
    bool isGlobal = isGlobalDecl();
    string name = parseIdent();
    vector<Register> value;
    vector<int> info;
    Register ptr = Register(0);
    Register bound = Register(0);

    while(isToken("LBRACK", true, false, __FUNCTION__)) {
        bound = generateConstExp();
        type.pushBoundary(bound.value);
        if (isToken("RBRACK", true, true,__FUNCTION__)) {
        } else {
            assert(0);
        }
    }

    if(!isGlobal) {
        ptr = allocRegister(type);
    } else {
        printllvmGlobalConst(name, type);
    }

    if (isToken("ASSIGN", true, true, __FUNCTION__)) {
        value = generateConstInitVal();
    } else {
        assert(0);
    }

    if (!isGlobal) {
        pushRegister(name, ptr);
        storeInitial(ptr, value);
    } else {
        if (!value.empty()) {
            printllvmGlobalAssign(name, type, value);
            Register global = *new Register(registers++, "", "@"+name, value.at(0).value, true, type.id, type.dim, true, true);
            global.type.boundary = type.boundary;
            pushRegister(name, global);
        } else {
            printllvm("zeroinitializer\n");
            Register global = *new Register(registers++, "", "@"+name, 0, true, type.id, type.dim, true, true);
            global.type.boundary = type.boundary;
            pushRegister(name, global);
        }
    }
}

vector<Register> generateConstInitVal() {
    vector<Register> result;
    Register exp = Register(0);
    int stack = 1;

    if (isToken("LBRACE", true, false, __FUNCTION__)) {
        while (stack) {
            if (isToken("LBRACE", true, false, __FUNCTION__)) {
                stack++;
                continue;
            } else if (isToken("RBRACE", true, false, __FUNCTION__)) {
                stack--;
                continue;
            } else if (isToken("COMMA", true, false, __FUNCTION__)) {
                continue;
            } else {
                Register exp0 = generateConstExp();
                result.push_back(exp0);
            }
        }
    } else {
        exp = generateConstExp();
        result.push_back(exp);
    }

    return result;
}

Register generateConstExp() {
    return generateAddExp();
}

void generateVarDecl() {
    int type_id = getType();
    generateVarDef(type_id);
    while(isToken("COMMA", true, false, __FUNCTION__)) {
        generateVarDef(type_id);
    }
    if(isToken("SEMICN", true, true, __FUNCTION__)) {
    } else {
        assert(0);
    }
}

void generateVarDef(int type_id) {
    Type type = Type(type_id, 0);
    bool isGlobal = isGlobalDecl();
    string name = parseIdent();
    vector<Register> value;
    Register ptr = Register(0);
    Register bound = Register(0);

    while(isToken("LBRACK", true, false, __FUNCTION__)) {
        bound = generateConstExp();
        type.pushBoundary(bound.value);
        if (isToken("RBRACK", true, true, __FUNCTION__)) {
        } else {
            assert(0);
        }
    }

    if(!isGlobal) {
        ptr = allocRegister(type);
    } else {
        printllvmGlobalVar(name, type);
    }

    if (isToken("ASSIGN", true, false, __FUNCTION__)) {
        value = generateInitVal();
    }

    if (!isGlobal) {
        pushRegister(name, ptr);
        storeInitial(ptr, value);
    } else {
        if (!value.empty()) {
            printllvmGlobalAssign(name, type, value);
            Register global = *new Register(registers++, "", "@"+name, value.at(0).value, true, type.id, type.dim, true, true);
            global.type.boundary = type.boundary;
            pushRegister(name, global);
        } else {
            printllvm("zeroinitializer\n");
            Register global = *new Register(registers++, "", "@"+name, 0, true, type.id, type.dim, true, true);
            global.type.boundary = type.boundary;
            pushRegister(name, global);
        }
    }
}

vector<Register> generateInitVal() {
    vector<Register> result;
    Register exp = Register(0);
    int stack = 1;

    if (isToken("LBRACE", true, false, __FUNCTION__)) {
        while (stack) {
            if (isToken("LBRACE", true, false, __FUNCTION__)) {
                stack++;
                continue;
            } else if (isToken("RBRACE", true, false, __FUNCTION__)) {
                stack--;
                continue;
            } else if (isToken("COMMA", true, false, __FUNCTION__)) {
                continue;
            } else {
                Register exp0 = generateExp();
                result.push_back(exp0);
            }
        }
    } else {
        exp = generateExp();
        result.push_back(exp);
    }

    return result;
}

Register generateLVal(bool left) {
    Type type = Type(0,0);
    Register basePtr = Register(0);
    Register ptr = Register(0);
    Register result = getNewRegister(false, 0, false, false, type.id, type.dim);
    std::string name = parseIdent();
    basePtr = searchRegister(name);
    bool isGlobal = isGlobalRegister(name);

    vector<Register> info;

    while(isToken("LBRACK", true, false, __FUNCTION__)) {
        Register index = generateExp();
        info.push_back(index);
        if (isToken("RBRACK", true, true, __FUNCTION__)) {
        } else {
            assert(0);
        }
    }

    if (info.empty()) {
        if (basePtr.type.dim == 0) {
            if (!globalDeclare && !left) {
                loadRegister(basePtr, result);
                basePtr.name = result.name;
            }
            return basePtr;
        } else {
            ptr = printllvmGetElementPtr(basePtr);
            if (basePtr.type.dim == 2 && basePtr.type.boundary.size() == 1) {
                ptr.type.boundary = basePtr.type.boundary;
            }
        }
    } else if (info.size() == 1) {
        Register mid = Register(0);
        if (!isGlobal && basePtr.type.dim != basePtr.type.boundary.size()) {
            mid = getNewRegister(false, 0, false, false, basePtr.type.id, basePtr.type.dim);
            mid.type.boundary = basePtr.type.boundary;
            loadRegister(basePtr, mid);
            basePtr = mid;
        }
        ptr = printllvmGetElementPtr(basePtr, info.at(0));
        if (ptr.type.dim == 0) {
            loadRegister(ptr, result);
            ptr.name = result.name;
        }
    } else if (info.size() == 2) {
        Register mid = Register(0);
        if (!isGlobal && basePtr.type.dim != basePtr.type.boundary.size()) {
            mid = getNewRegister(false, 0, false, false, basePtr.type.id, basePtr.type.dim);
            mid.type.boundary = basePtr.type.boundary;
            loadRegister(basePtr, mid);
            basePtr = mid;
        }
        ptr = printllvmGetElementPtr(basePtr, info.at(0), info.at(1));
        if (ptr.type.dim == 0) {
            loadRegister(ptr, result);
            ptr.name = result.name;
        }
    }

    return ptr;
}

void generateBlockStmt() {
    pushRegisterTable();
    generateBlock(false);
    popRegisterTable();
}

void generateGetintStmt() {
    Register left = generateLVal(true);
    if (isToken("ASSIGN", true, true, __FUNCTION__)) {
        if (isToken("GETINTTK",true, true, __FUNCTION__)) {
            if (isToken("LPARENT", true, true, __FUNCTION__)) {
                if (isToken("RPARENT",true, true, __FUNCTION__)) {
                    if (isToken("SEMICN", true, true, __FUNCTION__)) {
                    } else {
                        assert(0);
                    }
                } else {
                    assert(0);
                }
            } else {
                assert(0);
            }
        } else {
            assert(0);
        }
    } else {
        assert(0);
    }
    printllvmGetintStmt(left);
}

void generatePrintfStmt() {
    string str;
    vector<Register> params;
    if (isToken("PRINTFTK", true, true, __FUNCTION__)) {
        if (isToken("LPARENT", true, true, __FUNCTION__)) {
            str = iter->getName();
            if (isToken("STRCON", true, true, __FUNCTION__)) {
                while (isToken("COMMA", true, false, __FUNCTION__)) {
                    Register param = generateExp();
                    params.push_back(param);
                }
                if (isToken("RPARENT", true, true, __FUNCTION__)) {
                    if (isToken("SEMICN", true, true, __FUNCTION__ )) {
                    } else {
                        assert(0);
                    }
                } else {
                    assert(0);
                }
            } else {
                assert(0);
            }
        } else {
            assert(0);
        }
    } else {
        assert(0);
    }
    printllvmPrintfStmt(str, params);
}

void generateFuncDef() {
    vector<Register> params;

    std::string func_retype = iter->getType();
    parseFuncType();
    std::string funcname = iter->getName();
    parseIdent();

    Register global = *new Register(registers++, "" ,"@"+funcname, 0, false, stringToType(func_retype) - 1, 0, true, true);
    pushRegister(funcname, global);

    pushRegisterTable();
    if (isToken("LPARENT", true, true, __FUNCTION__)) {
        if (isToken("RPARENT", true, false, __FUNCTION__)) {
            printllvmFuncDef(stringToType(func_retype), funcname, params);
            globalDeclare = false;
            generateBlock(true);
        } else {
            if (isToken("INTTK", false, false, __FUNCTION__)) {
                params = generateFuncFParams();
            }
            if (isToken("RPARENT", true, true, __FUNCTION__)) {
            } else {
                assert(0);
            }
            printllvmFuncDef(stringToType(func_retype), funcname, params);
            globalDeclare = false;
            generateBlock(true);
        }
    } else {
        assert(0);
    }

    if (!Return) {
        printllvm("    ret void\n");
    }
    Return = false;
    popRegisterTable();
    printllvm("}\n\n");
}

vector<Register> generateFuncFParams() {
    vector<Register> params;
    Register param = generateFuncFParam();
    params.push_back(param);
    while(isToken("COMMA", true, false, __FUNCTION__)) {
        Register param1 = generateFuncFParam();
        params.push_back(param1);
    }
    return params;
}

Register generateFuncFParam() {
    Type type = Type(0,0);
    parseBType();
    std::string name = parseIdent();
    Register bound = Register(0);


    if (isToken("LBRACK", true, false, __FUNCTION__)) {
        type.dim++;
        if (isToken("RBRACK", true, true, __FUNCTION__)) {
            if (isToken("LBRACK", true, false, __FUNCTION__)) {
                type.dim++;
                bound = generateConstExp();
                if (isToken("RBRACK", true, true, __FUNCTION__)) {
                } else {
                    assert(0);
                }
            }
        } else {
            assert(0);
        }
    }

    Register result = getNewRegister(false,0,false,false, type.id, type.dim);
    if (type.dim == 2)
        result.type.boundary.push_back(bound.value);
    pushRegister(name, result);
    return result;
}

void generateExpStmt() {
    if (isToken("SEMICN", true, false, __FUNCTION__)) {
    } else {
        generateExp();
        if (isToken("SEMICN", true, true, __FUNCTION__)) {
        } else {
            assert(0);
        }
    }
}

void generateAssignStmt() {
    Register result = Register(0);
    std::string name = iter->getName();
    Register ptr = Register(0);
    vector<int> info;

    ptr = generateLVal(true);

    if (isToken("ASSIGN", true, true, __FUNCTION__)) {
        result = generateExp();
        if (isToken("SEMICN", true, true, __FUNCTION__)) {
        } else {
            assert(0);
        }
    } else {
        assert(0);
    }

    storeRegister(ptr, result);
    assignRegister(ptr, result);
}

vector<Register> generateFuncRParams() {
    vector<Register> params;
    Register param = generateExp();
    params.push_back(param);
    while (isToken("COMMA", true, false, __FUNCTION__)) {
        Register param1 = generateExp();
        params.push_back(param1);
    }
    return params;
}

Register generateFuncCall() {
    Type type = Type(0,0);
    vector<Register> params;
    string name = parseIdent();

    if (isToken("LPARENT", true, true, __FUNCTION__)) {
        if (isToken("RPARENT", true, false, __FUNCTION__)) {
        } else {
            if (preReadExp()) {
                params = generateFuncRParams();
            }
            if (isToken("RPARENT", true, true, __FUNCTION__)) {
            } else {
                assert(0);
            }
        }
    } else {
        assert(0);
    }

    Register result = getNewRegister(false, 0, false, false, type.id, type.dim);
    printllvmCallFunc(result, name, params);
    return result;
}

void generateIfStmt() {
    Register if_begin = getNewRegister(false, 0, false, false, -3, 0);
    Register if_end = getNewRegister(false, 0, false, false, -3, 0);
    Register else_end = Register(if_end);


    if (isToken("IFTK", true, true, __FUNCTION__)) {
        if (isToken("LPARENT", true, true, __FUNCTION__)) {

            generateCond(if_begin, if_end);
            printllvmLabel(if_begin);

            if (isToken("RPARENT", true, true, __FUNCTION__)) {

                generateStmt();


                if (isToken("ELSETK", true, false, __FUNCTION__)) {
                    else_end = getNewRegister(false, 0, false, false, -3, 0);

                    printllvmBranch(else_end);
                    printllvmLabel(if_end);

                    generateStmt();

                    printllvmBranch(else_end);
                    printllvmLabel(else_end);

                } else {
                    printllvmBranch(if_end);
                    printllvmLabel(if_end);
                }
            } else {
                assert(0);
            }
        } else {
            assert(0);
        }
    } else {
        assert(0);
    }
}

void generateCond(const Register& true_label, const Register& false_label) {
    generateLOrExp(true_label, false_label);
}

void generateLOrExp(const Register& true_label, const Register& false_label) {
    Type type = Type(0, 0);
    Register next_or_label = Register(0);
    if (!isLastLAndExp()) {
        next_or_label = getNewRegister(false, 0, false, false, -3, 0);
    } else {
        next_or_label = false_label;
    }
    Register left_i1 = generateLAndExp(true_label, next_or_label);
    Register right_i1 = Register(0);
    Register result = Register(0);

    if (!isLastLAndExp()) {
        // printllvmBranch(left_i1, true_label, false_label);
        printllvmLabel(next_or_label);
    }

    if (isToken("OR", false, false, __FUNCTION__)) {
        while ((isToken("OR", true, false, __FUNCTION__))) {
            bool flag = !isLastLAndExp();
            Register next_or_label0 = Register(0);
            if (flag) {
                next_or_label0 = getNewRegister(false, 0, false, false, -3, 0);
            } else {
                next_or_label0 = false_label;
            }

            right_i1 = generateLAndExp(true_label, next_or_label0);
            result = getNewRegister(false, 0, false, false, -2, 0);
            printllvmcalculate(result, left_i1, right_i1, "OR");

            printllvmBranch(result, true_label, next_or_label0);
            if (flag) {
                printllvmLabel(next_or_label0);
            }
            left_i1 = result;
        }
    }
}

Register generateLAndExp(const Register& true_label,  const Register& false_label) {
    Type type = Type(0, 0);
    Register next_and_label = Register(0);
    if (!isLastEqExp()) {
        next_and_label = getNewRegister(false, 0, false,false, -3, 0);
    } else {
        next_and_label = true_label;
    }
    Register left_i32 = generateEqExp(); // 要求返回的数据类型为i32
    Register right_i32 = Register(0);
    Register result = Register(0);

    Register left_i1 = getNewRegister(false, 0, false, false, -2, 0);
    printllvmCompare(left_i1, left_i32, Register(0), "NEQ");

    printllvmBranch(left_i1, next_and_label, false_label);
    if (!isLastEqExp()) {
        printllvmLabel(next_and_label);
    }


    Register right_i1 = Register(0);

    if (isToken("AND", false, false, __FUNCTION__)) {
        while ((isToken("AND", true, false, __FUNCTION__))) {
            bool flag = !isLastEqExp();
            Register next_and_label0 = Register(0);
            if (flag) {
                next_and_label0 = getNewRegister(false, 0, false, false, -3, 0);
            } else {
                next_and_label0 = true_label;
            }
            right_i32 = generateEqExp();
            right_i1 = getNewRegister(false, 0, false, false, -2, 0);
            printllvmCompare(right_i1, right_i32, Register(0), "NEQ");

            result = getNewRegister(false, 0, false, false, -2, 0);
            printllvmcalculate(result, left_i1, right_i1, "AND");

            printllvmBranch(result, next_and_label0, false_label);
            if (flag) {
                printllvmLabel(next_and_label0);
            }
            left_i1 = result;
        }
        return left_i1;
    } else {
        return left_i1;
    }
}

Register generateEqExp() {
    Type type = Type(-2, 0);
    Register left = generateRelExp();
    Register right = Register(0);
    Register result = Register(0);

    if (isToken("EQL", false, false, __FUNCTION__) || isToken("NEQ", false, false, __FUNCTION__)) {
        printParserResult("<EqExp>");
        while(isToken("EQL", true, false, __FUNCTION__) || isToken("NEQ", true, false, __FUNCTION__)) {
            string op = (iter-1)->getType();
            right = generateRelExp();
            result = getNewRegister(false,0,false, false, type.id, type.dim);
            printllvmCompare(result, left, right, op);
            if (result.type.id == -2) {
                result = printllvmZext(result);
            }
            if (result.hasValue) {
                return result;
            }
            left = result;
        }
        return result;
    } else {
        return left;
    }
}

Register generateRelExp() {
    Type type = Type(-2,0);
    Register left = generateAddExp();
    Register right = Register(0);
    Register result = Register(0);

    if (isToken("LSS", false, false, __FUNCTION__) || isToken("GRE", false, false, __FUNCTION__)
        || isToken("LEQ", false, false, __FUNCTION__) || isToken("GEQ", false, false, __FUNCTION__)) {
        while (isToken("LSS", true, false, __FUNCTION__) || isToken("GRE", true, false, __FUNCTION__)
               || isToken("LEQ", true, false, __FUNCTION__) || isToken("GEQ", true, false, __FUNCTION__)) {
            string op = (iter-1)->getType();
            right = generateAddExp();
            result = getNewRegister(false, 0, false, false, type.id, type.dim);
            printllvmCompare(result, left, right, op);
            if (result.type.id == -2) {
                result = printllvmZext(result);
            }
            if (result.hasValue) {
                return result;
            }
            left = result;
        }
        return result;
    } else {
        return left;
    }
}


bool isLastLAndExp() {
    int i = 0;
    bool flag = true;
    while (iter + i != TokenList.end()) {
        if ((iter+i)->getType() == "RPARENT" || (iter+i)->getType()=="SEMICN") {
            break;
        }
        if ((iter+i)->getType() == "OR") {
            flag = false;
        }
        i++;
    }
    return flag;
}

bool isLastEqExp() {
    int i = 0;
    bool flag = true;
    while (iter+i != TokenList.end()) {
        if ((iter+i)->getType() == "RPARENT" || (iter+i)->getType() == "OR" || (iter+i)->getType()=="SEMICN") {
            break;
        }
        if ((iter+i)->getType() == "AND") {
            flag = false;
        }
        i++;
    }
    return flag;
}


void generateForstmt() {
    Register for_begin = getNewRegister(false, 0, false, false, -3, 0);
    Register stmt_end = getNewRegister(false, 0, false, false, -3, 0);
    Register stmt_begin = getNewRegister(false, 0, false, false, -3, 0);
    Register for_end = getNewRegister(false, 0, false, false, -3, 0);
    currentStmtEnd.push_back(stmt_end.name);
    currentForEnd.push_back(for_end.name);

    if (isToken("FORTK", true, true, __FUNCTION__)) {
        if (isToken("LPARENT", true, true, __FUNCTION__)) {
            if (isToken("SEMICN", true, false, __FUNCTION__)) {
                // 没有ForStmt1
            } else {
                generateForStmt();
                if (isToken("SEMICN", true, true, __FUNCTION__)) {
                } else {
                    assert(0);
                }
            }
            printllvmBranch(for_begin);

            if (isToken("SEMICN", true, false, __FUNCTION__)) {
                printllvmLabel(for_begin);
                printllvmBranch(stmt_begin);
            } else {

                printllvmLabel(for_begin);
                generateCond(stmt_begin, for_end);

                if (isToken("SEMICN", true, true, __FUNCTION__)) {
                } else {
                    assert(0);
                }
            }
            printllvmLabel(stmt_end);
            if (isToken("RPARENT", true, false, __FUNCTION__)) {
                // 没有 ForStmt2
            } else {
                generateForStmt();

                if (isToken("RPARENT", true, true, __FUNCTION__)) {
                } else {
                    assert(0);
                }
            }
            printllvmBranch(for_begin);

            printllvmLabel(stmt_begin);
            generateStmt();

            printllvmBranch(stmt_end);
            printllvmLabel(for_end);

        } else {
            assert(0);
        }
    } else {
        assert(0);
    }
    currentStmtEnd.pop_back();
    currentForEnd.pop_back();
}

void generateForStmt() {
    Register ptr = generateLVal(true);
    Register result = Register(0);
    if (isToken("ASSIGN", true, true, __FUNCTION__)) {
        result = generateExp();
    } else {
        assert(0);
    }
    storeRegister(ptr, result);
}

void generateBreakStmt() {
    if (isToken("BREAKTK", true, true, __FUNCTION__)) {
        if (isToken("SEMICN", true, true, __FUNCTION__)) {
            printllvmBranch(currentForEnd.back());
        } else {
            assert(0);
        }
    } else {
        assert(0);
    }
}

void generateContinueStmt() {
    if (isToken("CONTINUETK", true, true, __FUNCTION__)) {
        if (isToken("SEMICN", true, true, __FUNCTION__)) {
            printllvmBranch(currentStmtEnd.back());
        } else {
            assert(0);
        }
    } else {
        assert(0);
    }
}