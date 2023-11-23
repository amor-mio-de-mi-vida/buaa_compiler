//
// Created by 杨贺森 on 2023/9/27.
//
#include <fstream>
#include "lexer.h"
#include "parser.h"
#include "error.h"
#include "utils.h"
#include <cassert>

extern bool parser;
extern bool error;
extern std::vector<Token>::iterator iter;
extern bool ir;
extern std::vector<Token> TokenList;
extern ofstream fout;
int isCircle = 0; // 错误处理判断是否在解析循环块
int currentFuncReturnType;

bool preReadExp() {
    if (isToken("PLUS", false,false,__FUNCTION__) || isToken("MINU", false,false,__FUNCTION__) || isToken("NOT", false,false,__FUNCTION__)
        || isToken("IDENFR", false,false,__FUNCTION__) || isToken("LPARENT", false,false,__FUNCTION__) || isToken("INTCON", false,false,__FUNCTION__)) {
        return true;
    }
    return false;
}

bool isToken(const char *str, bool getNext, bool errorMessage, const char* funcname) {
    if (iter != TokenList.end() && iter->getType() == str) {
        if (getNext) {
            if(parser) {
                fout << iter->getType() << " " << iter->getName() << std::endl;
            }
            getNextToken();
        }
        return true;
    } else {
        if (errorMessage)
            getParserErrorMessage("expect " + std::string(str), *iter, funcname);
        return false;
    }
}

void parseCompUnit() {
    iter = TokenList.begin();
    pushSymbolTable();

    while (isToken("VOIDTK",false, false, __FUNCTION__) || isToken("CONSTTK",false, false, __FUNCTION__) || isToken("INTTK",false, false, __FUNCTION__)) {
        if (isToken("VOIDTK", false, false, __FUNCTION__)) {
            currentFuncReturnType = 0;
            parseFuncDef();
        }
        else if (isToken("CONSTTK", false, false,__FUNCTION__)) {
            parseDecl();
        } else if (isToken("INTTK", false, false,__FUNCTION__)) {
            if (preReadToken(1)!= nullptr && preReadToken(1)->getType() == "MAINTK") {
                currentFuncReturnType = 1;
                parseMainFuncDef();
            } else if (preReadToken(1)!= nullptr && preReadToken(1)->getType() == "IDENFR") {
                if (preReadToken(2)!= nullptr && preReadToken(2)->getType() == "LPARENT") {
                    currentFuncReturnType = 1;
                    parseFuncDef();
                } else if (preReadToken(2)!= nullptr && (preReadToken(2)->getType() == "LBRACK"
                        || preReadToken(2)->getType() == "SEMICN" || preReadToken(2)->getType() == "COMMA"
                        || preReadToken(2)->getType() == "ASSIGN")) {
                    parseDecl();
                } else {
                    assert(0);
                }
            } else {
                assert(0);
            }
        }
    }
    if (iter != TokenList.end()) {
        assert(0);
    }

    printParserResult("<CompUnit>");
}

void parseDecl() {
    if(isToken("CONSTTK", false, false, __FUNCTION__)) {
        parseConstDecl();
        return;
    } else {
        parseVarDecl();
        return;
    }
}

void parseConstDecl() {
    if (isToken("CONSTTK", true, true, __FUNCTION__ )) {
        parseBType();
        parseConstDef();    // constDef 可能为nullptr
        while (isToken("COMMA", true, false, __FUNCTION__)) {
            parseConstDef();    // constDef 可能为nullptr
        }
        if (isToken("SEMICN", true, true, __FUNCTION__)) {
        } else {
            printError((iter-1)->getRowNo(), 'i', "parseConstDecl");
        }
        printParserResult("<ConstDecl>");
    } else {
        assert(0);
    }
}

void parseBType() {
    if (isToken("INTTK", true, true, __FUNCTION__)) {
    } else {
        assert(0);
    }
}

void parseConstDef() {
    int type = 0;
    std::string constName = iter->getName();
    int row = iter->getRowNo();

    parseIdent();
    if (checkName(row, "parseConstDef", constName)) {
        skip(0);
        return;
    }
    while(isToken("LBRACK", true, false, __FUNCTION__)) {
        type++;
        parseConstExp();
        if (isToken("RBRACK", true, true,__FUNCTION__)) {
        } else {
            printError((iter-1)->getRowNo(), 'k', "praseConstDef");
        }
    }

    pushSymbol(constName, type, 1, "");

    if (isToken("ASSIGN", true, true, __FUNCTION__)) {
        parseConstInitVal();
    } else {
        assert(0);
    }
    printParserResult("<ConstDef>");
}

