//
// Created by zck on 2021/12/5.
//

#include <map>
#include "AST.h"
#include "lexer.h"
#include <log4cplus/log4cplus.h>

static std::unique_ptr<ExprAST> ParseExpression();
static std::unique_ptr<ExprAST> ParseBinOpRHS(int ExprPrec,
                                              std::unique_ptr<ExprAST> LHS) ;


static  auto rootLogger = log4cplus::Logger().getRoot();


//auto LHS = std::make_unique<VariableExprAST>("x");
//auto RHS = std::make_unique<VariableExprAST>("y");

//auto Result = std::make_unique<BinaryExprAST>('+', std::move(LHS),
//                                              std::move(RHS));




std::unique_ptr<PrototypeAST> LogErrorP(const char *Str) {
    LogError(Str);
    return nullptr;
}

static std::unique_ptr<ExprAST> ParseNumberExpr(double num) {
    auto Result = std::make_unique<NumberExprAST>(num);
    Lexer::instance.gettok();
    return std::move(Result);
}

/// parenexpr ::= '(' experssion ')'
static std::unique_ptr<ExprAST> ParseParenExpr() {
    Lexer::instance.gettok(); // eat (.
    auto V = ParseExpression();
    if(!V)
        return nullptr;
    if(Lexer::instance.CurTok != ')')
        return LogError("expected ')'");
    Lexer::instance.gettok(); //eat ).
    return V;
}

//// identifierexpr
///    ::= identifier
///    ::= identifier '(' experssion* ')'
static std::unique_ptr<ExprAST> ParseIdentifierExpr(std::string id) {
    std::string IdName = id;
    Lexer::instance.gettok(); //eat identifier.
    if(Lexer::instance.CurTok != '(')
        return std::make_unique<VariableExprAST>(IdName);
    //Call.
    Lexer::instance.gettok(); // eat (
    std::vector<std::unique_ptr<ExprAST>> Args;
    if(Lexer::instance.CurTok != ')') {
        while(1) {
            if(auto Arg = ParseExpression())
                Args.push_back(std::move(Arg));
            else
                return nullptr;
            if(Lexer::instance.CurTok == ')')
                break;
            if(Lexer::instance.CurTok != ',')
                return LogError("Expected ')' or ',' in argument list" );
            Lexer::instance.gettok();
        }
    }
    Lexer::instance.gettok(); //Eat the ')'
    return std::make_unique<CallExprAST>(IdName, std::move(Args));
}

/// Primary
///  ::= identifierexpr
///  ::= numberexpr
///  ::= parenexpr
static std::unique_ptr<ExprAST> ParsePrimary() {
    switch(Lexer::instance.CurTok) {
        default:
            return LogError("unknown token when expecting an expression");
        case Token::tok_identifier:
            return ParseIdentifierExpr(Lexer::instance.currentValueAsId());
        case Token::tok_number:
            return ParseNumberExpr(Lexer::instance.currentValueAsDouble());
        case  '(':
            return ParseParenExpr();
    }
}

static std::map<char, int> BinopPrecedence;

static int GetTokPrecedence() {
    if(!isascii(Lexer::instance.CurTok))
        return -1;
    int TokPrec = BinopPrecedence[Lexer::instance.CurTok];
    if(TokPrec <= 0) return -1;
    return TokPrec;
}

static std::unique_ptr<ExprAST> ParseExpression() {
    auto LHS = ParsePrimary();
    if(!LHS)
        return nullptr;
    return ParseBinOpRHS(0, std::move(LHS));
}

static std::unique_ptr<ExprAST> ParseBinOpRHS(int ExprPrec,
                                              std::unique_ptr<ExprAST> LHS) {
    while(1) {
        int TokPrec = GetTokPrecedence();

        if(TokPrec < ExprPrec)
            return LHS;

        int BinOp = Lexer::instance.CurTok;
        Lexer::instance.gettok(); //eat binop
        auto RHS = ParsePrimary();
        if(!RHS)
            return nullptr;
        int NextPrec = GetTokPrecedence();
        if(TokPrec < NextPrec) {
            RHS = ParseBinOpRHS(TokPrec + 1, std::move(RHS));
            if(!RHS)
                return nullptr;
        }
        LHS = std::make_unique<BinaryExprAST>(BinOp, std::move(LHS),
                                             std::move(RHS));
    }
}

static std::unique_ptr<PrototypeAST> ParsePrototype() {
    if(Lexer::instance.CurTok != Token::tok_identifier)
        return LogErrorP("Expected function name in prototype");
    std::string FnName = Lexer::instance.currentValueAsId();
    Lexer::instance.gettok();
    if(Lexer::instance.CurTok != '(')
        return LogErrorP("Expected '(' in prototype");

    std::vector<std::string> ArgNames;
    while(Lexer::instance.gettok() == Token::tok_identifier)
        ArgNames.push_back(Lexer::instance.currentValueAsId());
    if(Lexer::instance.CurTok != ')')
        return LogErrorP("Expected ')' in prototype");
    Lexer::instance.gettok(); //eat ')'
    return std::make_unique<PrototypeAST>(FnName, std::move(ArgNames));
}

static std::unique_ptr<FunctionAST> ParseDefinition() {
    Lexer::instance.gettok();
    auto Proto = ParsePrototype();
    if(!Proto) return nullptr;
    if(auto E = ParseExpression())
        return std::make_unique<FunctionAST>(std::move(Proto), std::move(E));
    return nullptr;
}

static std::unique_ptr<PrototypeAST> ParseExtern() {
    Lexer::instance.gettok();
    return ParsePrototype();
}


static std::unique_ptr<FunctionAST> ParseTopLevelExpr() {
    if(auto E = ParseExpression()) {
        auto Proto = std::make_unique<PrototypeAST>("", std::vector<std::string>());
        return std::make_unique<FunctionAST>(std::move(Proto), std::move(E));
    }
    return nullptr;
}

//===----------------------------------===//
// Top-Level parsing
//===----------------------------------===//
static void HandleDefinition() {
    if(ParseDefinition()) {
        fprintf(stderr, "Parsed a function definition.\n");
    } else  {
        Lexer::instance.gettok();
    }
}

static void HandleExtern() {
    if(ParseExtern()) {
        fprintf(stderr,  "Parsed an extern\n");
    } else {
        Lexer::instance.gettok();
    }
}

static void HandleTopLevelExpression() {
    if(ParseTopLevelExpr()) {
        fprintf(stderr, "Parsed a top-level expr\n");
    } else {
        Lexer::instance.gettok();
    }
}



void InitBinopPrecedence() {

// Install standard binary operators.
// 1 is lowest precedence.
BinopPrecedence['<'] = 10;
BinopPrecedence['+'] = 20;
BinopPrecedence['-'] = 20;
BinopPrecedence['*'] = 40;  // highest.

}




void MainLoop() {
    //fprintf(stderr, "ready> ");
    LOG4CPLUS_INFO_FMT(rootLogger, "ready>");
    Lexer::instance.gettok();
    while(1) {
        //LOG4CPLUS_INFO_FMT(rootLogger, "ready>");
        LOG4CPLUS_INFO_FMT(rootLogger, "tooken %d", Lexer::instance.CurTok);
        switch(Lexer::instance.CurTok) {
            case Token::tok_eof:
                return;
            case ';':
                Lexer::instance.gettok();
                break;
            case Token::tok_def:
                HandleDefinition();
                break;
            case Token::tok_extern:
                HandleExtern();
                break;
            default:
                HandleTopLevelExpression();
                break;
        }
    }
}


