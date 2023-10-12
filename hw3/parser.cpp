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

void setNull(void** ptr) {
    if (ptr != nullptr) {
//         free(*ptr);
//        *ptr = nullptr;
//        fout << "set Null ptr!!!!!!!!!" << endl;
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
//                    getParserErrorMessage("Unknown token", *iter, __FUNCTION__);
//                    setNull((void**)& compUnit);
//                    return;
                }
            } else {
                assert(0);
//                getParserErrorMessage("Unknown token", *iter, __FUNCTION__);
//                setNull((void**)& compUnit);
//                return;
            }
        }
    }
    if (iter != TokenList.end()) {
        assert(0);
        getParserErrorMessage("Unknown token", *iter, __FUNCTION__);
        return;
    }

//    if (mainFuncDef == nullptr) {         // 判断main函数是否存在
//        getParserErrorMessage("No Main Function Define", *iter, (void**)&compUnit);
//        compUnit = nullptr;
//        return;
//    }

//    CompUnit result = CompUnit(decls, funcDefs, mainFuncDef);
//    *compUnit = CompUnit(result);
    printParserResult("<CompUnit>");
}

void parseDecl() {
    if(isToken("CONSTTK", false, false, __FUNCTION__)) {
        parseConstDecl();
//        Decl result = Decl(constDecl);
//        *decl = result;
        return;
    } else {
        parseVarDecl();
//        Decl result = Decl(varDecl);
//        *decl = result;
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
//            setNull((void **) &constDecl);
//            return;
        }
//        ConstDecl result = ConstDecl(constDefs);
//        *constDecl = result;
        printParserResult("<ConstDecl>");
    } else {
        assert(0);
//        setNull((void**)& constDecl);
//        return;
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
//            setNull((void**)&constDef);
//            return;
        }
    }
    if (isToken("ASSIGN", true, true, __FUNCTION__)) {
        parseConstInitVal();
    } else {
        assert(0);
//        setNull((void**)& constDef);
//        return;
    }
//    ConstDef result = ConstDef(ident, constExps, constInitVal);
//    *constDef = result;
    printParserResult("<ConstDef>");
}

void parseIdent() {
    if (isToken("IDENFR", true, true, __FUNCTION__)) {
//        auto result = Ident(token);
//        *ident = result;
    } else {
        assert(0);
//        setNull((void**)& ident);
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
//                setNull((void**)constInitVal);
//                return;
            }
        }
    } else {
        parseConstExp();
    }
//    ConstInitVal result = ConstInitVal(constExp, constInitVals);
//    *constInitVal = result;
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
//        setNull((void**) &varDecl);
//        return;
    }
//    VarDecl result = VarDecl(varDefs);
//    *varDecl = result;
    printParserResult("<VarDecl>");
}

void parseVarDef() {
    parseIdent();
    while(isToken("LBRACK", true, false, __FUNCTION__)) {
        parseConstExp();
        if (isToken("RBRACK", true, true, __FUNCTION__)) {
        } else {
            assert(0);
//            setNull((void**)&varDef);
//            return;
        }
    }
    if (isToken("ASSIGN", true, false, __FUNCTION__)) {
        parseInitVal();
    } else {
    }
//    VarDef result = VarDef(ident, constExps, initVal);
//    *varDef = result;
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
//                setNull((void**)& initVal);
//                return;
            }
        }
    } else {
        parseExp();
    }
//    InitVal result = InitVal(exp, initVals);
//    *initVal = result;
    printParserResult("<InitVal>");
}

void parseFuncDef() {
    parseFuncType();
    parseIdent();
    if (isToken("LPARENT", true, true, __FUNCTION__)) {
        if (isToken("RPARENT", true, false, __FUNCTION__)) {
            parseBlock();
//            FuncDef result = FuncDef(funcType, ident, block);
//            *funcDef = result;
            printParserResult("<FuncDef>");
            return;
        } else {
            parseFuncFParams();
            if (isToken("RPARENT", true, true, __FUNCTION__)) {
                parseBlock();
            } else {
                assert(0);
//                setNull((void**)& funcDef);
//                return;
            }
//            FuncDef result = FuncDef(funcType, ident, funcFParams, block);
//            *funcDef = result;
            printParserResult("<FuncDef>");
        }
    } else {
        assert(0);
//        setNull((void**)& funcDef);
//        return;
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
//                    setNull((void**)& mainFuncDef);
//                    return;
                }
            } else {
                assert(0);
//                setNull((void**)& mainFuncDef);
//                return;
            }
        } else {
            assert(0);
//            setNull((void**)& mainFuncDef);
//            return;
        }
    } else {
        assert(0);
//        setNull((void**)& mainFuncDef);
//        return;
    }