std::string parseIdent() {
    if (isToken("IDENFR", true, true, __FUNCTION__)) {
        return (iter-1)->getName();
    } else {
        assert(0);
    }
}

void parseConstInitVal() {
    if (isToken("LBRACE", true, false, __FUNCTION__)) {   // 嵌套情况
        if (isToken("RBRACE", true, false, __FUNCTION__)) {
        } else {
            parseConstInitVal();
            while(isToken("COMMA", true, false, __FUNCTION__)) {
                parseConstInitVal();
            }
            if (isToken("RBRACE", true, true, __FUNCTION__)) {
            } else {
                assert(0);
            }
        }
    } else {
        parseConstExp();
    }
    printParserResult("<ConstInitVal>");
}

void parseVarDecl() {
    parseBType();
    parseVarDef();
    while(isToken("COMMA", true, false, __FUNCTION__)) {
        parseVarDef();
    }
    if(isToken("SEMICN", true, true, __FUNCTION__)) {
    } else {
        printError((iter-1)->getRowNo(), 'i', "parseVarDecl");
    }
    printParserResult("<VarDecl>");
}

void parseVarDef() {
    int type = 0; // 变量的类型
    std::string varName = iter->getName(); // 变量的名字
    int row = iter->getRowNo();

    parseIdent();
    if (checkName(row, "parseVarDef", varName)) {
        skip(0);
        return;
    }
    while(isToken("LBRACK", true, false, __FUNCTION__)) {
        type++;
        parseConstExp();
        if (isToken("RBRACK", true, true, __FUNCTION__)) {
        } else {
            printError((iter-1)->getRowNo(), 'k', "parseVarDef");
        }
    }
    pushSymbol(varName, type, 0, "");

    if (isToken("ASSIGN", true, false, __FUNCTION__)) {
        parseInitVal();
    } else {
    }
    printParserResult("<VarDef>");
}

void parseInitVal() {
    if (isToken("LBRACE", true, false, __FUNCTION__)) {
        if (isToken("RBRACE", true, false, __FUNCTION__)) {
        } else {
            parseInitVal();
            while (isToken("COMMA", true, false, __FUNCTION__)) {
                parseInitVal();
            }
            if (isToken("RBRACE",true, true, __FUNCTION__)) {
            } else {
                assert(0);
            }
        }
    } else {
        parseExp();
    }
    printParserResult("<InitVal>");
}

void parseFuncDef() {
    std::string func_retype = iter->getType();
    parseFuncType();
    std::string funcname = iter->getName();
    int row = iter->getRowNo();
    parseIdent();

    if (checkName(row, "parseFuncDef", funcname)) {
        skip(1);
        return;
    }
    pushSymbol(funcname, -1, 0, func_retype);

    pushSymbolTable();
    if (isToken("LPARENT", true, true, __FUNCTION__)) {
        if (isToken("RPARENT", true, false, __FUNCTION__)) {
            parseBlock();
            int row0 = (iter-1)->getRowNo();
            checkHasReturn(row0, "parseFuncDef");
            printParserResult("<FuncDef>");
        } else {
            if (isToken("INTTK", false, false, __FUNCTION__)) {
                parseFuncFParams(funcname);
            }
            if (isToken("RPARENT", true, true, __FUNCTION__)) {
            } else {
                printError((iter-1)->getRowNo(), 'j', "parseFuncDef");
            }
            parseBlock();
            checkHasReturn((iter-1)->getRowNo(), "parseFuncDef");
            printParserResult("<FuncDef>");
        }
    } else {
        assert(0);
    }
    popSymbolTable(); // 弹出本层符号表
}

void parseMainFuncDef() {
    if (isToken("INTTK", true, true, __FUNCTION__)) {
        int row = iter->getRowNo();
        checkFuncSymbol(row, "parseMainFuncDef", "main", "INTTK");
        if (isToken("MAINTK", true, true, __FUNCTION__)) {
            if (isToken("LPARENT", true, true, __FUNCTION__)) {
                if (isToken("RPARENT", true, true, __FUNCTION__)) {
                    pushSymbolTable(); // 压入新的符号表
                    parseBlock();
                    int row0 = (iter-1)->getRowNo();
                    checkHasReturn(row0, "parseMainFuncDef");
                    popSymbolTable(); // 弹出符号表
                } else {
                    printError((iter-1)->getRowNo(),'j',"parseMainFuncDef");
                }
            } else {
                assert(0);
            }
        } else {
            assert(0);
        }
    } else {
        assert(0);
    }
    printParserResult("<MainFuncDef>");
}

