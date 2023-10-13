//
// Created by 杨贺森 on 2023/9/27.
//

#ifndef HW2_PARSER_H
#define HW2_PARSER_H
#include "lexer.h"
#include <utility>
#include <vector>
using namespace std;

void parseBType();
bool isToken(const char *str, bool errorMessage);
void setNull(void** ptr);

void parseIdent();
void parseCompUnit();
void parseDecl();
void parseConstDecl();
void parseConstDef();
void parseConstInitVal();
void parseVarDecl();
void parseVarDef();
void parseInitVal();
void parseFuncDef();
void parseMainFuncDef();
void parseFuncType();
void parseFuncFParams();
void parseFuncFParam();
void parseBlock();
void parseBlockItem();
void parseStmt();
void parseForStmt();
void parseExp();
void parseCond();
void parseLVal();
void parsePrimaryExp();
void parseNumber();
void parseUnaryExp();
void parseUnaryOp();
void parseFuncRParams();
void parseMulExp();
void parseAddExp();
void parseRelExp();
void parseEqExp();
void parseLAndExp();
void parseLOrExp();
void parseConstExp();

void parseAssignStmt();
void parseExpStmt();
void parseBlockStmt();
void parseIfStmt();
void parseForstmt();
void parseBreakStmt();
void parseContinueStmt();
void parseReturnStmt();
void parseGetintStmt();
void parsePrintfStmt();

void printParserResult(const char* string);

#endif //HW2_PARSER_H
