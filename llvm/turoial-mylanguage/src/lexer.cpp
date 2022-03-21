//
// Created by zck on 2021/12/5.
//

#include "lexer.h"
#include "AST.h"
#include <log4cplus/log4cplus.h>
static  auto rootLogger = log4cplus::Logger().getRoot();


Lexer Lexer::instance;
char Lexer::read_char() {
     this->count_char++;
     return in_stream->get();
}
Token Lexer::gettok_imp(){
    //static char LastChar = ' ';
    //skip any whitespace
    while(std::isspace(LastChar)) {
        LastChar = read_char();
    }
    std::string IdentifierStr;
    if(std::isalpha(LastChar)) { // [a-zA-Z][a-zA-Z0-9]
        IdentifierStr = LastChar;
        while(std::isalnum(LastChar = read_char())) {
            IdentifierStr += LastChar;
        }
        if(IdentifierStr == "def") {
            return Token::Type::tok_def;
        }
        if(IdentifierStr == "extern") {
            return Token::Type::tok_extern;
        }
        return Token(Token::Type::tok_identifier, IdentifierStr);
    }

    if(std::isdigit(LastChar) || LastChar == '.') {
        std::string NumStr;
        do {
            NumStr += LastChar;
            LastChar = read_char();
        } while (std::isdigit(LastChar) || LastChar == '.');
        double NumVal = strtod(NumStr.c_str(), 0);
        return Token(NumVal);
    }

    if(LastChar == '#') {
        do {
            LastChar = read_char();
        } while(LastChar != EOF && LastChar != '\n' && LastChar != '\r');
        if(LastChar != EOF)
            return gettok();
    }

    if(LastChar == EOF) {
        return Token::Type::tok_eof;
    }

    char char_val = LastChar;
    LastChar = read_char();
    return Token(char_val);
    //return char_val;
}

Token Lexer::gettok() {
    CurTok =  gettok_imp();
    LOG4CPLUS_DEBUG_FMT(rootLogger, "gettok Curtok: %s", CurTok.toString().c_str());
    return CurTok;
}