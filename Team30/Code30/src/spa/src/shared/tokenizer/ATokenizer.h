#pragma once
#include "token/Token.h"
#include <string>
#include <iostream>
#include <vector>

class ATokenizer
{
private:
    std::istream &input;

public:
    explicit ATokenizer(std::istream &input);
    ~ATokenizer() = default;

    std::shared_ptr<Token> next();
    //void handleWord();
    //void handleWhiteSpace();
    //void handleSpecialChar();
    //void handleInteger();
    //std::vector<std::shared_ptr<Token>> tokenize();
};