void parseFuncType() {
    if (isToken("VOIDTK", true, false, __FUNCTION__)) {
    } else if (isToken("INTTK", true, false, __FUNCTION__)) {
    } else {
        assert(0);
    }
    printParserResult("<FuncType>");
}

void parseFuncFParams(const std::string& funcname) {
    int type = parseFuncFParam();
    pushSymbolFuncParam(funcname, type);
    while(isToken("COMMA", true, false, __FUNCTION__)) {
        type = parseFuncFParam();
        pushSymbolFuncParam(funcname, type);
    }
    printParserResult("<FuncFParams>");
}

int parseFuncFParam() {
    int type = 0;
    parseBType();
    int row = iter->getRowNo();
    std::string name = parseIdent();
    if (checkName(row, "parseFuncFParam", name)) {
        skip(0);
        return -1;
    }

    if (isToken("LBRACK", true, false, __FUNCTION__)) {
        type++;
        if (isToken("RBRACK", true, true, __FUNCTION__)) {
            if (isToken("LBRACK", true, false, __FUNCTION__)) {
                type++;
                parseConstExp();
                if (isToken("RBRACK", true, true, __FUNCTION__)) {
                } else {
                    printError((iter-1)->getRowNo(),'k',"parseFuncFParam");
                }
            } else {
            }
        } else {
            printError((iter-1)->getRowNo(),'k',"parseFuncFParam");
        }
    }
    checkVarSymbol(row, "parseFuncFParam", name, type, 0);
    printParserResult("<FuncFParam>");
    return type;
}

void parseBlock() {
    bool flag = false;
    if (isToken("LBRACE", true, true, __FUNCTION__)) {
        while (!isToken("RBRACE", true, false, __FUNCTION__)) {
            parseBlockItem();
        }
    } else {
        assert(0);
    }
    printParserResult("<Block>");
}

void parseBlockItem() {
    if (isToken("CONSTTK", false, false, __FUNCTION__) || isToken("INTTK", false, false, __FUNCTION__)) {
        parseDecl();
    } else {
        parseStmt();
    }
}

void parseStmt() {
    if (isToken("SEMICN", false, false, __FUNCTION__)) {
        parseExpStmt();
    } else if (isToken("LBRACE", false, false, __FUNCTION__)) {
        parseBlockStmt();
    } else if (isToken("IFTK", false, false, __FUNCTION__)) {
        parseIfStmt();
    } else if (isToken("FORTK", false, false, __FUNCTION__)) {
        isCircle++;
        parseForstmt();
        isCircle--;
    } else if (isToken("BREAKTK",false, false, __FUNCTION__)) {
        if (!isCircle) {    // 检查是否在循环块中
            printError(iter->getRowNo(),'m', "parseBreakStmt");
        }
        parseBreakStmt();
    } else if (isToken("CONTINUETK", false, false, __FUNCTION__)) {
        if (!isCircle) {    //检查是否在循环块中
            printError(iter->getRowNo(),'m', "parseContinueStmt");
        }
        parseContinueStmt();
    } else if (isToken("RETURNTK", false, false, __FUNCTION__)) {
        parseReturnStmt();
    } else if (isToken("PRINTFTK", false, false, __FUNCTION__)) {
        parsePrintfStmt();
    } else {
        if (isToken("IDENFR", false, false, __FUNCTION__)) {
            if (preReadToken(1) != nullptr && preReadToken(1)->getType() == "LPARENT") {
                parseExpStmt();
            } else {
                auto it = iter;
                bool mod = parser;
                bool mode = error;
                parser = false;
                error = false;
                parseLVal();
                if (isToken("ASSIGN", true, false, __FUNCTION__)) {
                    parser = mod;
                    error = mode;
                    if (isToken("GETINTTK", false, false, __FUNCTION__)) {
                        iter = it;
                        parseGetintStmt();
                    } else {
                        iter = it;
                        parseAssignStmt();
                    }
                } else {   // 回溯
                    parser = mod;
                    error = mode;
                    iter = it;
                    parseExpStmt();
                }
            }
        } else {
            parseExpStmt();
        }
    }
    printParserResult("<Stmt>");
}

