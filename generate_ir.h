//
// Created by 杨贺森 on 2023/11/16.
//

#ifndef BUAA_COMPILER_GENERATE_IR_H
#define BUAA_COMPILER_GENERATE_IR_H
#include <iostream>
#include "utils.h"
using namespace std;

void generateIR();
void init();
void printllvm(const string& str);
int calculate(int left, int right, const string& op);
int compare(int left, int right, const string& op);
void printllvmcalculate(Register& result, const Register& left, const Register& right, const string& op);
void printllvmCompare(Register& result, const Register& left, const Register& right, const string& op);
void printllvmFuncDef(int retype, const string& name, const vector<Register>& params);
void printllvmReturnStmt(int retype, const Register& name);
void printllvmGlobalConst(const string& name, const Type& type);
void printllvmGlobalAssign(const string& name, const Type& type, const vector<Register>& value);
void printllvmGlobalVar(const string& name, const Type& type);
void printllvmGetintStmt(const Register& left);
void printllvmPrintfStmt(const string& str, const vector<Register>& params);
vector<Register> generateFuncFParams();
void printllvmCallFunc(Register& result, const string& name, const vector<Register>& params);
Register printllvmGetElementPtr(const Register& basePtr);
Register printllvmGetElementPtr(const Register& basePtr, const Register& value_i);
Register printllvmGetElementPtr(const Register& basePtr, const Register& value_i, const Register& value_j);
void printllvmBranch(const string& name);
void printllvmBranch(const Register& label);
void printllvmBranch(const Register& cond, const Register& stmt1, const Register& stmt2);
void printllvmLabel(const Register& label);
// Register printllvmTruncCond(const Register& cond);
Register printllvmZext(Register& right);

#endif //BUAA_COMPILER_GENERATE_IR_H
