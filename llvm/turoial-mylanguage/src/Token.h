#pragma once

class  Token {
public:
    enum Type {
        tok_eof = -1,
        //commands
        tok_def = -2,
        tok_extern = -3,
        //primary
        tok_identifier = -4,
        tok_number = -5,
        tok_char = -6,
    };
    Type token_type ;
    struct Value {
        double num;
        char c;
        std::string str;
    } value;
    Token(Type type = tok_eof) {
        this->token_type = type;
        this->value.num = 0.0;
    }
    Token(double num) {
        this->token_type = tok_number;
        this->value.num = num;
    }
    Token(char c) {
        this->token_type = tok_char;
        this->value.c = c;
    }
    Token(Type type, std::string val) {
        this->token_type = type;
        if(type == Token::tok_def) {
            assert(val == "def");
        }
        else if(type == Token::tok_extern) {
            assert(val == "extern");
        }
        else {
            assert(type == tok_identifier);
        }
        this->value.str = val;
    }
    std::string toString() const {
        switch(this->token_type) {
            case tok_eof:
                return "eof";
            case tok_def:
                return "def";
            case tok_extern:
                return "extern";
            case tok_identifier:
                return "identifier";
            case tok_number:
                return "number";
            case tok_char:
                return "char";
            default:
                return "unknown";
        }
    }

};
inline bool operator==(const Token& a, const char& b){
    return a.token_type == b;
};
inline bool operator!=(const Token& a, const char& b){
    return a.token_type != b;
};