void parseAssignStmt() {
    std::string name = iter->getName();
    parseLVal();
    checkConst(name, (iter-1)->getRowNo(), "parseAssignStmt");
    if (isToken("ASSIGN", true, true, __FUNCTION__)) {
        parseExp();
        if (isToken("SEMICN", true, true, __FUNCTION__)) {
        } else {
            printError((iter-1)->getRowNo(), 'i', "parseAssignStmt");
        }
    } else {
        assert(0);
    }
}

void parseExpStmt() {
    if (isToken("SEMICN", true, false, __FUNCTION__)) {
    } else {
        parseExp();
        if (isToken("SEMICN", true, true, __FUNCTION__)) {
        } else {
            printError((iter-1)->getRowNo(), 'i', "parseExpStmt");
        }
    }
}

void parseBlockStmt() {
    pushSymbolTable();
    parseBlock();
    popSymbolTable();
}

void parseIfStmt() {
    if (isToken("IFTK", true, true, __FUNCTION__)) {
        if (isToken("LPARENT", true, true, __FUNCTION__)) {
            parseCond();
            if (isToken("RPARENT", true, true, __FUNCTION__)) {
                parseStmt();
                if (isToken("ELSETK", true, false, __FUNCTION__)) {
                    parseStmt();
                }
            } else {
                printError((iter-1)->getRowNo(), 'j', "parseIfStmt");
            }
        } else {
            assert(0);
        }
    } else {
        assert(0);
    }
}

void parseForstmt() {
    if (isToken("FORTK", true, true, __FUNCTION__)) {
        if (isToken("LPARENT", true, true, __FUNCTION__)) {
            if (isToken("SEMICN", true, false, __FUNCTION__)) {
            } else {
                parseForStmt();
                if (isToken("SEMICN", true, true, __FUNCTION__)) {
                } else {
                    printError((iter-1)->getRowNo(), 'i', "parseForStmt");
                }
            }
            if (isToken("SEMICN", true, false, __FUNCTION__)) {
            } else {
                parseCond();
                if (isToken("SEMICN", true, true, __FUNCTION__)) {
                } else {
                    printError((iter-1)->getRowNo(), 'i', "parseForStmt");
                }
            }
            if (isToken("RPARENT", true, false, __FUNCTION__)) {
            } else {
                parseForStmt();
                if (isToken("RPARENT", true, true, __FUNCTION__)) {
                } else {
                    printError((iter-1)->getRowNo(), 'j', "parseForStmt");
                }
            }
            parseStmt();
        } else {
            assert(0);
        }
    } else {
        assert(0);
    }
}

void parseBreakStmt() {
    if (isToken("BREAKTK", true, true, __FUNCTION__)) {
        if (isToken("SEMICN", true, true, __FUNCTION__)) {
        } else {
            printError((iter-1)->getRowNo(), 'i', "parseBreakStmt");
        }
    } else {
        assert(0);
    }
}

void parseContinueStmt() {
    if (isToken("CONTINUETK", true, true, __FUNCTION__)) {
        if (isToken("SEMICN", true, true, __FUNCTION__)) {
        } else {
            printError((iter-1)->getRowNo(), 'i', "parseContinueStmt");
        }
    } else {
        assert(0);
    }
}

void parseReturnStmt() {
    int row = iter->getRowNo();
    if (isToken("RETURNTK", true, true, __FUNCTION__)) {
        if (isToken("SEMICN", true, false, __FUNCTION__)) {
            checkReturn(row, "parseReturnStmt", 0);
        } else {
            if (preReadExp()) {
                parseExp();
            }
            checkReturn(row, "parseReturnStmt", 1);
            if (isToken("SEMICN", true, true, __FUNCTION__)) {
            } else {
                printError((iter-1)->getRowNo(), 'i', "parseReturnStmt");
            }
            return;
        }
    } else {
        assert(0);
    }
}

void parseGetintStmt() {
    std::string name = iter->getName();
    parseLVal();
    checkConst(name, (iter-1)->getRowNo(), "parseGetintStmt");
    if (isToken("ASSIGN", true, true, __FUNCTION__)) {
        if (isToken("GETINTTK",true, true, __FUNCTION__)) {
            if (isToken("LPARENT", true, true, __FUNCTION__)) {
                if (isToken("RPARENT",true, true, __FUNCTION__)) {
                    if (isToken("SEMICN", true, true, __FUNCTION__)) {
                    } else {
                        printError((iter-1)->getRowNo(), 'i', "parseGetIntStmt");
                    }
                } else {
                    printError((iter-1)->getRowNo(), 'j', "parseGetIntStmt");
                }
            } else {
                assert(0);
            }
        } else {
            assert(0);
        }
    } else {
        assert(0);
    }
}

