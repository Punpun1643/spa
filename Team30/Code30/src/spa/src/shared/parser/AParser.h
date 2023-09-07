#pragma once

#include <vector>

#include "../../source_processor/node/ProgramNode.h"
#include "../tokenizer/token/Token.h"

class AParser {
public:
    AParser(std::vector <Token> tokens);

    // move pointer to the next token
    Token nextToken();

    // explore the next token without moving the pointer
    Token peekToken();

    // get the current token
    Token getCurrToken();

    virtual void parse() = 0;

    virtual ~AParser() = default;

protected:
    std::vector <Token> tokens;
    size_t currTokenIndex = 0;
};