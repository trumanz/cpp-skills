//
// Created by zck on 2021/12/5.
//

#ifndef TUROIAL_MYLANGUAGE_LEXER_H
#define TUROIAL_MYLANGUAGE_LEXER_H
#include <string>

enum Token {
    tok_eof = -1,
    //commands
    tok_def = -2,
    tok_extern = -3,
    //primary
    tok_identifier = -4,
    tok_number = -5,
 };
extern std::string IdentifierStr;
extern double NumVal;


int gettok();


#endif //TUROIAL_MYLANGUAGE_LEXER_H
