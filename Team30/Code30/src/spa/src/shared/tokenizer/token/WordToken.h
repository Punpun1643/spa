#pragma once
#include "Token.h"

class WordToken : public Token
{
public:
    explicit WordToken(std::string val);
    TokenType getTokenType();
};