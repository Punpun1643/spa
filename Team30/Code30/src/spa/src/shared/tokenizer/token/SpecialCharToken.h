#pragma once

#include "Token.h"

class SpecialCharToken : public Token {
public:
    explicit SpecialCharToken(std::string val);

    ~SpecialCharToken() = default;

    TokenType getTokenType();
};