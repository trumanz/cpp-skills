//
// Created by zck on 2021/12/5.
//

#include "lexer.h"
#include "AST.h"


std::string IdentifierStr;
double NumVal;

int gettok(){
    static char LastChar = ' ';
    //skip any whitespace
    while(std::isspace(LastChar)) {
        LastChar = getchar();
    }

    if(std::isalpha(LastChar)) { // [a-zA-Z][a-zA-Z0-9]
        IdentifierStr = LastChar;
        while(std::isalnum(LastChar = getchar())) {
            IdentifierStr += LastChar;
        }
        if(IdentifierStr == "def") {
            return tok_def;
        }
        if(IdentifierStr == "extern") {
            return tok_extern;
        }
        return tok_identifier;
    }

    if(std::isdigit(LastChar) || LastChar == '.') {
        std::string NumStr;
        do {
            NumStr += LastChar;
            LastChar = getchar();
        } while (std::isdigit(LastChar) || LastChar == '.');
        NumVal = strtod(NumStr.c_str(), 0);
        return tok_number;
    }

    if(LastChar == '#') {
        do {
            LastChar = getchar();
        } while(LastChar != EOF && LastChar != '\n' && LastChar != '\r');
        if(LastChar != EOF)
            return gettok();
    }

    if(LastChar == EOF) {
        return tok_eof;
    }

    int ThisChar = LastChar;
    LastChar = getchar();
    return ThisChar;
}
