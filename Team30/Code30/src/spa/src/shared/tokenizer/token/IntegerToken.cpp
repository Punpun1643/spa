#include "IntegerToken.h"

IntegerToken::IntegerToken(std::string val) : Token(val)
{
}

TokenType IntegerToken::getTokenType()
{
    return TokenType::INTEGER_TOKEN;
}