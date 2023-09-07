#include "AParser.h"

AParser::AParser(std::vector <Token> tokens) : tokens(tokens) {}

Token AParser::nextToken() {
    if (currTokenIndex < tokens.size()) {
        return tokens[currTokenIndex++];
    }
};
