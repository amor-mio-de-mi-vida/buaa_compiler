//
// Created by 杨贺森 on 2023/10/1.
//
#include "token.h"
#include "error.h"
#include <fstream>

extern bool comment;
std::vector<Token> TokenList;

Token::Token(const std::string &name, const std::string &type) {
    this->name = name;
    this->type = type;
}

bool startsWith(const std::string& str, const std::string& prefix) {
    return (str.rfind(prefix, 0) == 0);
}

bool isIdentifier(const char ch) {
    return (ch <= 'z' && ch >= 'a') || (ch <= 'Z' && ch >= 'A') || ch == '_';
}

bool isDigit(const char ch) {
    return ch <= '9' && ch >= '0';
}

void getTokens(const std::string& str, size_t strLen) {
    while(colNo < strLen) {
        // 处理空白字符
        if (std::string(" \t\n").find(str.at(colNo)) != std::string::npos) {
            colNo++;
            continue;
        } // 处理不换行注释
        else if (startsWith(str.substr(colNo, strLen - colNo),"//")) {
            break;
        }
        //处理可换行注释
        else if (startsWith(str.substr(colNo, strLen - colNo), "/*")) {
            colNo += 2;
            size_t r = str.substr(colNo, strLen - colNo).find("*/");
            if (r != std::string::npos) {   // 注释结束符在本行时的情况。
                colNo = r + 2 + colNo;  // 小心溢出
                continue;
            } else {    // 注释结束符不在本行时的情况
                comment = true;
                break;
            }
        }
        // 处理FormatString
        else if (startsWith(str.substr(colNo, strLen - colNo), "\"")) {
            getSTRCON(str.substr(colNo, strLen - colNo));
        }
        else if (startsWith(str.substr(colNo, strLen - colNo),"!=")){
            getNEQ();
        }
        else if (startsWith(str.substr(colNo, strLen - colNo), "&&")) {
            getAND();
        }
        else if (startsWith(str.substr(colNo, strLen - colNo), "||")) {
            getOR();
        }
        else if (startsWith(str.substr(colNo, strLen - colNo), "==")) {
            getEQL();
        }
        else if (startsWith(str.substr(colNo, strLen - colNo), ">=")) {
            getGEQ();
        }
        else if (startsWith(str.substr(colNo, strLen - colNo), "<=")) {
            getLEQ();
        }
        else if (startsWith(str.substr(colNo, strLen - colNo), "!")) {
            getNOT();
        }
        else if (startsWith(str.substr(colNo, strLen - colNo), "+")) {
            getPLUS();
        }
        else if (startsWith(str.substr(colNo, strLen - colNo), "-")) {
            getMINU();
        }
        else if (startsWith(str.substr(colNo, strLen - colNo), "*")) {
            getMULT();
        }
        else if (startsWith(str.substr(colNo, strLen - colNo), "/")) {
            getDIV();
        }
        else if (startsWith(str.substr(colNo, strLen - colNo), "%")) {
            getMOD();
        }
        else if (startsWith(str.substr(colNo, strLen - colNo), "<")) {
            getLSS();
        }
        else if (startsWith(str.substr(colNo, strLen - colNo), ">")) {
            getGRE();
        }
        else if (startsWith(str.substr(colNo, strLen - colNo), "=")) {
            getASSIGN();
        }
        else if (startsWith(str.substr(colNo, strLen - colNo), ";")) {
            getSEMICN();
        }
        else if (startsWith(str.substr(colNo, strLen - colNo), ",")) {
            getCOMMA();
        }
        else if (startsWith(str.substr(colNo, strLen - colNo), "(")) {
            getLPARENT();
        }
        else if (startsWith(str.substr(colNo, strLen - colNo), ")")) {
            getRPARENT();
        } else if (startsWith(str.substr(colNo, strLen - colNo), "[")) {
            getLBRACK();
        } else if (startsWith(str.substr(colNo, strLen - colNo), "]")) {
            getRBRACK();
        } else if (startsWith(str.substr(colNo, strLen - colNo), "{")) {
            getLBRACE();
        } else if (startsWith(str.substr(colNo, strLen - colNo), "}")) {
            getRBRACE();
        } else if (isDigit(str.at(colNo))) {
            getINTCON(str.substr(colNo, strLen - colNo));
        } else if (isIdentifier(str.at(colNo))) {
            getIDENFR(str.substr(colNo, strLen - colNo));
        } else {
            getErrorMessage("Unknown char");
        }
    }
}

void getIDENFR(const std::string& str) {
    int len = 0;
    while (len < str.length() && (isIdentifier(str.at(len)) || isDigit(str.at(len)))) {
        len++;
    }
    if (len >= str.length()) {
        getErrorMessage("expect \";\"");
    } // 可能会出错
    std::string name = str.substr(0,len);
    if (name == "main") {
        getMAINTK();
    } else if (name == "const") {
        getCONSTTK();
    } else if (name == "int") {
        getINTTK();
    } else if (name == "break") {
        getBREAKTK();
    } else if (name == "continue") {
        getCONTINUETK();
    } else if (name == "if") {
        getIFTK();
    } else if (name == "else") {
        getELSETK();
    } else if (name == "for") {
        getFORTK();
    } else if (name == "getint") {
        getGETINTTK();
    } else if (name == "printf") {
        getPRINTFTK();
    } else if (name == "return") {
        getRETURNTK();
    } else if (name == "void") {
        getVOIDTK();
    } else {
        Token token = *new Token(name, "IDENFR");
        colNo += len;
        TokenList.push_back(token);
    }
}

