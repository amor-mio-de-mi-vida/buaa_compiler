//
// Created by 杨贺森 on 2023/9/27.
//

#ifndef HW2_PARSER_H
#define HW2_PARSER_H
#include "lexer.h"
#include <utility>
#include <vector>
using namespace std;

//class Unit {
//};
//
//class UnaryExp;
//class Stmt;
//
//class Ident : Unit {
//    Token* ident;
//public:
//    explicit Ident(Token* _ident) : ident(_ident) {}
//};
//
//class FuncType : Unit {
//    Token* funcType;
//public:
//    explicit FuncType(Token* _funcType) : funcType(_funcType) {}
//};
//
//class MulExp : Unit {
//    vector<UnaryExp*> unaryExp;
//    vector<Token*> mulOp;
//public:
//    MulExp(vector<UnaryExp*> _unaryExp, vector<Token*> _mulOp)
//    : unaryExp(std::move(_unaryExp)), mulOp(std::move(_mulOp)) {}
//};
//
//class AddExp : Unit {
//    vector<MulExp*> mulExp;
//    vector<Token*> addOp;
//public:
//    AddExp(vector<MulExp*> _mulExp, vector<Token*> _addOp)
//        : mulExp(std::move(_mulExp)), addOp(std::move(_addOp)) {}
//};
//
//class ConstExp : Unit {
//    AddExp* addExp;
//public:
//    explicit ConstExp(AddExp* _addExp) : addExp(_addExp) {}
//};
//
//class RelExp : Unit {
//    vector<AddExp*> addExp;
//    vector<Token*> relOp;
//public:
//    RelExp(vector<AddExp*> _addExp, vector<Token*> _relOp)
//        : addExp(std::move(_addExp)), relOp(std::move(_relOp)) {}
//};
//
//class EqExp : Unit {
//    vector<RelExp*> relExp;
//    vector<Token*> eqOp;
//public:
//    EqExp(vector<RelExp*> _relExp, vector<Token*> _eqOp)
//        : relExp(std::move(_relExp)), eqOp(std::move(_eqOp)) {}
//};
//
//class LAndExp : Unit {
//    vector<EqExp*> eqExp;
//
//public:
//    explicit LAndExp(vector<EqExp*> _eqExp) : eqExp(std::move(_eqExp)){}
//};
//
//class LOrExp : Unit {
//    vector<LAndExp*> lAndExp;
//public:
//    explicit LOrExp(vector<LAndExp*> _lAndExp) : lAndExp(std::move(_lAndExp)) {}
//};
//
//class Exp : Unit {
//    AddExp* addExp;
//public:
//    explicit Exp(AddExp* _addExp) : addExp(_addExp) {}
//};
//
//class FuncRParams : Unit {
//    vector<Exp*> exp;
//public:
//    explicit FuncRParams(vector<Exp*> _exp) : exp(std::move(_exp)) {}
//};
//
//class UnaryOp : Unit {
//    Token* unaryOp;
//public:
//    explicit UnaryOp(Token* _unaryOp) : unaryOp(_unaryOp) {}
//};
//
//class Number : Unit {
//    Token* intConst;
//public:
//    explicit Number(Token* _intConst) : intConst(_intConst) {}
//};
//
//class Cond : Unit {
//    LOrExp* lOrExp;
//public:
//    explicit Cond(LOrExp* _lOrExp) : lOrExp(_lOrExp) {}
//};
//
//class LVal : Unit {
//    Ident* ident;
//    vector<Exp*> exp;
//public:
//    LVal(Ident* _ident, vector<Exp*> _exp)
//        : ident(_ident), exp(std::move(_exp)) {}
//};
//
//class PrimaryExp : Unit {
//    Exp* exp;
//    LVal* lval;
//    Number* number;
//public:
//    explicit PrimaryExp(Exp* _exp) : exp(_exp), lval(nullptr), number(nullptr) {}
//    explicit PrimaryExp(LVal* _lval) : exp(nullptr), lval(_lval), number(nullptr) {}
//    explicit PrimaryExp(Number* _number) : exp(nullptr), lval(nullptr), number(_number) {}
//};
//
//class UnaryExp : Unit {
//    PrimaryExp* primaryExp;
//    Ident* ident;
//    FuncRParams* funcRParams;
//    UnaryOp* unaryOp;
//    UnaryExp* unaryExp;
//public:
//    explicit UnaryExp(PrimaryExp* _primaryExp)
//            : primaryExp(_primaryExp), ident(nullptr), funcRParams(nullptr), unaryOp(nullptr), unaryExp(nullptr) {}
//    explicit UnaryExp(Ident* _ident, FuncRParams* _funcRParams)
//            : primaryExp(nullptr), ident(_ident), funcRParams(_funcRParams), unaryOp(nullptr), unaryExp(nullptr) {}
//    UnaryExp(UnaryOp* _unaryOp, UnaryExp* _unaryExp)
//        : primaryExp(nullptr), ident(nullptr), funcRParams(nullptr),  unaryOp(_unaryOp), unaryExp(_unaryExp) {}
//};
//
//class ForStmt : Unit {
//    LVal* lval;
//    Exp* exp;
//public:
//    ForStmt(LVal* _lval, Exp* _exp)
//        : lval(_lval), exp(_exp) {}
//};
//
//class AssignStmt : Unit {
//    LVal* lval;
//    Exp* exp;
//public:
//    AssignStmt(LVal* _lval, Exp* _exp)
//        : lval(_lval), exp(_exp) {}
//};
//
//class ExpStmt : Unit {
//    Exp* exp;
//public:
//    explicit ExpStmt(Exp* _exp) : exp(_exp) {}
//};
//
//class IfStmt : Unit {
//    Cond* cond;
//    Stmt* ifStmt;
//    Stmt* elseStmt;
//public:
//    IfStmt(Cond* _cond, Stmt* _ifStmt, Stmt* _elseStmt)
//        : cond(_cond),ifStmt(_ifStmt), elseStmt(_elseStmt) {}
//};
//
//class Forstmt : Unit {
//    ForStmt* forStmt1;
//    Cond* cond;
//    ForStmt* forStmt2;
//    Stmt* stmt;
//public:
//    Forstmt(ForStmt* _forStmt1, Cond* _cond, ForStmt* _forStmt2, Stmt* _stmt)
//        : forStmt1(_forStmt1), cond(_cond), forStmt2(_forStmt2), stmt(_stmt) {}
//};
//
//class BreakStmt : Unit {
//};
//
//class ContinueStmt : Unit {
//};
//
//class ReturnStmt : Unit {
//    Exp* exp;
//public:
//    explicit ReturnStmt(Exp* _exp) : exp(_exp) {}
//    explicit ReturnStmt() {}
//};
//
//class GetintStmt : Unit {
//    LVal* lval;
//public:
//    explicit GetintStmt(LVal* _lval) : lval(_lval) {}
//};
//
//class PrintfStmt : Unit {
//    Token* formatString;
//    vector<Exp*> exp;
//public:
//    PrintfStmt(Token* _formatString, vector<Exp*> _exp)
//        : formatString(_formatString), exp(std::move(_exp)) {}
//};
//
//class InitVal : Unit {
//    Exp* exp;
//    vector<InitVal*> initVal;
//public:
//    InitVal(Exp* _exp, vector<InitVal*> _initVal)
//        : exp(_exp), initVal(std::move(_initVal)) {}
//};
//
//class VarDef : Unit {
//    Ident* ident;
//    vector<ConstExp*> constExp;
//    InitVal* initVal;
//public:
//    VarDef(Ident* _ident, vector<ConstExp*> _constExp, InitVal* _initVal)
//    : ident(_ident), constExp(std::move(_constExp)), initVal(_initVal) {}
//};
//
//class ConstInitVal : Unit { // ?
//    ConstExp* constExp;
//    vector<ConstInitVal*> constInitVal;
//public:
//    ConstInitVal(ConstExp* _constExp, vector<ConstInitVal*> _constInitVal)
//    : constExp(_constExp), constInitVal(std::move(_constInitVal)) {}
//};
//
//class ConstDef : Unit {
//    Ident* ident;
//    vector<ConstExp*> constExp;
//    ConstInitVal* constInitVal;
//public:
//    ConstDef(Ident* _ident, vector<ConstExp*> _constExp, ConstInitVal* _constInitVal)
//            : ident(_ident), constExp(std::move(_constExp)), constInitVal(_constInitVal) {}
//};
//class ConstDecl : Unit {
//    vector<ConstDef*> constDef;
//public:
//    explicit ConstDecl(vector<ConstDef*> _constDef) : constDef(std::move(_constDef)) {}
//};
//
//class VarDecl : Unit {
//    vector<VarDef*> varDef;
//public:
//    explicit VarDecl(vector<VarDef*> _varDef) : varDef(std::move(_varDef)) {}
//};
//
//class Decl : Unit {
//    ConstDecl* constDecl;
//    VarDecl* varDecl;
//public:
//    explicit Decl(ConstDecl* _constDecl) : constDecl(_constDecl), varDecl(nullptr) {}
//    explicit Decl(VarDecl* _varDecl) : constDecl(nullptr), varDecl(_varDecl) {}
//};
//
//class BlockItem : Unit {
//    Decl* decl;
//    Stmt* stmt;
//public:
//    BlockItem(Decl* _decl) : decl(_decl) {}
//    BlockItem(Stmt* _stmt) : stmt(_stmt) {}
//};
//
//class Block : Unit {
//    vector<BlockItem*> blockItem;
//public:
//    explicit Block(vector<BlockItem*> _blockItem) : blockItem(std::move(_blockItem)) {}
//};
//
//class BlockStmt : Unit {
//    Block* block;
//public:
//    explicit BlockStmt(Block* _block) : block(_block) {}
//};
//
//class Stmt : Unit {
//    AssignStmt* assignStmt;
//    ExpStmt* expStmt;
//    BlockStmt* blockStmt;
//    IfStmt* ifStmt;
//    Forstmt* forstmt;
//    BreakStmt* breakStmt;
//    ContinueStmt* continueStmt;
//    ReturnStmt* returnStmt;
//    GetintStmt* getintStmt;
//    PrintfStmt* printfStmt;
//public:
//    explicit Stmt(AssignStmt* _assignStmt) : assignStmt(_assignStmt), expStmt(nullptr), blockStmt(nullptr),
//                                             ifStmt(nullptr), forstmt(nullptr), breakStmt(nullptr),
//                                             continueStmt(nullptr), returnStmt(nullptr),
//                                             getintStmt(nullptr), printfStmt(nullptr) {}
//    explicit Stmt(ExpStmt* _expStmt) : assignStmt(nullptr), expStmt(_expStmt), blockStmt(nullptr),
//                                       ifStmt(nullptr), forstmt(nullptr), breakStmt(nullptr),
//                                       continueStmt(nullptr), returnStmt(nullptr),
//                                       getintStmt(nullptr), printfStmt(nullptr) {}
//    explicit Stmt(BlockStmt* _blockStmt) : assignStmt(nullptr), expStmt(nullptr), blockStmt(_blockStmt),
//                                           ifStmt(nullptr), forstmt(nullptr), breakStmt(nullptr),
//                                           continueStmt(nullptr), returnStmt(nullptr),
//                                           getintStmt(nullptr), printfStmt(nullptr) {}
//    explicit Stmt(IfStmt* _ifStmt) : assignStmt(nullptr), expStmt(nullptr), blockStmt(nullptr),
//                                     ifStmt(_ifStmt), forstmt(nullptr), breakStmt(nullptr),
//                                     continueStmt(nullptr), returnStmt(nullptr),
//                                     getintStmt(nullptr), printfStmt(nullptr) {}
//    explicit Stmt(Forstmt* _forStmt) : assignStmt(nullptr), expStmt(nullptr), blockStmt(nullptr),
//                                       ifStmt(nullptr), forstmt(_forStmt), breakStmt(nullptr),
//                                       continueStmt(nullptr), returnStmt(nullptr),
//                                       getintStmt(nullptr), printfStmt(nullptr) {}
//    explicit Stmt(BreakStmt* _breakStmt) : assignStmt(nullptr), expStmt(nullptr), blockStmt(nullptr),
//                                           ifStmt(nullptr), forstmt(nullptr), breakStmt(_breakStmt),
//                                           continueStmt(nullptr), returnStmt(nullptr),
//                                           getintStmt(nullptr), printfStmt(nullptr) {}
//    explicit Stmt(ContinueStmt* _continueStmt) : assignStmt(nullptr), expStmt(nullptr), blockStmt(nullptr),
//                                                 ifStmt(nullptr), forstmt(nullptr), breakStmt(nullptr),
//                                                 continueStmt(_continueStmt), returnStmt(nullptr),
//                                                 getintStmt(nullptr), printfStmt(nullptr) {}
//    explicit Stmt(ReturnStmt* _returnStmt) : assignStmt(nullptr), expStmt(nullptr), blockStmt(nullptr),
//                                             ifStmt(nullptr), forstmt(nullptr), breakStmt(nullptr),
//                                             continueStmt(nullptr), returnStmt(_returnStmt),
//                                             getintStmt(nullptr), printfStmt(nullptr) {}
//    explicit Stmt(GetintStmt* _getintStmt) : assignStmt(nullptr), expStmt(nullptr), blockStmt(nullptr),
//                                             ifStmt(nullptr), forstmt(nullptr), breakStmt(nullptr),
//                                             continueStmt(nullptr), returnStmt(nullptr),
//                                             getintStmt(_getintStmt), printfStmt(nullptr) {}
//    explicit Stmt(PrintfStmt* _printfStmt) : assignStmt(nullptr), expStmt(nullptr), blockStmt(nullptr),
//                                             ifStmt(nullptr), forstmt(nullptr), breakStmt(nullptr),
//                                             continueStmt(nullptr), returnStmt(nullptr),
//                                             getintStmt(nullptr), printfStmt(_printfStmt) {}
//};
//
//class FuncFParam : Unit { // ?
//    Ident* ident;
//    ConstExp* constExp;
//public:
//    FuncFParam(Ident* _ident, ConstExp* _constExp)
//        : ident(_ident), constExp(_constExp) {}
//};
//
//class FuncFParams : Unit {
//    vector<FuncFParam*> funcFParam;
//public:
//    explicit FuncFParams(vector<FuncFParam*> _funcFParam) {
//        funcFParam = std::move(_funcFParam);
//    }
//};
//
//class MainFuncDef : Unit {
//    Block* block;
//public:
//    explicit MainFuncDef(Block* _block) : block(_block) {}
//};
//
//class FuncDef : Unit {
//    FuncType* funcType;
//    Ident* ident;
//    FuncFParams* funcFParams;
//    Block* block;
//public:
//    FuncDef(FuncType* _funcType, Ident* _ident, FuncFParams* _funcFParams, Block* _block)
//        : funcType(_funcType), ident(_ident), funcFParams(_funcFParams), block(_block) {}
//    FuncDef(FuncType* _funcType, Ident* _ident, Block* _block)
//            : funcType(_funcType), ident(_ident), block(_block) {}
//};
//
//class CompUnit : Unit { // 编译单元
//    vector<Decl*> decl;
//    vector<FuncDef*> funcDef;
//    MainFuncDef* mainFuncDef;
//public:
//    CompUnit(vector<Decl*> _decl, vector<FuncDef*> _funcDef, MainFuncDef* _mainFuncDef)
//        {
//        decl = std::move(_decl);
//        funcDef = std::move(_funcDef);
//        mainFuncDef = _mainFuncDef;
//        }
//};

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