//    auto result = MainFuncDef(block);
//    *mainFuncDef = result;
    printParserResult("<MainFuncDef>");
}

void parseFuncType() {
    if (isToken("VOIDTK", true, false, __FUNCTION__)) {
    } else if (isToken("INTTK", true, false, __FUNCTION__)) {
    } else {
        assert(0);
//        setNull((void**)& funcType);
//        getParserErrorMessage("Unknown token", *iter, __FUNCTION__);
        return;
    }
//    auto result = FuncType(token);
//    *funcType = result;
    printParserResult("<FuncType>");
}

void parseFuncFParams() {
    parseFuncFParam();
    while(isToken("COMMA", true, false, __FUNCTION__)) {
        parseFuncFParam();
    }
//    FuncFParams result = FuncFParams(funcFParamss);
//    *funcFParams = result;
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
//                    setNull((void**)& funcFParam);
//                    return;
                }
            } else {
                // constExp maybe null
            }
        } else {
            assert(0);
//            setNull((void**)& funcFParam);
//            return;
        }
    } else {
        // constExp maybe null
    }
//    FuncFParam result = FuncFParam(ident, constExp);
//    *funcFParam = result;
    printParserResult("<FuncFParam>");
}

void parseBlock() {
    if (isToken("LBRACE", true, true, __FUNCTION__)) {
        while (!isToken("RBRACE", true, false, __FUNCTION__)) {
            parseBlockItem();
        }
    } else {
        assert(0);
//        setNull((void**)& block);
//        return;
    }
//    Block result = Block(blockItems);
//    *block = result;
    printParserResult("<Block>");
}

void parseBlockItem() {
    if (isToken("CONSTTK", false, false, __FUNCTION__) || isToken("INTTK", false, false, __FUNCTION__)) {
        parseDecl();
//        auto result = BlockItem(decl);
//        *blockItem = result;
    } else {
        parseStmt();
//        auto result = BlockItem(stmt);
//        *blockItem = result;
    }
}

void parseStmt() {
    if (isToken("SEMICN", false, false, __FUNCTION__)) {
        parseExpStmt();
//        Stmt result = Stmt(expStmt);
//        *stmt = result;
    } else if (isToken("LBRACE", false, false, __FUNCTION__)) {
        parseBlockStmt();
//        Stmt result = Stmt(blockStmt);
//        *stmt = result;
    } else if (isToken("IFTK", false, false, __FUNCTION__)) {
        parseIfStmt();
//        Stmt result = Stmt(ifStmt);
//        *stmt = result;
    } else if (isToken("FORTK", false, false, __FUNCTION__)) {
        parseForstmt();
//        Stmt result = Stmt(forstmt);
//        *stmt = result;
    } else if (isToken("BREAKTK",false, false, __FUNCTION__)) {
        parseBreakStmt();
//        Stmt result = Stmt(breakStmt);
//        *stmt = result;
    } else if (isToken("CONTINUETK", false, false, __FUNCTION__)) {
        parseContinueStmt();
//        Stmt result = Stmt(continueStmt);
//        *stmt = result;
    } else if (isToken("RETURNTK", false, false, __FUNCTION__)) {
        parseReturnStmt();
//        Stmt result = Stmt(returnStmt);
//        *stmt = result;
    } else if (isToken("PRINTFTK", false, false, __FUNCTION__)) {
        parsePrintfStmt();
//        Stmt result = Stmt(printfStmt);
//        *stmt = result;
    } else {
        if (isToken("IDENFR", false, false, __FUNCTION__)) {
            if (preReadToken(1) != nullptr && preReadToken(1)->getType() == "LPARENT") {
                parseExpStmt();
//                Stmt result = Stmt(expStmt);
//                *stmt = result;
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
//                        Stmt result = Stmt(getintStmt);
//                        *stmt = result;
                    } else {
                        iter = it;
                        parseAssignStmt();
//                        Stmt result = Stmt(assignStmt);
//                        *stmt = result;
                    }
                } else {   // 回溯
                    parser = mod;
                    iter = it;
                    parseExpStmt();
//                    Stmt result = Stmt(expStmt);
//                    *stmt = result;
                }
            }
        } else {
            parseExpStmt();
//            Stmt result = Stmt(expStmt);
//            *stmt = result;
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
//            setNull((void**)& assignStmt);
//            return;
        }
    } else {
        assert(0);
//        setNull((void**)& assignStmt);
//        return;
    }
