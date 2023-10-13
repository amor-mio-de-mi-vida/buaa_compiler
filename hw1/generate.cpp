//
// Created by 杨贺森 on 2023/9/17.
//
#include <iostream>
#include <fstream>
#include "generate.h"
using namespace std;
void gen_FormatString();

// 编译单元
void gen_CompUnit() {
    int seed = rand() % 10;
    for (int i = 0; i < seed; i++) {
        gen_Decl();
    }
    write(" ");
    int seed1 = rand() % 10;
    for (int i = 0; i < seed1; i++) {
        gen_FuncDef();
    }
    gen_MainFuncDef();
}

// 声明
void gen_Decl() {
    int seed = rand() % 2;
    if (seed == 1) {
        gen_ConstDecl();
    } else {
        gen_VarDecl();
    }
}

// 常量声明
void gen_ConstDecl() {
    write("const ");
    gen_Btype();
    gen_ConstDef();
    int seed = rand() % 10;
    for (int i = 0; i < seed; i++) {
        write(",");
        gen_ConstDef();
    }
    write(";\n");
}

void gen_Btype() {
    write("int ");
}

void gen_ConstDef() {
    gen_Ident();
    int seed = rand() % 3;
    for (int i = 0; i < seed; i++) {
        write("[");
        gen_ConstExp();
        write("]");
    }
    write("=");
    gen_ConstInitVal();

}

void gen_ConstInitVal() {
    int seed = rand() % 2;
    if (seed == 0) {
        gen_ConstExp();
    } else {
        write("{");
        int seed1 = rand() % 2;
        if (seed1 == 1) {
            gen_ConstInitVal();
            int seed2 = rand() % 3;
            for (int i = 0; i < seed2; i++) {
                write(",");
                gen_ConstInitVal();
            }
        }
        write("}");
    }
}

void gen_VarDecl() {
    gen_Btype();
    gen_VarDef();
    int seed = rand() % 5;
    for (int i = 0 ;i < 5; i++) {
        write(",");
        gen_VarDef();
    }
    write(";\n");
}

void gen_VarDef() {
    int seed = rand()% 2;
    if (seed == 1) {
        gen_Ident();
        int seed1 = rand() % 3;
        for (int i = 0; i < seed1; i++) {
            write("[");
            gen_ConstExp();
            write("]");
        }
    } else {
        gen_Ident();
        int seed2 = rand() % 3;
        for (int i = 0; i < seed2; i++) {
            gen_ConstExp();
        }
        write("=");
        gen_InitVal();
    }
}

void gen_InitVal() {
    int seed = rand() % 2;
    if (seed == 1) {
        gen_Exp();
    } else {
        write("{");
        int seed1 = rand() % 2;
        if (seed1 == 1) {
            gen_InitVal();
            int seed2 = rand() % 3;
            for (int i = 0; i < seed2; i++) {
                write(",");
                gen_InitVal();
            }
        }
        write("}");
    }
}

void gen_FuncDef() {
    gen_FuncType();
    write(" ");
    gen_Ident();
    write(" ");
    write("(");
    int seed = rand() % 2;
    if (seed == 1) {
        gen_FuncFParams();
    }
    write(")");
    gen_Block();
}

void gen_MainFuncDef() {
    write("int main ( ) ");
    gen_Block();
}

void gen_FuncType() {
    int seed = rand() % 2;
    if (seed == 1) {
        write("void");
    } else {
        write("int");
    }
}

void gen_FuncFParams() {
    gen_FuncFParam();
    write(" ");
    int seed = rand() % 5;
    for (int i = 0; i < seed; i++) {
        write(", ");
        gen_FuncFParam();
    }
}

void gen_FuncFParam() {
    gen_Btype();
    write(" ");
    gen_Ident();
    write(" ");
    int seed = rand() % 2;
    if (seed == 1) {
        write("[ ]");
        int seed2 = rand() % 2;
        for (int i = 0; i < seed2; i++) {
            write("[ ");
            gen_ConstExp();
            write(" ]");
        }
    }
}

void gen_Block() {
    write("{ ");
    int seed = rand() % 5;
    for (int i = 0; i < seed; i++) {
        gen_BlockItem();
    }
    write(" }");
}

void gen_BlockItem() {
    int seed = rand() % 2;
    if (seed == 1) {
        gen_Decl();
    } else {
        gen_Stmt();
    }
}

void gen_Stmt() {
    int seed = rand() % 10;
    switch (seed) {
        case 0: {
            gen_LVal();
            write(" = ");
            gen_Exp();
            write(";\n");
            break;
        }
        case 1: {
            int seed1 = rand() % 2;
            if (seed1 == 1) {
                gen_Exp();
            }
            write(" ;\n");
            break;
        }
        case 2: {
            gen_Block();
            break;
        }
        case 3: {
            write("if( ");
            gen_Cond();
            write(") ");
            gen_Stmt();
            int seed2 = rand() % 2;
            if (seed2 == 1) {
                write(" else ");
                gen_Stmt();
            }
            break;
        }
        case 4: {
            write(" for ( ");
            int seed3 = rand() % 2;
            if (seed3 == 1) {
                gen_ForStmt();
            }
            write(" ; ");
            int seed4 = rand() % 2;
            if (seed4 == 1) {
                gen_Cond();
            }
            write(" ; ");
            int seed5 = rand() % 2;
            if (seed5 == 1) {
                gen_ForStmt();
            }
            write(" ) ");
            gen_Stmt();
            break;
        }
        case 5: {
            write(" break ;\n");
            break;
        }
        case 6: {
            write("continue ;\n");
            break;
        }
        case 7: {
            write("return ");
            int seed6 = rand() % 2;
            if (seed6 == 1) {
                gen_Exp();
            }
            write(" ;\n");
            break;
        }
        case 8: {
            // TODO INPUT
            gen_LVal();
            write(" = getint();\n");
            break;
        }
        case 9: {
            // TODO OUTPUT
            write(" printf(");
            gen_FormatString();
            int seed7 = rand() % 5;
            for (int i = 0; i < seed7; i++) {
                write(",");
                gen_Exp();
            }
            write(");\n");
        }
        default:
            break;
    }
}

