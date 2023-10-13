//
// Created by 杨贺森 on 2023/9/27.
//
#include <fstream>
#include "lexer.h"
#include "parser.h"
#include "error.h"
#include <cassert>

extern bool parser;
extern std::vector<Token>::iterator iter;
extern std::vector<Token> TokenList;
extern ofstream fout;

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

    while (isToken("VOIDTK",false, false, __FUNCTION__) || isToken("CONSTTK",false, false, __FUNCTION__) || isToken("INTTK",false, false, __FUNCTION__)) {
        if (isToken("VOIDTK", false, false, __FUNCTION__)) {
            parseFuncDef();
        }
        else if (isToken("CONSTTK", false, false,__FUNCTION__)) {
            parseDecl();
        } else if (isToken("INTTK", false, false,__FUNCTION__)) {
            if (preReadToken(1)!= nullptr && preReadToken(1)->getType() == "MAINTK") {
                parseMainFuncDef();
            } else if (preReadToken(1)!= nullptr && preReadToken(1)->getType() == "IDENFR") {
                if (preReadToken(2)!= nullptr && preReadToken(2)->getType() == "LPARENT") {
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
            assert(0);
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
    parseIdent();
    while(isToken("LBRACK", true, false, __FUNCTION__)) {
        parseConstExp();
        if (isToken("RBRACK", true, true,__FUNCTION__)) {
        } else {
            assert(0);
        }
    }
    if (isToken("ASSIGN", true, true, __FUNCTION__)) {
        parseConstInitVal();
    } else {
        assert(0);
    }
    printParserResult("<ConstDef>");
}

void parseIdent() {
    if (isToken("IDENFR", true, true, __FUNCTION__)) {
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
        assert(0);
    }
    printParserResult("<VarDecl>");
}

void parseVarDef() {
    parseIdent();
    while(isToken("LBRACK", true, false, __FUNCTION__)) {
        parseConstExp();
        if (isToken("RBRACK", true, true, __FUNCTION__)) {
        } else {
            assert(0);
        }
    }
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
    parseFuncType();
    parseIdent();
    if (isToken("LPARENT", true, true, __FUNCTION__)) {
        if (isToken("RPARENT", true, false, __FUNCTION__)) {
            parseBlock();
            printParserResult("<FuncDef>");
            return;
        } else {
            parseFuncFParams();
            if (isToken("RPARENT", true, true, __FUNCTION__)) {
                parseBlock();
            } else {
                assert(0);
            }
            printParserResult("<FuncDef>");
        }
    } else {
        assert(0);
    }
}

void parseMainFuncDef() {
    if (isToken("INTTK", true, true, __FUNCTION__)) {
        if (isToken("MAINTK", true, true, __FUNCTION__)) {
            if (isToken("LPARENT", true, true, __FUNCTION__)) {
                if (isToken("RPARENT", true, true, __FUNCTION__)) {
                    parseBlock();
                } else {
                    assert(0);
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

void parseFuncFParams() {
    parseFuncFParam();
    while(isToken("COMMA", true, false, __FUNCTION__)) {
        parseFuncFParam();
    }
    printParserResult("<FuncFParams>");
}

void parseFuncFParam() {
    parseBType();
    parseIdent();
    if (isToken("LBRACK", true, false, __FUNCTION__)) {
        if (isToken("RBRACK", true, true, __FUNCTION__)) {
            if (isToken("LBRACK", true, false, __FUNCTION__)) {
                parseConstExp();
                if (isToken("RBRACK", true, true, __FUNCTION__)) {
                } else {
                    assert(0);
                }
            } else {
            }
        } else {
            assert(0);
        }
    } else {
    }
    printParserResult("<FuncFParam>");
}

void parseBlock() {
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
        parseForstmt();
    } else if (isToken("BREAKTK",false, false, __FUNCTION__)) {
        parseBreakStmt();
    } else if (isToken("CONTINUETK", false, false, __FUNCTION__)) {
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
                parser = false;
                parseLVal();
                if (isToken("ASSIGN", true, false, __FUNCTION__)) {
                    parser = mod;
                    if (isToken("GETINTTK", false, false, __FUNCTION__)) {
                        iter = it;
                        parseGetintStmt();
                    } else {
                        iter = it;
                        parseAssignStmt();
                    }
                } else {   // 回溯
                    parser = mod;
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
    parseLVal();
    if (isToken("ASSIGN", true, true, __FUNCTION__)) {
        parseExp();
        if (isToken("SEMICN", true, true, __FUNCTION__)) {
        } else {
            assert(0);
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
            assert(0);
        }
    }
}

void parseBlockStmt() {
    parseBlock();
}

void parseIfStmt() {
    if (isToken("IFTK", true, true, __FUNCTION__)) {
        if (isToken("LPARENT", true, true, __FUNCTION__)) {
            parseCond();
            if (isToken("RPARENT", true, true, __FUNCTION__)) {
                parseStmt();
                if (isToken("ELSETK", true, false, __FUNCTION__)) {
                    parseStmt();
                } else {
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

void parseForstmt() {
    if (isToken("FORTK", true, true, __FUNCTION__)) {
        if (isToken("LPARENT", true, true, __FUNCTION__)) {
            if (isToken("SEMICN", true, false, __FUNCTION__)) {
            } else {
                parseForStmt();
                if (isToken("SEMICN", true, true, __FUNCTION__)) {
                } else {
                    assert(0);
                }
            }
            if (isToken("SEMICN", true, false, __FUNCTION__)) {
            } else {
                parseCond();
                if (isToken("SEMICN", true, true, __FUNCTION__)) {
                } else {
                    assert(0);
                }
            }
            if (isToken("RPARENT", true, false, __FUNCTION__)) {
            } else {
                parseForStmt();
                if (isToken("RPARENT", true, true, __FUNCTION__)) {
                } else {
                    assert(0);
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
            assert(0);
        }
    } else {
        assert(0);
    }
}

void parseContinueStmt() {
    if (isToken("CONTINUETK", true, true, __FUNCTION__)) {
        if (isToken("SEMICN", true, true, __FUNCTION__)) {
        } else {
            assert(0);
        }
    } else {
        assert(0);
    }
}

void parseReturnStmt() {
    if (isToken("RETURNTK", true, true, __FUNCTION__)) {
        if (isToken("SEMICN", true, false, __FUNCTION__)) {
        } else {
            parseExp();
            if (isToken("SEMICN", true, true, __FUNCTION__)) {
            } else {
                assert(0);
            }
            return;
        }
    } else {
        assert(0);
    }
}

void parseGetintStmt() {
    parseLVal();
    if (isToken("ASSIGN", true, true, __FUNCTION__)) {
        if (isToken("GETINTTK",true, true, __FUNCTION__)) {
            if (isToken("LPARENT", true, true, __FUNCTION__)) {
                if (isToken("RPARENT",true, true, __FUNCTION__)) {
                    if (isToken("SEMICN", true, true, __FUNCTION__)) {
                    } else {
                        assert(0);
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
    } else {
        assert(0);
    }
}

void parsePrintfStmt() {
    if (isToken("PRINTFTK", true, true, __FUNCTION__)) {
        if (isToken("LPARENT", true, true, __FUNCTION__)) {
            if (isToken("STRCON", true, true, __FUNCTION__)) {
                while (isToken("COMMA", true, false, __FUNCTION__)) {
                    parseExp();
                }
                if (isToken("RPARENT", true, true, __FUNCTION__)) {
                    if (isToken("SEMICN", true, true, __FUNCTION__ )) {
                    } else {
                        assert(0);
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

void parseExp() {
    parseAddExp();
    printParserResult("<Exp>");
}

void parseCond() {
    parseLOrExp();
    printParserResult("<Cond>");
}

void parseLVal() {
    parseIdent();
    while(isToken("LBRACK", true, false, __FUNCTION__)) {
        parseExp();
        if (isToken("RBRACK", true, true, __FUNCTION__)) {
        } else {
            assert(0);
        }
    }
    printParserResult("<LVal>");
}

void parsePrimaryExp() {
    if(isToken("LPARENT", true, false, __FUNCTION__)) {
        parseExp();
        if (isToken("RPARENT", true, true, __FUNCTION__)) {
        } else {
            assert(0);
        }
    } else if(isToken("IDENFR", false, false, __FUNCTION__)) {
        parseLVal();
    } else {
        parseNumber();
    }
    printParserResult("<PrimaryExp>");
}

void parseNumber() {
    if (isToken("INTCON", true, true, __FUNCTION__)) {
        printParserResult("<Number>");
    } else {
        assert(0);
    }
}

void parseUnaryExp() {
    if (isToken("PLUS", false, false, __FUNCTION__) || isToken("MINU", false, false, __FUNCTION__) || isToken("NOT", false, false, __FUNCTION__)) {
        parseUnaryOp();
        parseUnaryExp();
    } else if (isToken("IDENFR", false, false, __FUNCTION__)) {
        if (preReadToken(1) != nullptr && preReadToken(1)->getType() == "LPARENT") {
            parseIdent();
            if (isToken("LPARENT", true, true, __FUNCTION__)) {
                if (isToken("RPARENT", true, false, __FUNCTION__)) {
                } else {
                    parseFuncRParams();
                    if (isToken("RPARENT", true, true, __FUNCTION__)) {
                    } else {
                        assert(0);
                    }
                }
            } else {
                assert(0);
            }
        } else {
            parsePrimaryExp();
        }
    } else {
        parsePrimaryExp();
    }
    printParserResult("<UnaryExp>");
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

void parseFuncRParams() {
    parseExp();
    while (isToken("COMMA", true, false, __FUNCTION__)) {
        parseExp();
    }
    printParserResult("<FuncRParams>");
}

void parseMulExp() {
    parseUnaryExp();
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
}

void parseAddExp() {
    parseMulExp();
    if (isToken("PLUS", false, false, __FUNCTION__) || isToken("MINU", false, false, __FUNCTION__)) {
        printParserResult("<AddExp>");
        while(isToken("PLUS", true, false, __FUNCTION__) || isToken("MINU", true, false, __FUNCTION__)) {
            parseMulExp();
            printParserResult("<AddExp>");
        }
    } else {
        printParserResult("<AddExp>");
    }
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