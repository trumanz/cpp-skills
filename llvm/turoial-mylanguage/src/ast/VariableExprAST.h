//
// Created by zck on 2022/3/21.
//

#pragma once

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
