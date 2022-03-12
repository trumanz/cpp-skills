//
// Created by zck on 2021/12/5.
//

#include "lexer.h"
#include "AST.h"

Lexer Lexer::instance;
bool operator==(const Token& l, const char& r) {
    return (int)l == r;
}
bool operator!=(const Token& l, const char& r) {
    return (int)l != r;
}
std::istream* in_stream = nullptr;
char read_char() {
     if(in_stream == nullptr) {
         return read_char();
     }
     return in_stream->get();
}

Token Lexer::gettok_imp(){
    static char LastChar = ' ';
    //skip any whitespace
    while(std::isspace(LastChar)) {
        LastChar = read_char();
    }

    if(std::isalpha(LastChar)) { // [a-zA-Z][a-zA-Z0-9]
        IdentifierStr = LastChar;
        while(std::isalnum(LastChar = read_char())) {
            IdentifierStr += LastChar;
        }
        if(IdentifierStr == "def") {
            return Token::tok_def;
        }
        if(IdentifierStr == "extern") {
            return Token::tok_extern;
        }
        return Token::tok_identifier;
    }

    if(std::isdigit(LastChar) || LastChar == '.') {
        std::string NumStr;
        do {
            NumStr += LastChar;
            LastChar = read_char();
        } while (std::isdigit(LastChar) || LastChar == '.');
        NumVal = strtod(NumStr.c_str(), 0);
        return Token::tok_number;
    }

    if(LastChar == '#') {
        do {
            LastChar = read_char();
        } while(LastChar != EOF && LastChar != '\n' && LastChar != '\r');
        if(LastChar != EOF)
            return gettok();
    }

    if(LastChar == EOF) {
        return Token::tok_eof;
    }

    this->char_val = LastChar;
    LastChar = read_char();
    return Token::tok_char;
    //return char_val;
}

Token Lexer::gettok() {
    CurTok =  gettok_imp();
    return CurTok;
}