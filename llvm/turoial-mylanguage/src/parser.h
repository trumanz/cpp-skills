//
// Created by zck on 2021/12/5.
//

#ifndef TUROIAL_MYLANGUAGE_PARSER_H
#define TUROIAL_MYLANGUAGE_PARSER_H
#include <map>
#include <map>
#include "AST.h"

class Parser {
    std::map<char, int> BinopPrecedence;
    std::unique_ptr<ExprAST> ParseNumberExpr(double num) ;
    std::unique_ptr<ExprAST> ParseParenExpr() ;
    std::unique_ptr<ExprAST> ParseIdentifierExpr(std::string id) ;
    std::unique_ptr<ExprAST> ParsePrimary() ;
    int GetTokPrecedence() ;

    std::unique_ptr<ExprAST> ParseExpression() ;
    std::unique_ptr<ExprAST> ParseBinOpRHS(int ExprPrec,
                                           std::unique_ptr<ExprAST> LHS) ;
    std::unique_ptr<PrototypeAST> ParsePrototype() ;
    std::unique_ptr<FunctionAST> ParseDefinition() ;
    std::unique_ptr<PrototypeAST> ParseExtern() ;
    std::unique_ptr<FunctionAST> ParseTopLevelExpr() ;
//===----------------------------------===//
// Top-Level parsing
//===----------------------------------===//
    void HandleDefinition() ;
    void HandleExtern();
    void HandleTopLevelExpression();
    void InitBinopPrecedence();
    std::unique_ptr<PrototypeAST> LogErrorP(const char *Str);
public:
    Parser() {
        InitBinopPrecedence();
    }
    void parse(const std::string& expr);
};
#endif //TUROIAL_MYLANGUAGE_PARSER_H
