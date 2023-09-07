#pragma once

#include <string>
#include "../../shared/tokenizer/token/Token.h"
#include "../../shared/parser/AParser.h"

class SpParser : public AParser {
public:
    SpParser(std::vector<Token> tokens);

    ~SpParser() = default;

    void parse();
};