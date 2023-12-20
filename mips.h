//
// Created by 杨贺森 on 2023/12/6.
//

#ifndef BUAA_COMPILER_MIPS_H
#define BUAA_COMPILER_MIPS_H
#include <iostream>
#include "ir.h"
using namespace std;

void generateMIPS();
void printMIPSData();
void printMIPSText();
void printMIPSGlobal();
void printMIPSString();
void printMIPSFunction(const Function& function);
void printMIPSBasicBlock(const BasicBlock& basicBlock);
void printMIPSInstruction(Instruction* instruction);
void printmips(const string& str);

void printMIPSAllocInst(AllocInst* instruction);
void printMIPSLoadInst(LoadInst* instruction);
void printMIPSStoreInst(StoreInst* instruction);
void printMIPSGetElementPtrInst(GetElementPtrInst* instruction);
void printMIPSCalculateInst(CalculateInst* instruction);
void printMIPSCallInst(CallInst* instruction);
void printMIPSBranchInst(BranchInst* instruction);
void printMIPSZextInst(ZextInst* instruction);
void printMIPSReturnInst(ReturnInst* instruction);

#endif //BUAA_COMPILER_MIPS_H
