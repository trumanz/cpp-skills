//
// Created by zck on 2022/3/21.
//

#pragma once

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
                LogErrorV("invalid binary operator");
                return nullptr;

        }
    }
};



