//
// Created by zck on 2021/12/5.
//

#ifndef TUROIAL_MYLANGUAGE_PARSER_H
#define TUROIAL_MYLANGUAGE_PARSER_H

void InitBinopPrecedence();
int getNextToken();

class Parser {
public:
    void parse(const std::string& expr);
};
#endif //TUROIAL_MYLANGUAGE_PARSER_H
