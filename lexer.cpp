//
// Created by 杨贺森 on 2023/10/1.
//
#include "lexer.h"
#include "error.h"
#include <fstream>

extern bool lexer;
extern bool comment;
extern int colNo;
extern int rowNo;
extern ofstream fout;
std::vector<Token> TokenList;
auto iter = TokenList.begin();

Token::Token(const std::string &name, const std::string &type, int rowNo, int colNo) {
    this->name = name;
    this->type = type;
    this->rowNo = rowNo;
    this->colNo = colNo;
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

bool isSpace(const char ch) {
    return ch == ' ' || ch == '\n' || ch == '\t' || ch == '\r';
}

void getTokens(const std::string& str, size_t strLen) {
    while(colNo < strLen) {
        // 处理空白字符
        if (isSpace(str.at(colNo))) {
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
                colNo = (int) (r + 2 + colNo);  // 小心溢出
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
            getLexerErrorMessage("Unknown char");
        }
    }
}

void getIDENFR(const std::string& str) {
    int len = 0;
    while (len < str.length() && (isIdentifier(str.at(len)) || isDigit(str.at(len)))) {
        len++;
    }
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
        Token token = Token(name, "IDENFR", rowNo, colNo);
        colNo += len;
        TokenList.push_back(token);
    }
}

void getINTCON(const std::string& str){
    int len = 0;
    while (len < str.length() && (isDigit(str.at(len)))) {
        len++;
    }
    std::string name = str.substr(0, len);
    Token token = Token(name, "INTCON", rowNo, colNo);
    colNo += len;
    TokenList.push_back(token);
}

void getSTRCON(const std::string& str) {
    int len = 1;
    while (len < str.length() && str.at(len) != '\"') {
        len ++;
    }
    if (len >= str.length()) {
        getLexerErrorMessage("expect \"");
    } // 可能会出错
    std::string name = str.substr(0, len+1);
    checkInvalidChar(name);
    Token token = Token(name, "STRCON", rowNo, colNo);
    colNo += len + 1;
    TokenList.push_back(token);
}

void getMAINTK() {
    Token token = Token("main", "MAINTK", rowNo, colNo);
    colNo += 4;
    TokenList.push_back(token);
}

void getCONSTTK() {
    Token token = Token("const", "CONSTTK", rowNo, colNo);
    colNo += 5;
    TokenList.push_back(token);
}

void getINTTK() {
    Token token = Token("int", "INTTK", rowNo, colNo);
    colNo += 3;
    TokenList.push_back(token);
}

void getBREAKTK() {
    Token token = Token("break", "BREAKTK", rowNo, colNo);
    colNo += 5;
    TokenList.push_back(token);
}

void getCONTINUETK() {
    Token token = Token("continue", "CONTINUETK", rowNo, colNo);
    colNo += 8;
    TokenList.push_back(token);
}

void getIFTK() {
    Token token = Token("if", "IFTK", rowNo, colNo);
    colNo += 2;
    TokenList.push_back(token);
}

void getELSETK() {
    Token token = Token("else", "ELSETK", rowNo, colNo);
    colNo += 4;
    TokenList.push_back(token);
}

void getNOT() {
    Token token = Token("!", "NOT", rowNo, colNo);
    colNo += 1;
    TokenList.push_back(token);
}

void getAND() {
    Token token = Token("&&", "AND", rowNo, colNo);
    colNo += 2;
    TokenList.push_back(token);
}

void getOR() {
    Token token = Token("||", "OR", rowNo, colNo);
    colNo += 2;
    TokenList.push_back(token);
}

void getFORTK() {
    Token token = Token("for", "FORTK", rowNo, colNo);
    colNo += 3;
    TokenList.push_back(token);
}

void getGETINTTK() {
    Token token = Token("getint", "GETINTTK", rowNo, colNo);
    colNo += 6;
    TokenList.push_back(token);
}

