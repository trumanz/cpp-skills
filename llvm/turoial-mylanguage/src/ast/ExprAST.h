//
// Created by zck on 2022/3/21.
//

#pragma once

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
#include <log4cplus/log4cplus.h>

static llvm::LLVMContext TheContext;
static llvm::IRBuilder<> Builder(TheContext);
static std::unique_ptr<llvm::Module> TheModule;
static std::map<std::string, llvm::Value*> NamedValues;
class ExprAST;
//std::unique_ptr<ExprAST> LogError(const char *Str);

class ExprAST {
protected:
    void LogErrorV(const char *Str) {
        LOG4CPLUS_ERROR_FMT(log4cplus::Logger::getRoot(), "LogError: %s", Str);
    }
public:
    virtual ~ExprAST(){}
    virtual llvm::Value *codegen() = 0;
};



