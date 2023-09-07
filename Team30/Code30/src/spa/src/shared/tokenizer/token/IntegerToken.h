#pragma once
#include "Token.h"

class IntegerToken : public Token
{
public:
    explicit IntegerToken(std::string val);
    TokenType getTokenType();
};