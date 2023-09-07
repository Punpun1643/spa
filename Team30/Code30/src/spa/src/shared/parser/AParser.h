#pragma once

#include <vector>

#include "../../source_processor/node/ProgramNode.h"
#include "../tokenizer/token/Token.h"

class AParser {
public:
    AParser(std::vector <std::shared_ptr<Token>> tokens);

    // move pointer to the next token
    std::shared_ptr<Token> nextToken();

    // explore the next token without moving the pointer
    std::shared_ptr<Token> peekToken();

    // get the current token
    std::shared_ptr<Token> getCurrToken();

    virtual void parse() = 0;

    virtual ~AParser() = default;

protected:
    std::vector <std::shared_ptr<Token>> tokens;
    size_t currTokenIndex = 0;
};