#pragma once#pragma once
#include "token/Token.h"
#include <string>
#include <iostream>
#include <vector>

class ATokenizer
{
private:
    std::istream &input;
    std::shared_ptr<Token> handleSpecialChar(char c);

public:
    explicit ATokenizer(std::istream &input);
    ~ATokenizer() = default;

    std::vector<std::shared_ptr<Token>> tokenize();
    std::shared_ptr<Token> next();
    bool hasNext();
};