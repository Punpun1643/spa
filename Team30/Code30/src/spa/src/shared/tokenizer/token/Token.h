#pragma once
#include <string>

enum class TokenType
{
WORD_TOKEN, SPECIAL_CHAR_TOKEN, INTEGER_TOKEN
};

class Token
{
private:
    std::string value;

public:
    explicit Token(std::string val);

    std::string getTokenVal()
    {
        return value;
    }
      
    virtual TokenType getTokenType() = 0;
};