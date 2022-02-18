//
// Created by zck on 2021/12/5.
//

#include "AST.h"

std::unique_ptr<ExprAST> LogError(const char *Str) {
    fprintf(stderr, "LogError: %s\n", Str);
    return nullptr;
}

llvm::Value* LogErrorV(const char *Str) {
    LogError(Str);
    return nullptr;
}
