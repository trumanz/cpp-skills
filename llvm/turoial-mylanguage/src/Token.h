#pragma once

class  Token {
public:
    enum class Type {
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
    Token(Type type = Type::tok_eof) {
        this->token_type = type;
    }
    Token(double num) {
        this->token_type = Type::tok_number;
        this->value.num = num;
    }
    Token(char c) {
        this->token_type = Type::tok_char;
        this->value.c = c;
    }
    Token(Type type, std::string val) {
        this->token_type = type;
        if(type == Token::Type::tok_def) {
            assert(val == "def");
        }
        else if(type == Token::Type::tok_extern) {
            assert(val == "extern");
        }
        else {
            assert(type == Type::tok_identifier);
        }
        this->value.str = val;
    }
    std::string toString() const {
        switch(this->token_type) {
            case Type::tok_eof:
                return "tok_eof";
            case Type::tok_def:
                return "tok_def";
            case Type::tok_extern:
                return "tok_extern";
            case Type::tok_identifier:
                return "tok_identifier:'" + this->value.str + "'";
            case Type::tok_number:
                return "tok_number:'" + std::to_string(this->value.num) +"'" ;
            case Type::tok_char:
                return std::string("tok_char'") + this->value.c + "'";
            default:
                return "unknown";
        }
    }

    char valueAsChar()const {
        assert(this->token_type == Type::tok_char);
        return this->value.c;
    }
};
inline bool operator==(const Token& a, const char& b){
    return a.token_type == Token::Type::tok_char &&
           a.value.c == b;
};
inline bool operator!=(const Token& a, const char& b){
     return !(a == b);
};
inline bool operator==(const Token& a, const Token::Type& b){
    return a.token_type == b;
};
inline bool operator!=(const Token& a, const Token::Type& b){
    return !(a == b);
};