//    AssignStmt result = AssignStmt(lval, exp);
//    *assignStmt = result;
}

void parseExpStmt() {
    if (isToken("SEMICN", true, false, __FUNCTION__)) {
    } else {
        parseExp();
        if (isToken("SEMICN", true, true, __FUNCTION__)) {
        } else {
            assert(0);
//            setNull((void**)& expStmt);
//            return;
        }
    }
//    auto result = ExpStmt(exp);
//    *expStmt = result;
}

void parseBlockStmt() {
    parseBlock();
//    auto result = BlockStmt(block);
//    *blockStmt = result;
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
//                setNull((void**)& ifStmt);
//                return;
            }
        } else {
            assert(0);
//            setNull((void**)& ifStmt);
//            return;
        }
    } else {
        assert(0);
//        setNull((void**)& ifStmt);
//        return;
    }
//    IfStmt result = IfStmt(cond, ifstmt, elsestmt);
//    *ifStmt = result;
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
//                    setNull((void**)& forstmt);
//                    return;
                }
            }
            if (isToken("SEMICN", true, false, __FUNCTION__)) {
            } else {
                parseCond();
                if (isToken("SEMICN", true, true, __FUNCTION__)) {
                } else {
                    assert(0);
//                    setNull((void**)& forstmt);
//                    return;
                }
            }
            if (isToken("RPARENT", true, false, __FUNCTION__)) {
            } else {
                parseForStmt();
                if (isToken("RPARENT", true, true, __FUNCTION__)) {
                } else {
                    assert(0);
//                    setNull((void**)& forstmt);
//                    return;
                }
            }
            parseStmt();
        } else {
            assert(0);
//            setNull((void**)& forstmt);
//            return;
        }
    } else {
        assert(0);
//        setNull((void**)& forstmt);
//        return;
    }
//    Forstmt result = Forstmt(forStmt1, cond, forStmt2, stmt);
//    *forstmt = result;
}

void parseBreakStmt() {
    if (isToken("BREAKTK", true, true, __FUNCTION__)) {
        if (isToken("SEMICN", true, true, __FUNCTION__)) {
        } else {
            assert(0);
//            setNull((void**)& breakStmt);
//            return;
        }
    } else {
        assert(0);
//        setNull((void**)& breakStmt);
//        return;
    }
//    BreakStmt result = BreakStmt();
//    *breakStmt = result;
}

void parseContinueStmt() {
    if (isToken("CONTINUETK", true, true, __FUNCTION__)) {
        if (isToken("SEMICN", true, true, __FUNCTION__)) {
        } else {
            assert(0);
//            setNull((void**)& continueStmt);
//            return;
        }
    } else {
        assert(0);
//        setNull((void**)& continueStmt);
//        return;
    }
//    ContinueStmt result = ContinueStmt();
//    *continueStmt = result;
}

void parseReturnStmt() {
    if (isToken("RETURNTK", true, true, __FUNCTION__)) {
        if (isToken("SEMICN", true, false, __FUNCTION__)) {
//            auto result = ReturnStmt();
//            *returnStmt = result;
        } else {
            parseExp();
            if (isToken("SEMICN", true, true, __FUNCTION__)) {
            } else {
                assert(0);
//                setNull((void**)& returnStmt);
//                return;
            }
//            auto result = ReturnStmt(exp);
//            *returnStmt = result;
            return;
        }
    } else {
        assert(0);
//        setNull((void**)& returnStmt);
//        return;
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
//                        setNull((void**)& getintStmt);
//                        return;
                    }
                } else {
                    assert(0);
//                    setNull((void**)& getintStmt);
//                    return;
                }
            } else {
                assert(0);
//                setNull((void**)& getintStmt);
//                return;
            }
        } else {
            assert(0);
//            setNull((void**)& getintStmt);
//            return;
        }
    } else {
        assert(0);
//        setNull((void**)& getintStmt);
//        return;
    }
//    auto result = GetintStmt(lval);
//    *getintStmt = result;
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
//                        setNull((void**)& printfStmt);
//                        return;
                    }
                } else {
                    assert(0);
//                    setNull((void**)& printfStmt);
//                    return;
                }
            } else {
                assert(0);
//                setNull((void**)& printfStmt);
//                return;
            }
        } else {
            assert(0);
//            setNull((void**)& printfStmt);
//            return;
        }
    } else {
        assert(0);
//        setNull((void**)& printfStmt);
//        return;
    }
//    PrintfStmt result = PrintfStmt(formatString, exps);
//    *printfStmt = result;
}

