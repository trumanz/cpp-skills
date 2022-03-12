//
// Created by zck on 2021/12/5.
//

#ifndef TUROIAL_MYLANGUAGE_LEXER_H
#define TUROIAL_MYLANGUAGE_LEXER_H
#include <string>

enum  Token {
    tok_eof = -1,
    //commands
    tok_def = -2,
    tok_extern = -3,
    //primary
    tok_identifier = -4,
    tok_number = -5,
    tok_char = -6,
 };
bool operator==(const Token& l, const char& r) ;
bool operator!=(const Token& l, const char& r) ;


class Lexer {
    Token gettok_imp();
    double NumVal;
    std::string IdentifierStr;
    char char_val;
public:
    Token gettok();
    Token CurTok;
    double currentValueAsDouble() {
        return NumVal;
    }
    std::string currentValueAsId() {
        return IdentifierStr;
    };
    char currentValueAsChar() {
        return this->char_val;
    }
    static Lexer instance;
};


#endif //TUROIAL_MYLANGUAGE_LEXER_H
