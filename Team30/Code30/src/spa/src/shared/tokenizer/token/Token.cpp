#include "Token.h"

Token::Token(std::string val) : value(val)
{
}

std::string Token::getTokenVal()
{
    return value;
}