void parseForStmt() {
    parseLVal();
    if (isToken("ASSIGN", true, true, __FUNCTION__)) {
        parseExp();
    } else {
        assert(0);
//        setNull((void**)& forStmt);
//        return;
    }
//    ForStmt result = ForStmt(lval, exp);
//    *forStmt = result;
    printParserResult("<ForStmt>");
}

void parseExp() {
    parseAddExp();
//    Exp result = Exp(addExp);
//    *exp = result;
    printParserResult("<Exp>");
}

void parseCond() {
    parseLOrExp();
//    Cond result = Cond(lOrExp);
//    *cond = result;
    printParserResult("<Cond>");
}

void parseLVal() {
    parseIdent();
    while(isToken("LBRACK", true, false, __FUNCTION__)) {
        parseExp();
        if (isToken("RBRACK", true, true, __FUNCTION__)) {
        } else {
            assert(0);
//            setNull((void**)& lVal);
//            return;
        }
    }
//    LVal result = LVal(ident, exps);
//    *lVal = result;
    printParserResult("<LVal>");
}

void parsePrimaryExp() {
    if(isToken("LPARENT", true, false, __FUNCTION__)) {
        parseExp();
        if (isToken("RPARENT", true, true, __FUNCTION__)) {
        } else {
            assert(0);
//            setNull((void**)& primaryExp);
//            return;
        }
//        auto result = PrimaryExp(exp);
//        *primaryExp = result;
    } else if(isToken("IDENFR", false, false, __FUNCTION__)) {
        parseLVal();
//        auto result = PrimaryExp(lval);
//        *primaryExp = result;
    } else {
        parseNumber();
//        auto result = PrimaryExp(number);
//        *primaryExp = result;
    }
    printParserResult("<PrimaryExp>");
}

void parseNumber() {
    if (isToken("INTCON", true, true, __FUNCTION__)) {
//        auto result = Number(intConst);
//        *number = result;
        printParserResult("<Number>");
    } else {
        assert(0);
//        setNull((void**)& number);
//        return;
    }
}

void parseUnaryExp() {
    if (isToken("PLUS", false, false, __FUNCTION__) || isToken("MINU", false, false, __FUNCTION__) || isToken("NOT", false, false, __FUNCTION__)) {
        parseUnaryOp();
        parseUnaryExp();
//        UnaryExp result = UnaryExp(unaryOp, unaryExp1);
//        *unaryExp = result;
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
//                        setNull((void**)& unaryExp);
//                        return;
                    }
//                    UnaryExp result = UnaryExp(ident, funcRParams);
//                    *unaryExp = result;
                }
            } else {
                assert(0);
//                setNull((void**)& unaryExp);
//                return;
            }
        } else {
            parsePrimaryExp();
//            auto result = UnaryExp(primaryExp);
//            *unaryExp = result;
        }
    } else {
        parsePrimaryExp();
//        auto result = UnaryExp(primaryExp);
//        *unaryExp = result;
    }
    printParserResult("<UnaryExp>");
}

void parseUnaryOp() {
    if (isToken("PLUS", true, false, __FUNCTION__)) {
    } else if (isToken("MINU", true, false, __FUNCTION__)) {
    } else if (isToken("NOT", true, false, __FUNCTION__)) {
    } else {
        assert(0);
//        setNull((void**)& unaryOp);
//        getParserErrorMessage("Unknown Token", *iter, __FUNCTION__);
//        return;
    }
//    auto result = UnaryOp(token);
//    *unaryOp = result;
    printParserResult("<UnaryOp>");
}

void parseFuncRParams() {
    parseExp();
    while (isToken("COMMA", true, false, __FUNCTION__)) {
        parseExp();
    }
//    FuncRParams result = FuncRParams(exps);
//    *funcRParams = result;
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
//    MulExp result = MulExp(unaryExps, mulOps);
//    *mulExp = result;
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
//    AddExp result = AddExp(mulExps, addOps);
//    *addExp = result;
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
//    RelExp result = RelExp(addExps, relOps);
//    *relExp = result;
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
//    EqExp result = EqExp(relExps, eqOps);
//    *eqExp = result;
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
//    LAndExp result = LAndExp(eqExps);
//    *landExp = result;
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
//    LOrExp result = LOrExp(landExps);
//    *lorExp = result;
}

void parseConstExp() {
    parseAddExp();
//    auto result = ConstExp(addExp);
//    *constExp = result;
    printParserResult("<ConstExp>");
}

void printParserResult(const char* string) {
    if (parser) { // 使用printf模式
            fout << string << endl;
    }
}