void getINTCON(const std::string& str){
    int len = 0;
    while (len < str.length() && (isDigit(str.at(len)))) {
        len++;
    }
    if (len >= str.length()) {
        getErrorMessage("expect \";\"");
    } // 可能会出错
    std::string name = str.substr(0, len);
    Token token = *new Token(name, "INTCON");
    colNo += len;
    TokenList.push_back(token);
}

void getSTRCON(const std::string& str) {
    int len = 1;
    while (len < str.length() && str.at(len) != '\"') {
        len ++;
    }
    if (len >= str.length()) {
        getErrorMessage("expect \"");
    } // 可能会出错
    std::string name = str.substr(0, len+1);
    Token token = *new Token(name, "STRCON");
    colNo += len + 1;
    TokenList.push_back(token);
}

void getMAINTK() {
    Token token = *new Token("main", "MAINTK");
    colNo += 4;
    TokenList.push_back(token);
}

void getCONSTTK() {
    Token token = *new Token("const", "CONSTTK");
    colNo += 5;
    TokenList.push_back(token);
}

void getINTTK() {
    Token token = *new Token("int", "INTTK");
    colNo += 3;
    TokenList.push_back(token);
}

void getBREAKTK() {
    Token token = *new Token("break", "BREAKTK");
    colNo += 5;
    TokenList.push_back(token);
}

void getCONTINUETK() {
    Token token = *new Token("continue", "CONTINUETK");
    colNo += 8;
    TokenList.push_back(token);
}

void getIFTK() {
    Token token = *new Token("if", "IFTK");
    colNo += 2;
    TokenList.push_back(token);
}

void getELSETK() {
    Token token = *new Token("else", "ELSETK");
    colNo += 4;
    TokenList.push_back(token);
}

void getNOT() {
    Token token = *new Token("!", "NOT");
    colNo += 1;
    TokenList.push_back(token);
}

void getAND() {
    Token token = *new Token("&&", "AND");
    colNo += 2;
    TokenList.push_back(token);
}

void getOR() {
    Token token = *new Token("||", "OR");
    colNo += 2;
    TokenList.push_back(token);
}

void getFORTK() {
    Token token = *new Token("for", "FORTK");
    colNo += 3;
    TokenList.push_back(token);
}

void getGETINTTK() {
    Token token = *new Token("getint", "GETINTTK");
    colNo += 6;
    TokenList.push_back(token);
}

void getPRINTFTK() {
    Token token = *new Token("printf", "PRINTFTK");
    colNo += 6;
    TokenList.push_back(token);
}

void getRETURNTK() {
    Token token = *new Token("return", "RETURNTK");
    colNo += 6;
    TokenList.push_back(token);
}

void getPLUS() {
    Token token = *new Token("+", "PLUS");
    colNo += 1;
    TokenList.push_back(token);
}

void getMINU() {
    Token token = *new Token("-", "MINU");
    colNo += 1;
    TokenList.push_back(token);
}

void getVOIDTK() {
    Token token = *new Token("void", "VOIDTK");
    colNo += 4;
    TokenList.push_back(token);
}

void getMULT() {
    Token token = *new Token("*", "MULT");
    colNo += 1;
    TokenList.push_back(token);
}

void getDIV() {
    Token token = *new Token("/", "DIV");
    colNo += 1;
    TokenList.push_back(token);
}

void getMOD() {
    Token token = *new Token("%", "MOD");
    colNo += 1;
    TokenList.push_back(token);
}

void getLSS() {
    Token token = *new Token("<", "LSS");
    colNo += 1;
    TokenList.push_back(token);
}

void getLEQ() {
    Token token = *new Token("<=", "LEQ");
    colNo += 2;
    TokenList.push_back(token);
}

void getGRE() {
    Token token = *new Token(">", "GRE");
    colNo += 1;
    TokenList.push_back(token);
}

void getGEQ() {
    Token token = *new Token(">=", "GEQ");
    colNo += 2;
    TokenList.push_back(token);
}

void getEQL() {
    Token token = *new Token("==", "EQL");
    colNo += 2;
    TokenList.push_back(token);
}

void getNEQ() {
    Token token = *new Token("!=", "NEQ");
    colNo += 2;
    TokenList.push_back(token);
}

void getASSIGN() {
    Token token = *new Token("=", "ASSIGN");
    colNo += 1;
    TokenList.push_back(token);
}

void getSEMICN() {
    Token token = *new Token(";", "SEMICN");
    colNo += 1;
    TokenList.push_back(token);
}

void getCOMMA() {
    Token token = *new Token(",", "COMMA");
    colNo += 1;
    TokenList.push_back(token);
}

void getLPARENT() {
    Token token = *new Token("(", "LPARENT");
    colNo += 1;
    TokenList.push_back(token);
}

void getRPARENT() {
    Token token = *new Token(")", "RPARENT");
    colNo += 1;
    TokenList.push_back(token);
}

void getLBRACK() {
    Token token = *new Token("[", "LBRACK");
    colNo += 1;
    TokenList.push_back(token);
}

void getRBRACK() {
    Token token = *new Token("]", "RBRACK");
    colNo += 1;
    TokenList.push_back(token);
}

void getLBRACE() {
    Token token = *new Token("{", "LBRACE");
    colNo += 1;
    TokenList.push_back(token);
}

void getRBRACE() {
    Token token = *new Token("}", "RBRACE");
    colNo += 1;
    TokenList.push_back(token);
}

void printTokens() {
    ofstream fout;
    fout.open("../output.txt", ios::app);

    auto it = TokenList.begin();
    while (it != TokenList.end()) {
        fout << it->type << " " << it->name << std::endl;
        it++;
    }
    fout.close();
}


