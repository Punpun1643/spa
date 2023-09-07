#pragma once
#include "Token.h"

class SpecialCharToken : public Token
{
public:
    explicit SpecialCharToken(std::string val);
    TokenType getTokenType();
};