void gen_ForStmt() {
    gen_LVal();
    write(" = ");
    gen_Exp();
}

void gen_Exp() {
    gen_AddExp();
}

void gen_Cond() {
    gen_LOrExp();
}

void gen_LVal() {
    gen_Ident();
    int seed = rand() % 3;
    for (int i = 0; i < seed; i++) {
        write("[ ");
        gen_Exp();
        write(" ]");
    }
}

void gen_PrimaryExp() {
    int seed = rand() % 3;
    if (seed == 0) {
        write("( ");
        gen_Exp();
        write(" )");
    } else if (seed == 1) {
        gen_LVal();
    } else {
        gen_Number();
    }
}

void gen_Number() {
    gen_IntConst();
}

void gen_UnaryExp() {
    int seed = rand() % 3;
    if (seed == 0) {
        gen_PrimaryExp();
    } else if (seed == 1) {
        gen_Ident();
        write(" ( ");
        int seed1 = rand() % 2;
        if (seed1 == 1) {
            gen_FuncRParams();
        }
        write(" )");
    } else {
        gen_UnaryOp();
        write(" ");
        gen_UnaryExp();
    }
}

void gen_UnaryOp() {
    int seed = rand() % 3;
    if (seed == 0) {
        write(" + ");
    } else if (seed == 1) {
        write(" - ");
    } else {
        write(" ! ");
    }
}

void gen_FuncRParams() {
    gen_Exp();
    write(" ");
    int seed = rand() % 5;
    for (int i = 0; i < seed; i++) {
        write(" , ");
        gen_Exp();
    }
}

void gen_MulExp() {
    int seed = rand() % 2;
    if (seed == 1) {
        gen_UnaryExp();
    } else {
        gen_MulExp();
        int seed1 = rand() % 3;
        if (seed1 == 0) {
            write(" * ");
        } else if (seed1 == 1) {
            write(" / ");
        } else {
            write(" % ");
        }
        gen_UnaryExp();
    }
}

void gen_AddExp() {
    int seed = rand() % 2;
    if (seed == 1) {
        gen_MulExp();
    } else {
        gen_AddExp();
        int seed1 = rand() % 2;
        if (seed1 == 1) {
            write(" + ");
        } else {
            write(" - ");
        }
        gen_MulExp();
    }
}

void gen_RelExp() {
    int seed = rand() % 2;
    if (seed == 1) {
        gen_AddExp();
    } else {
        gen_RelExp();
        int seed1 = rand() % 4;
        if (seed1 == 0) {
            write(" < ");
        } else if (seed1 == 1) {
            write(" > ");
        } else if (seed1 == 2) {
            write(" <= ");
        } else {
            write(" >= ");
        }
        gen_AddExp();
    }
}

void gen_EqExp() {
    int seed = rand() % 2;
    if (seed == 1) {
        gen_RelExp();
    } else {
        gen_EqExp();
        int seed1 = rand() % 2;
        if (seed1 == 0) {
            write(" == ");
        } else {
            write(" != ");
        }
        gen_RelExp();
    }
}

void gen_LAndExp() {
    int seed = rand() % 2;
    if (seed == 1) {
        gen_EqExp();
    } else {
        gen_LAndExp();
        write(" && ");
        gen_EqExp();
    }
}

void gen_LOrExp() {
    int seed = rand() % 2;
    if (seed == 0) {
        gen_LAndExp();
    } else {
        gen_LOrExp();
        write(" || ");
        gen_LAndExp();
    }
}

void gen_ConstExp() {
    gen_AddExp();
}

void gen_Ident() {
    int seed = rand() % 3;
    if (seed == 0) {
        gen_identifier_nondigit();
    } else if (seed == 1){
        gen_Ident();
        gen_identifier_nondigit();
    } else {
        gen_Ident();
        gen_digit();
    }
}

void gen_identifier_nondigit() {
    char alpha[53][2] = {"a","b","c","d","e","f"
                         ,"g","h","i","j","k","l","m",
                         "n","o","p","q","r","s","t",
                         "u","v","w","x","y","z","A",
                         "B","C","D","E","F","G","H",
                         "I","J","K","L","M","N","O",
                         "P","Q","R","S","T","U","V",
                         "W","X","Y","Z","_"};
    int seed = rand() % 53;
    write(alpha[seed]);
}

void gen_digit() {
    char num[10][2] = {"0","1","2","3","4",
                       "5","6","7","8","9"};
    int seed = rand() % 10;
    write(num[seed]);
}

void gen_FormatString() {

}

void gen_IntConst() {
    int seed = rand() % 2;
    if (seed == 1) {
        gen_decimalConst();
    } else {
        write("0");
    }
}

void gen_decimalConst() {
    int seed = rand() % 2;
    if (seed == 1) {
        gen_nonzeroDigit();
    } else {
        gen_decimalConst();
        gen_digit();
    }
}

void gen_nonzeroDigit() {
    char num[9][2] = {"1","2","3","4","5","6"
                                          ,"7","8","9"};
    int seed = rand() % 9;
    write(num[seed]);
}

int main(int argc, char** argv) {
    gen_CompUnit();
    return 0;
}

void write(const char* str) {
    fstream fout;
    fout.open("test.txt", ios::out|ios::app);
    fout << str;
    cout << str << endl;
    fout.close();
}