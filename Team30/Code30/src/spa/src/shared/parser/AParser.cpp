#include "AParser.h"

AParser::AParser(std::vector<std::shared_ptr<Token>> tokens)
    : tokens(std::move(tokens)) {}

std::shared_ptr<Token> AParser::nextToken() {
  if (currTokenIndex < tokens.size()) {
    return tokens[++currTokenIndex];
  }
}

std::shared_ptr<Token> AParser::peekToken() {
  if (currTokenIndex + 1 < tokens.size()) {
    return tokens[currTokenIndex + 1];
  }
}

std::shared_ptr<Token> AParser::getCurrToken() {
  return tokens[currTokenIndex];
}
