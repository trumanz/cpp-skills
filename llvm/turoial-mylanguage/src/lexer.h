//
// Created by zck on 2021/12/5.
//

#ifndef TUROIAL_MYLANGUAGE_LEXER_H
#define TUROIAL_MYLANGUAGE_LEXER_H
#include <string>
#include "Token.h"

class Lexer {
    Token gettok_imp();
    char read_char();
public:
    Token gettok();
    Token CurTok;
    std::istream* in_stream = nullptr;
    static Lexer instance;
};

#endif //TUROIAL_MYLANGUAGE_LEXER_H
