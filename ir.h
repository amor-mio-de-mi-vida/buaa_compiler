//
// Created by 杨贺森 on 2023/11/21.
//

#ifndef BUAA_COMPILER_IR_H
#define BUAA_COMPILER_IR_H
#include <iostream>
#include <utility>
#include "utils.h"

using namespace std;

class Value {
};

class Use {
};


class User : Value {
};

class Instruction : User {
};


class BasicBlock : Value {
public:
    string label;
    vector<Instruction*> instructions;
    explicit BasicBlock(const string& label) {
        this->label = label;
    }
    void print();
};

class Constant : User {
};

class GlobalValue : Constant {
public:
    string name;
    bool isConst;
    Type type = Type(0, 0);
    vector<Register> value;
    GlobalValue(const string& name, bool isConst, Type type, const vector<Register> &value) {
        this->name = name;
        this->isConst = isConst;
        this->type = std::move(type);
        this->value = value;
    }
    void print();
};

class GetElementPtrInst : Instruction {
public:
    Register result = Register(0);
    Type addrType = Type(0, 0);
    Register baseAddr = Register(0);
    vector<Register> index;
    GetElementPtrInst(const Register& result, const Type& addrType, const Register& baseAddr, const vector<Register>& index) {
        this->result = result;
        this->addrType = addrType;
        this->baseAddr = baseAddr;
        this->index = index;
    }
    void print();
};

class StoreInst : Instruction {
public:
    Register value = Register(0);
    Register addr = Register(0);
    StoreInst(const Register& value, const Register& addr) {
        this->value = value;
        this->addr = addr;
    }
    void print();
};

class LoadInst : Instruction {
public:
    Register value = Register(0);
    Register addr = Register(0);
    LoadInst(const Register& value, const Register& addr) {
        this->value = value;
        this->addr = addr;
    }
    void print();
};

class AllocInst : Instruction {
public:
    Register addr = Register(0);
    Type type = Type(0, 0);
    AllocInst(const Register& addr, const Type& type) {
        this->addr = addr;
        this->type = type;
    }
    void print();
};

class CalculateInst : Instruction {
public:
    Register result = Register(0);
    Register left = Register(0);
    Register right = Register(0);
    string op;
    CalculateInst(const Register& result, const Register& left, const Register& right, const string& op) {
        this->result = result;
        this->left = left;
        this->right = right;
        this->op = op;
    }
    void print();
};

class CallInst : Instruction {
public:
    string funcname;
    vector<Register> Rparams;
    CallInst(const string& funcname, const vector<Register>& Rparams) {
        this->funcname = funcname;
        this->Rparams = Rparams;
    }
    CallInst(const string& funcname) {
        this->funcname = funcname;
        this->Rparams = *new vector<Register>;
    }
    void print();
};

class BranchInst : Instruction {
public:
    Register cond = Register(0);
    Register trueLabel = Register(0);
    Register falseLabel = Register(0);
    BranchInst(const Register& cond, const Register& trueLabel, const Register& falseLabel) {
        this->cond = cond;
        this->trueLabel = trueLabel;
        this->falseLabel = falseLabel;
    }
    void print();
};

class BranchLabelInst : Instruction {
public:
    string label;
    BranchLabelInst(const string& label) {
        this->label = label;
    }
    void print();
};

class ZextInst : Instruction {
public:
    Register from = Register(0);
    Register to = Register(0);
    ZextInst(const Register& from, const Register& to) {
        this->from = from;
        this->to = to;
    }
    void print();
};

class ReturnInst : Instruction {
public:
    Register ret = Register(0);
    ReturnInst(const Register& ret) {
        this->ret = ret;
    }
    void print();
};

class Function : Value {
public:
    string name;
    vector<Register> Fparams;
    vector<BasicBlock> basicBlocks;
    explicit Function(const string& name) {
        this->name = name;
    }
    void print();
};

#endif //BUAA_COMPILER_IR_H
