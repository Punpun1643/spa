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

bool AParser::IsTokenType(std::shared_ptr<Token> token, TokenType token_type) {
  if (token->getTokenType() != token_type) {
    return false;
  }
  return true;
}

bool AParser::IsWordToken(std::shared_ptr<Token> token) {
  return AParser::IsTokenType(token, TokenType::WORD_TOKEN);
}

bool AParser::IsIntegerToken(std::shared_ptr<Token> token) {
  return AParser::IsTokenType(token, TokenType::INTEGER_TOKEN);
}

bool AParser::IsSpecialCharToken(std::shared_ptr<Token> token) {
  return AParser::IsTokenType(token, TokenType::SPECIAL_CHAR_TOKEN);
}

bool AParser::IsEOFToken(std::shared_ptr<Token> token) {
  return AParser::IsTokenType(token, TokenType::EOF_TOKEN);
}
