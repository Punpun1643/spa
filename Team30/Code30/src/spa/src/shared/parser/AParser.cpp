#include "AParser.h"

AParser::AParser(std::vector <Token> tokens) : tokens(tokens) {}

Token AParser::nextToken() {
    if (currTokenIndex < tokens.size()) {
        return tokens[currTokenIndex++];
    }
};

Token AParser::peekToken() {
    if (currTokenIndex + 1 < tokens.size()) {
        return tokens[currTokenIndex + 1];
    }
}

Token AParser::getCurrToken() {
    return tokens[currTokenIndex];
}
