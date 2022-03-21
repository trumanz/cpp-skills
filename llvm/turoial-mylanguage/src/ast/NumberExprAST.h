//
// Created by zck on 2022/3/21.
//
#pragma  once
#include "ExprAST.h"
class NumberExprAST : public ExprAST {
    double Val;
public:
    NumberExprAST(double Val) : Val(Val){}
    llvm::Value *codegen() override {
        return llvm::ConstantFP::get(TheContext, llvm::APFloat(Val));
    }
};


