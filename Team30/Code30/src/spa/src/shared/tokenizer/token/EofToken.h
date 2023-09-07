#pragma once
#include "EofToken.h"
#include "Token.h"

class EofToken : public Token
{
public:
    explicit EofToken(std::string val);
    ~EofToken() = default;
    TokenType getTokenType();
};