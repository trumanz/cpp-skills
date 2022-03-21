//
// Created by zck on 2021/12/5.
//


#include "lexer.h"
#include <log4cplus/log4cplus.h>
#include  "parser.h"
static std::unique_ptr<ExprAST> ParseExpression();
static std::unique_ptr<ExprAST> ParseBinOpRHS(int ExprPrec,
                                              std::unique_ptr<ExprAST> LHS) ;


static  auto rootLogger = log4cplus::Logger().getRoot();


//auto LHS = std::make_unique<VariableExprAST>("x");
//auto RHS = std::make_unique<VariableExprAST>("y");

//auto Result = std::make_unique<BinaryExprAST>('+', std::move(LHS),
//                                              std::move(RHS));




std::unique_ptr<PrototypeAST> Parser::LogErrorP(const char *Str) {

    LOG4CPLUS_ERROR_FMT( rootLogger, "position %d, currentToken: %s,  %s",
                         Lexer::instance.countChar(),
                         Lexer::instance.CurTok.toString().c_str(),
                         Str);
    return nullptr;
}

std::unique_ptr<ExprAST> Parser::ParseNumberExpr(double num) {
    auto Result = std::make_unique<NumberExprAST>(num);
    Lexer::instance.gettok();
    return std::move(Result);
}

/// parenexpr ::= '(' experssion ')'
std::unique_ptr<ExprAST> Parser::ParseParenExpr() {
    Lexer::instance.gettok(); // eat (.
    auto V = ParseExpression();
    if(!V)
        return nullptr;
    if(Lexer::instance.CurTok != ')') {
        LOG4CPLUS_ERROR_FMT(rootLogger, "expected ')'");
        return nullptr;
    }
    Lexer::instance.gettok(); //eat ).
    return V;
}

