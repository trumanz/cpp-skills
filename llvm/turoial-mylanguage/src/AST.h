//
// Created by zck on 2021/12/5.
//

#ifndef TUROIAL_MYLANGUAGE_AST_H
#define TUROIAL_MYLANGUAGE_AST_H

#include <iostream>
#include <vector>

#include "llvm/ADT/APFloat.h"
#include "llvm/ADT/STLExtras.h"
#include "llvm/IR/BasicBlock.h"
#include "llvm/IR/Constants.h"
#include "llvm/IR/DerivedTypes.h"
#include "llvm/IR/Function.h"
#include "llvm/IR/IRBuilder.h"
#include "llvm/IR/LLVMContext.h"
#include "llvm/IR/Module.h"
#include "llvm/IR/Type.h"
#include "llvm/IR/Verifier.h"

static llvm::LLVMContext TheContext;
static llvm::IRBuilder<> Builder(TheContext);
static std::unique_ptr<llvm::Module> TheModule;
static std::map<std::string, llvm::Value*> NamedValues;
class ExprAST;
std::unique_ptr<ExprAST> LogError(const char *Str);

llvm::Value* LogErrorV(const char *Str);

class ExprAST {
public:
    virtual ~ExprAST(){}
    virtual llvm::Value *codegen() = 0;
};

class NumberExprAST : public ExprAST {
    double Val;
public:
    NumberExprAST(double Val) : Val(Val){}
    llvm::Value *codegen() override {
        return llvm::ConstantFP::get(TheContext, llvm::APFloat(Val));
    }
};

class VariableExprAST : public ExprAST {
    std::string Name;
public:
    VariableExprAST(const std::string &Name) : Name(Name){}
    llvm::Value* codegen() override {
        llvm::Value* V = NamedValues[Name];
        if(!V) {
            LogErrorV("Unknown variable name");
        }
        return V;
    }
};
class BinaryExprAST: public ExprAST {
    char Op;
    std::unique_ptr<ExprAST>  LHS, RHS;
public:
    BinaryExprAST(const char op, std::unique_ptr<ExprAST> LHS,
                  std::unique_ptr<ExprAST> RHS)
                  : Op(op), LHS(std::move(LHS)), RHS(std::move(RHS)) {}

    llvm::Value* codegen() {
        llvm::Value* L = LHS->codegen();
        llvm::Value* R = RHS->codegen();
        if(!L || !R) return nullptr;

        switch(Op) {
            case '+':
                return Builder.CreateFAdd(L, R, "addtmp");
            case '-':
                return Builder.CreateFSub(L, R, "subtmp");
            case '*':
                return Builder.CreateFMul(L, R, "cmptmp");
            case '<':
                L = Builder.CreateFCmpULT(L, R, "cmptmp" );
                //convert bool 0/1 to double 0.0 or 1.0
                return Builder.CreateUIToFP(L, llvm::Type::getDoubleTy(TheContext),
                                            "booltmp");
            default:
                return LogErrorV("invalid binary operator");

        }
    }
};


#include "CallExprAST.h"

class PrototypeAST {
    std::string Name;
    std::vector<std::string> Args;
public:
    PrototypeAST(const std::string &name, std::vector<std::string> Args)
    : Name(name), Args(std::move(Args)) {}
};

class FunctionAST {
    std::unique_ptr<PrototypeAST> Proto;
    std::unique_ptr<ExprAST> Body;
public:
    FunctionAST(std::unique_ptr<PrototypeAST> Proto,
                std::unique_ptr<ExprAST> Body)
                : Proto(std::move(Proto)), Body(std::move(Body)) {}
};

#endif //TUROIAL_MYLANGUAGE_AST_H
