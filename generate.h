//
// Created by 杨贺森 on 2023/9/17.
//

#ifndef COMPILER_GENERATE_H
#define COMPILER_GENERATE_H

void gen_CompUnit();
void gen_Decl();
void gen_ConstDecl();
void gen_Btype();
void gen_ConstDef();
void gen_ConstInitVal();
void gen_VarDecl();
void gen_VarDef();
void gen_InitVal();
void gen_FuncDef();
void gen_MainFuncDef();
void gen_FuncType();
void gen_FuncFParams();
void gen_FuncFParam();
void gen_Block();
void gen_BlockItem();
void gen_Stmt();
void gen_ForStmt();
void gen_Exp();
void gen_Cond();
void gen_LVal();
void gen_PrimaryExp();
void gen_Number();
void gen_UnaryExp();
void gen_UnaryOp();
void gen_FuncRParams();
void gen_MulExp();
void gen_AddExp();
void gen_RelExp();
void gen_EqExp();
void gen_LAndExp();
void gen_LOrExp();
void gen_ConstExp();
void gen_Ident();
void gen_digit();
void gen_identifier_nondigit();
void gen_decimalConst();
void gen_nonzeroDigit();
void gen_IntConst();

void write(const char* str);

#endif //COMPILER_GENERATE_H
