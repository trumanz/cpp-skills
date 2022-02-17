//
// Created by zck on 2022/2/17.
//

#ifndef TUROIAL_MYLANGUAGE_CALLEXPRAST_H
#define TUROIAL_MYLANGUAGE_CALLEXPRAST_H

#include "AST.h"

class CallExprAST : public ExprAST {
    std::string Callee;
    std::vector<std::unique_ptr<ExprAST>> Args;
public:
    CallExprAST(const std::string &Callee,
                std::vector<std::unique_ptr<ExprAST>> Args)
            : Callee(Callee), Args(std::move(Args)) {}

    llvm::Value * codegen() override  {
       llvm::Function *CalleeF = TheModule->getFunction(this->Callee);
       if(!CalleeF) {
           return LogErrorV("Unknown function referenced : "  );
       }

       if(CalleeF->arg_size() != this->Args.size()) {
           return LogErrorV("Incorrect #arugments passed");
       }

       std::vector<llvm::Value*> ArgsV;
       for(size_t i = 0; i < Args.size(); ++i) {
           ArgsV.push_back(Args[i]->codegen());
           if(!ArgsV.back()) {
               return nullptr;
           }
       }
       return Builder.CreateCall(CalleeF, ArgsV, "calltmp");
    }
};

#endif //TUROIAL_MYLANGUAGE_CALLEXPRAST_H
