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
bool preReadExp();
bool isToken(const char *str, bool getNext, bool errorMessage, const char* funcname);

std::string parseIdent();
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
void parseFuncFParams(const std::string& funcname);
int parseFuncFParam();
void parseBlock();
void parseBlockItem();
void parseStmt();
void parseForStmt();
int parseExp();
void parseCond();
int parseLVal();
int parsePrimaryExp();
void parseNumber();
int parseUnaryExp();
void parseUnaryOp();
int parseFuncRParams(int row, const std::string& funcname);
int parseMulExp();
int parseAddExp();
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
int parseFuncCall();

void printParserResult(const char* string);

#endif //HW2_PARSER_H