void getPRINTFTK() {
    Token token = Token("printf", "PRINTFTK", rowNo, colNo);
    colNo += 6;
    TokenList.push_back(token);
}

void getRETURNTK() {
    Token token = Token("return", "RETURNTK", rowNo, colNo);
    colNo += 6;
    TokenList.push_back(token);
}

void getPLUS() {
    Token token = Token("+", "PLUS", rowNo, colNo);
    colNo += 1;
    TokenList.push_back(token);
}

void getMINU() {
    Token token = Token("-", "MINU", rowNo, colNo);
    colNo += 1;
    TokenList.push_back(token);
}

void getVOIDTK() {
    Token token = Token("void", "VOIDTK", rowNo, colNo);
    colNo += 4;
    TokenList.push_back(token);
}

void getMULT() {
    Token token = Token("*", "MULT", rowNo, colNo);
    colNo += 1;
    TokenList.push_back(token);
}

void getDIV() {
    Token token = Token("/", "DIV", rowNo, colNo);
    colNo += 1;
    TokenList.push_back(token);
}

void getMOD() {
    Token token = Token("%", "MOD", rowNo, colNo);
    colNo += 1;
    TokenList.push_back(token);
}

void getLSS() {
    Token token = Token("<", "LSS", rowNo, colNo);
    colNo += 1;
    TokenList.push_back(token);
}

void getLEQ() {
    Token token = Token("<=", "LEQ", rowNo, colNo);
    colNo += 2;
    TokenList.push_back(token);
}

void getGRE() {
    Token token = Token(">", "GRE", rowNo, colNo);
    colNo += 1;
    TokenList.push_back(token);
}

void getGEQ() {
    Token token = Token(">=", "GEQ", rowNo, colNo);
    colNo += 2;
    TokenList.push_back(token);
}

void getEQL() {
    Token token = Token("==", "EQL", rowNo, colNo);
    colNo += 2;
    TokenList.push_back(token);
}

void getNEQ() {
    Token token = Token("!=", "NEQ", rowNo, colNo);
    colNo += 2;
    TokenList.push_back(token);
}

void getASSIGN() {
    Token token = Token("=", "ASSIGN", rowNo, colNo);
    colNo += 1;
    TokenList.push_back(token);
}

void getSEMICN() {
    Token token = Token(";", "SEMICN", rowNo, colNo);
    colNo += 1;
    TokenList.push_back(token);
}

void getCOMMA() {
    Token token = Token(",", "COMMA", rowNo, colNo);
    colNo += 1;
    TokenList.push_back(token);
}

void getLPARENT() {
    Token token = Token("(", "LPARENT", rowNo, colNo);
    colNo += 1;
    TokenList.push_back(token);
}

void getRPARENT() {
    Token token = Token(")", "RPARENT", rowNo, colNo);
    colNo += 1;
    TokenList.push_back(token);
}

void getLBRACK() {
    Token token = Token("[", "LBRACK", rowNo, colNo);
    colNo += 1;
    TokenList.push_back(token);
}

void getRBRACK() {
    Token token = Token("]", "RBRACK", rowNo, colNo);
    colNo += 1;
    TokenList.push_back(token);
}

void getLBRACE() {
    Token token = Token("{", "LBRACE", rowNo, colNo);
    colNo += 1;
    TokenList.push_back(token);
}

void getRBRACE() {
    Token token = Token("}", "RBRACE", rowNo, colNo);
    colNo += 1;
    TokenList.push_back(token);
}

void getNextToken() {
    if (iter != TokenList.end()) {
        iter++;
    }
}

Token* preReadToken(int i) {
    if ( iter+i != TokenList.end() ) {
        return &*(iter + i);
    } else {
        return nullptr;
    }
}

void printTokens() {
    if (lexer) {
        auto it = TokenList.begin();
        while (it != TokenList.end()) {
            fout << it->getType() << " " << it->getName() << std::endl;
            it++;
        }
        fout.close();
    }
}