void parsePrintfStmt() {
    if (isToken("PRINTFTK", true, true, __FUNCTION__)) {
        if (isToken("LPARENT", true, true, __FUNCTION__)) {
            checkInvalidChar(iter->getRowNo(), iter->getName());
            if (isToken("STRCON", true, true, __FUNCTION__)) {
                int row = (iter-1)->getRowNo();
                int num = getParamNumber(*(iter-1));
                while (isToken("COMMA", true, false, __FUNCTION__)) {
                    parseExp();
                    num--;
                }
                if(num) {
                    printError(row,'l', "parsePrintfStmt");
                }
                if (isToken("RPARENT", true, true, __FUNCTION__)) {
                    if (isToken("SEMICN", true, true, __FUNCTION__ )) {
                    } else {
                        printError((iter-1)->getRowNo(), 'i', "parsePrintfStmt");
                    }
                } else {
                    printError((iter-1)->getRowNo(), 'j', "parsePrintfStmt");
                }
            } else {
                assert(0);
            }
        } else {
            assert(0);
        }
    } else {
        assert(0);
    }
}

void parseForStmt() {
    parseLVal();
    if (isToken("ASSIGN", true, true, __FUNCTION__)) {
        parseExp();
    } else {
        assert(0);
    }
    printParserResult("<ForStmt>");
}

int parseExp() {
    int type = parseAddExp();
    printParserResult("<Exp>");
    return type;
}

void parseCond() {
    parseLOrExp();
    printParserResult("<Cond>");
}

int parseLVal() {
    std::string name = parseIdent();
    int type = getVarType(name);
    int row = (iter-1)->getRowNo();
    checkVarDefine(row, "parseLVal", name);

    while(isToken("LBRACK", true, false, __FUNCTION__)) {
        type--;
        parseExp();
        if (isToken("RBRACK", true, true, __FUNCTION__)) {
        } else {
            printError((iter-1)->getRowNo(),'k',"parseLVal");
        }
    }
    printParserResult("<LVal>");
    return type;
}

int parsePrimaryExp() {
    int type;
    if(isToken("LPARENT", true, false, __FUNCTION__)) {
        type = parseExp();
        if (isToken("RPARENT", true, true, __FUNCTION__)) {
        } else {
            printError((iter-1)->getRowNo(),'k',"parsePrimaryExp");
        }
    } else if(isToken("IDENFR", false, false, __FUNCTION__)) {
        type = parseLVal();
    } else {
        type = 0;
        parseNumber();
    }
    printParserResult("<PrimaryExp>");
    return type;
}

void parseNumber() {
    if (isToken("INTCON", true, true, __FUNCTION__)) {
        printParserResult("<Number>");
    } else {
        assert(0);
    }
}

int parseUnaryExp() {
    int type;
    if (isToken("PLUS", false, false, __FUNCTION__) || isToken("MINU", false, false, __FUNCTION__) || isToken("NOT", false, false, __FUNCTION__)) {
        parseUnaryOp();
        type = parseUnaryExp();
    } else if (isToken("IDENFR", false, false, __FUNCTION__)) {
        if (preReadToken(1) != nullptr && preReadToken(1)->getType() == "LPARENT") {
            type = parseFuncCall();
        } else {
            type = parsePrimaryExp();
        }
    } else {
        type = parsePrimaryExp();
    }
    printParserResult("<UnaryExp>");
    return type;
}

void parseUnaryOp() {
    if (isToken("PLUS", true, false, __FUNCTION__)) {
    } else if (isToken("MINU", true, false, __FUNCTION__)) {
    } else if (isToken("NOT", true, false, __FUNCTION__)) {
    } else {
        assert(0);
    }
    printParserResult("<UnaryOp>");
}

int parseFuncRParams(int row, const std::string& funcname) {
    int num = 0;
    int type = parseExp();
    checkParaType(row, "parseFuncRParams", funcname, num, type);
    num++;
    while (isToken("COMMA", true, false, __FUNCTION__)) {
        type = parseExp();
        checkParaType(row, "parseFuncRParams", funcname, num, type);
        num++;
    }
    printParserResult("<FuncRParams>");
    return num;
}