//// identifierexpr
///    ::= identifier
///    ::= identifier '(' experssion* ')'
std::unique_ptr<ExprAST> Parser::ParseIdentifierExpr(std::string id) {
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
            if(Lexer::instance.CurTok != ',') {
                LOG4CPLUS_ERROR_FMT(rootLogger, "Expected ')' or ',' in argument list" );
                return nullptr;
            }
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
std::unique_ptr<ExprAST> Parser::ParsePrimary() {
    switch(Lexer::instance.CurTok.token_type) {
        default:
            LOG4CPLUS_ERROR_FMT(rootLogger,"unknown token when expecting an expression");
            return nullptr;
        case Token::Type::tok_identifier:
            return ParseIdentifierExpr(Lexer::instance.CurTok.value.str);
        case Token::Type::tok_number:
            return ParseNumberExpr(Lexer::instance.CurTok.value.num);
        case Token::Type::tok_char:
            if ('(' == Lexer::instance.CurTok.value.c)
              return ParseParenExpr();
    }
    return nullptr;
}


int Parser::GetTokPrecedence() {
    assert(Lexer::instance.CurTok.token_type ==  Token::Type::tok_char);
    char char_val = Lexer::instance.CurTok.value.c;
    if(!isascii(char_val))
        return -1;
    int TokPrec = BinopPrecedence[char_val];
    if(TokPrec <= 0) return -1;
    return TokPrec;
}

std::unique_ptr<ExprAST> Parser::ParseExpression() {
    auto LHS = ParsePrimary();
    if(!LHS)
        return nullptr;
    return ParseBinOpRHS(0, std::move(LHS));
}

std::unique_ptr<ExprAST> Parser::ParseBinOpRHS(int ExprPrec,
                                              std::unique_ptr<ExprAST> LHS) {
    while(1) {
        int TokPrec = GetTokPrecedence();

        if(TokPrec < ExprPrec)
            return LHS;

        int BinOp = Lexer::instance.CurTok.valueAsChar();
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

std::unique_ptr<PrototypeAST> Parser::ParsePrototype() {
    if(Lexer::instance.CurTok != Token::Type::tok_identifier)
        return LogErrorP("Expected function name in prototype");
    std::string FnName = Lexer::instance.CurTok.value.str;
    LOG4CPLUS_DEBUG_FMT(rootLogger, "function name:%s", FnName.c_str());
    Lexer::instance.gettok();
    if(Lexer::instance.CurTok != '(')
        return LogErrorP("Expected '(' in prototype");

    std::vector<std::string> ArgNames;
    while(Lexer::instance.gettok() == Token::Type::tok_identifier)
        ArgNames.push_back(Lexer::instance.CurTok.value.str);
    if(Lexer::instance.CurTok != ')')
        return LogErrorP("Expected ')' in prototype");
    Lexer::instance.gettok(); //eat ')'
    return std::make_unique<PrototypeAST>(FnName, std::move(ArgNames));
}

std::unique_ptr<FunctionAST> Parser::ParseDefinition() {
    Lexer::instance.gettok();
    auto Proto = ParsePrototype();
    if(!Proto) return nullptr;
    if(auto E = ParseExpression())
        return std::make_unique<FunctionAST>(std::move(Proto), std::move(E));
    return nullptr;
}

std::unique_ptr<PrototypeAST> Parser::ParseExtern() {
    Lexer::instance.gettok();
    return ParsePrototype();
}


std::unique_ptr<FunctionAST> Parser::ParseTopLevelExpr() {
    if(auto E = ParseExpression()) {
        auto v = E->codegen();
        std::string str;
        llvm::raw_string_ostream rso(str);
        v->print(rso, true);
        LOG4CPLUS_DEBUG_FMT(rootLogger, "%s", str.c_str());

      //  auto Proto = std::make_unique<PrototypeAST>("", std::vector<std::string>());
      //  return std::make_unique<FunctionAST>(std::move(Proto), std::move(E));
    }
    return nullptr;
}

//===----------------------------------===//
// Top-Level parsing
//===----------------------------------===//
void Parser::HandleDefinition() {
    if(ParseDefinition()) {
        LOG4CPLUS_DEBUG_FMT(rootLogger, "Parsed a function definition");
    } else  {
        Lexer::instance.gettok();
    }
}

void Parser::HandleExtern() {
    if(ParseExtern()) {
        LOG4CPLUS_INFO_FMT(rootLogger, "Parsed an extern");
    } else {
        Lexer::instance.gettok();
    }
}

void Parser::HandleTopLevelExpression() {
    if(ParseTopLevelExpr()) {
        LOG4CPLUS_INFO_FMT(rootLogger,"Parsed a top-level expr");
    } else {
        Lexer::instance.gettok();

    }
}



void Parser::InitBinopPrecedence() {

// Install standard binary operators.
// 1 is lowest precedence.
BinopPrecedence['<'] = 10;
BinopPrecedence['+'] = 20;
BinopPrecedence['-'] = 20;
BinopPrecedence['*'] = 40;  // highest.

}


void Parser::parse(const std::string& expr)
{
    std::stringstream  ss;
    ss << expr;
    Lexer::instance.setInput(&ss);
    LOG4CPLUS_INFO_FMT(rootLogger, "Start parsing \"%s\"", expr.c_str());
    Lexer::instance.gettok();
   // while(1)
    {
        //LOG4CPLUS_INFO_FMT(rootLogger, "ready>");
        LOG4CPLUS_INFO_FMT(rootLogger, "token %s",
                           Lexer::instance.CurTok.toString().c_str());
        switch(Lexer::instance.CurTok.token_type) {
            case Token::Type::tok_eof:
                return;
            case Token::Type::tok_char:
                if(Lexer::instance.CurTok.value.c == ';') {
                    Lexer::instance.gettok();
                }
                break;
            case Token::Type::tok_def:
                HandleDefinition();
                break;
            case Token::Type::tok_extern:
                HandleExtern();
                break;
            default:
                HandleTopLevelExpression();
                break;
        }
    }
}
