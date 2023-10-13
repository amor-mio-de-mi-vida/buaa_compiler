//
// Created by 杨贺森 on 2023/10/1.
//
#ifndef HW2_TOKEN_H
#define HW2_TOKEN_H

#include <iostream>
#include <vector>

using namespace std;

enum Tokens {
    IDENFR,
    INTCON,
    STRCON,
    MAINTK,
    CONSTTK,
    INTTK,
    BREAKTK,
    CONTINUETK,
    IFTK,
    ELSETK,
    NOT,
    AND,
    OR,
    FORTK,
    GETINTTK,
    PRINTFTK,
    RETURNTK,
    PLUS,
    MINU,
    VOIDTK,
    MULT,
    DIV,
    MOD,
    LSS,
    LEQ,
    GRE,
    GEQ,
    EQL,
    NEQ,
    ASSIGN,
    SEMICN,
    COMMA,
    LPARENT,
    RPARENT,
    LBRACK,
    RBRACK,
    LBRACE,
    RBRACE
};

class Token {
    std::string name;
    std::string type;
    int rowNo;
    int colNo;
public:
    Token(const std::string& name, const std::string& type, int rowNo, int colNo);
    std::string getName() {
        return this->name;
    }
    std::string getType() {
        return this->type;
    }
    int getRowNo() const {
        return this->rowNo;
    }
    int getColNo() const {
        return this->colNo;
    }
};
bool startsWith(const std::string& str, const std::string& prefix);
bool isSpace(const char ch);
void getSTRCON(const std::string& str);
void getNOT();
void getAND();
void getOR();
void getPLUS();
void getMINU();
void getMULT();
void getDIV();
void getMOD();
void getLSS();
void getLEQ();
void getGRE();
void getGEQ();
void getEQL();
void getNEQ();
void getASSIGN();
void getSEMICN();
void getCOMMA();
void getLPARENT();
void getRPARENT();
void getLBRACK();
void getRBRACK();
void getLBRACE();
void getRBRACE();
void getIDENFR(const std::string& str);
void getINTCON(const std::string& str);
void getMAINTK();
void getCONSTTK();
void getINTTK();
void getBREAKTK();
void getCONTINUETK();
void getIFTK();
void getELSETK();
void getFORTK();
void getGETINTTK();
void getPRINTFTK();
void getRETURNTK();
void getVOIDTK();
void getTokens(const std::string& str, size_t strLen);
void getNextToken();
Token* preReadToken(int i);

void printTokens();

#endif //HW2_TOKEN_H