int parseMulExp() {
    int type = parseUnaryExp();
    if (isToken("MULT", false, false, __FUNCTION__ ) || isToken("DIV", false, false, __FUNCTION__ ) || isToken("MOD", false, false, __FUNCTION__ )) {
        printParserResult("<MulExp>");
        while (isToken("MULT", true, false, __FUNCTION__) || isToken("DIV", true, false, __FUNCTION__) ||
               isToken("MOD", true, false, __FUNCTION__)) {
            parseUnaryExp();
            printParserResult("<MulExp>");
        }
    } else {
        printParserResult("<MulExp>");
    }
    return type;
}

int parseAddExp() {
    int type = parseMulExp();
    if (isToken("PLUS", false, false, __FUNCTION__) || isToken("MINU", false, false, __FUNCTION__)) {
        printParserResult("<AddExp>");
        while(isToken("PLUS", true, false, __FUNCTION__) || isToken("MINU", true, false, __FUNCTION__)) {
            parseMulExp();
            printParserResult("<AddExp>");
        }
    } else {
        printParserResult("<AddExp>");
    }
    return type;
}

void parseRelExp() {
    parseAddExp();
    if (isToken("LSS", false, false, __FUNCTION__) || isToken("GRE", false, false, __FUNCTION__)
                        || isToken("LEQ", false, false, __FUNCTION__) || isToken("GEQ", false, false, __FUNCTION__)) {
        printParserResult("<RelExp>");
        while (isToken("LSS", true, false, __FUNCTION__) || isToken("GRE", true, false, __FUNCTION__)
               || isToken("LEQ", true, false, __FUNCTION__) || isToken("GEQ", true, false, __FUNCTION__)) {
            parseAddExp();
            printParserResult("<RelExp>");
        }
    } else {
        printParserResult("<RelExp>");
    }
}

void parseEqExp() {
    parseRelExp();
    if (isToken("EQL", false, false, __FUNCTION__) || isToken("NEQ", false, false, __FUNCTION__)) {
        printParserResult("<EqExp>");
        while(isToken("EQL", true, false, __FUNCTION__) || isToken("NEQ", true, false, __FUNCTION__)) {
            parseRelExp();
            printParserResult("<EqExp>");
        }
    } else {
        printParserResult("<EqExp>");
    }
}

void parseLAndExp() {
    parseEqExp();
    if (isToken("AND", false, false, __FUNCTION__)) {
        printParserResult("<LAndExp>");
        while ((isToken("AND", true, false, __FUNCTION__))) {
            parseEqExp();
            printParserResult("<LAndExp>");
        }
    } else {
        printParserResult("<LAndExp>");
    }
}

void parseLOrExp() {
    parseLAndExp();
    if (isToken("OR", false, false, __FUNCTION__)) {
        printParserResult("<LOrExp>");
        while ((isToken("OR", true, false, __FUNCTION__))) {
            parseLAndExp();
            printParserResult("<LOrExp>");
        }
    } else {
        printParserResult("<LOrExp>");
    }
}

void parseConstExp() {
    parseAddExp();
    printParserResult("<ConstExp>");
}

void printParserResult(const char* string) {
    if (parser) { // 使用printf模式
            fout << string << endl;
    }
}

int parseFuncCall() {
    int type;
    std::string name = parseIdent();
    int row = (iter-1)->getRowNo();
    checkFuncDefine(row, "parseUnaryExp", name);
    type = getFuncType(name) - 1;

    if (isToken("LPARENT", true, true, __FUNCTION__)) {
        if (isToken("RPARENT", true, false, __FUNCTION__)) {
            checkParaNum(row, "parseFuncRParams", name, 0);
        } else {
            if (preReadExp()) {
                int num = parseFuncRParams(row, name);
                checkParaNum(row, "parseFuncRParams", name, num);
            }
            if (isToken("RPARENT", true, true, __FUNCTION__)) {
            } else {
                printError((iter-1)->getRowNo(),'j',"parseUnaryExp");
            }
        }
    } else {
        assert(0);
    }
    return type;
}

bool isLastStmt() {
    bool mode1 = ir;
    bool mode2 = error;
    auto iter0 = iter;
    ir = false;
    error = false;

    while (!isToken("RBRACE", true, false, __FUNCTION__)) {
        if (isToken("CONSTTK", false, false, __FUNCTION__) || isToken("INTTK", false, false, __FUNCTION__)) {
            parseDecl();
        } else {
            ir = mode1;
            error = mode2;
            iter = iter0;
            return false;
        }
    }
    ir = mode1;
    error = mode2;
    iter = iter0;
    return true;
}