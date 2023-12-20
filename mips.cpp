//
// Created by 杨贺森 on 2023/12/6.
//

#include "mips.h"
#include <iostream>
#include <fstream>
#include <cstring>
#include "ir.h"

using namespace std;

extern int printfllvmstring;
extern unordered_map<string, string> printfllvmstrings;
extern unordered_map<string, GlobalValue> MIPSglobalValues;
extern unordered_map<string, Function> MIPSfunctions;
extern bool mips;
extern bool debug;
ofstream fmips;

void generateMIPS() {
    const char* fileMipsPath = "./mips.txt";
    fmips.open(fileMipsPath, ios::out);
    printMIPSData();
    printMIPSText();
    fmips.close();
}

void printMIPSData() {
    printmips(".data:");
    printMIPSGlobal();
    printMIPSString();
    printmips("\n");
}

void printMIPSText() {
    printmips(".text:");
    for (auto & MIPSfunction : MIPSfunctions) {
        printMIPSFunction(MIPSfunction.second);
    }
}

void printMIPSFunction(const Function& function) {
    for (const auto & basicBlock : function.basicBlocks) {
        printMIPSBasicBlock(basicBlock);
    }
}

void printMIPSBasicBlock(const BasicBlock& basicBlock) {
    for (auto instruction : basicBlock.instructions) {
        printMIPSInstruction(instruction);
    }
}

void printMIPSInstruction(Instruction* instruction) {
    const char* type_name = typeid(instruction).name();
    if (strcmp(type_name, "AllocInst") != 0) {
        printMIPSAllocInst((AllocInst *) instruction);
    } else if (strcmp(type_name, "LoadInst") != 0) {
        printMIPSLoadInst((LoadInst *) instruction);
    } else if (strcmp(type_name, "StoreInst") != 0) {
        printMIPSStoreInst((StoreInst *) instruction);
    } else if (strcmp(type_name, "GetElementPtrInst") != 0) {
        printMIPSGetElementPtrInst((GetElementPtrInst *) instruction);
    } else if (strcmp(type_name, "CalculateInst") != 0) {
        printMIPSCalculateInst((CalculateInst *) instruction);
    } else if (strcmp(type_name, "CallInst") != 0) {
        printMIPSCallInst((CallInst *) instruction);
    } else if (strcmp(type_name, "BranchInst") != 0) {
        printMIPSBranchInst((BranchInst *) instruction);
    } else if (strcmp(type_name, "ZextInst") != 0) {
        printMIPSZextInst((ZextInst *) instruction);
    } else if (strcmp(type_name, "ReturnInst") != 0) {
        printMIPSReturnInst((ReturnInst *) instruction);
    }
}

void printMIPSAllocInst(AllocInst* instruction) {

}

void printMIPSLoadInst(LoadInst* instruction) {

}

void printMIPSStoreInst(StoreInst* instruction) {

}

void printMIPSGetElementPtrInst(GetElementPtrInst* instruction) {

}

void printMIPSCalculateInst(CalculateInst* instruction) {

}

void printMIPSCallInst(CallInst* instruction) {

}

void printMIPSBranchInst(BranchInst* instruction) {

}

void printMIPSZextInst(ZextInst* instruction) {

}

void printMIPSReturnInst(ReturnInst* instruction) {

}

void printMIPSString() {
    int pos = 0;
    for (; pos < printfllvmstring; pos++) {
        string name = "printllvmprintfasciiz" + to_string(pos);
        string str = name + ": .asciiz \"" + printfllvmstrings.at(name) + "\"";
        printmips(str);
    }
}

void printMIPSGlobal() {
    for (auto & MIPSglobalValue : MIPSglobalValues) {
        string str = MIPSglobalValue.first + ": .word ";
        for (auto iter0 = MIPSglobalValue.second.value.end() - 1; iter0 != MIPSglobalValue.second.value.begin(); iter0--) {
            str += to_string(iter0->value) + ",";
        }
        str += to_string(MIPSglobalValue.second.value.begin()->value);
        printmips(str);
    }
}

void printmips(const string& str) {
    if (mips) {
        if (debug) {
            cout << str << endl;
        }
        fmips << str << endl;
    }
}