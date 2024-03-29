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
    int count_char = 0;
    std::istream* in_stream = nullptr;
    char LastChar;
public:
    Token CurTok;
    Token gettok();
    void setInput(std::istream* in) {
        this->in_stream = in;
        this->LastChar = ' ';
        this->count_char = 0;
    }
    int countChar() const  { return this->count_char;}
    static Lexer instance;
};

#endif //TUROIAL_MYLANGUAGE_LEXER_H
