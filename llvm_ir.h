//
// Created by 杨贺森 on 2023/11/14.
//

#ifndef BUAA_COMPILER_LLVM_IR_H
#define BUAA_COMPILER_LLVM_IR_H
#include "utils.h"

Register getNewRegister(int num);
Register getNewRegister(bool hasValue, int value, bool isGlobal, bool isConst, int type_id, int dim);
Register allocRegister(const Type& type);
void storeInitial(const Register& basePtr, vector<Register> value);
void storeRegister(const Register& ptr, const Register& value);
void loadRegister(const Register& ptr, Register& result);

void generateCompUnit();
void generateMainFuncDef();
void generateBlock(bool funcBlock);
void generateBlockItem();
void generateStmt();
void generateReturnStmt();
Register generateExp();
Register generateAddExp();
Register generateMulExp();
Register generateUnaryExp();
Register generatePrimaryExp();
Register generateNumber();
void generateDecl();
void generateConstDecl();
int getType();
void generateConstDef(int type_id);
vector<Register> generateConstInitVal();
Register generateConstExp();
void generateVarDecl();
void generateVarDef(int type_id);
vector<Register> generateInitVal();
Register generateLVal(bool left);
void generateBlockStmt();
void generateGetintStmt();
void generatePrintfStmt();
void generateFuncDef();
Register generateFuncFParam();
void allocRegister(const Register& ptr);
void generateExpStmt();
void generateAssignStmt();
vector<Register> generateFuncRParams();
Register generateFuncCall();
void generateIfStmt();
void generateCond(const Register& true_label, const Register& false_label);
void generateLOrExp(const Register& true_label, const Register& false_label);
void generateLAndExp(const Register& true_label,  const Register& false_label);
Register generateEqExp();
Register generateRelExp();
void generateForstmt();
void generateForStmt();
void generateBreakStmt();
void generateContinueStmt();

bool isLastLAndExp();
bool isLastEqExp();

#endif //BUAA_COMPILER_LLVM_IR_H
