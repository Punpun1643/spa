#include "AParser.h"
#include <stdexcept>

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

std::string AParser::getCurrTokenValue() {
  return getCurrToken()->getTokenVal();
}

std::string AParser::getPeekTokenValue() { return peekToken()->getTokenVal(); }

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

bool AParser::IsWordOrIntegerToken(std::shared_ptr<Token> token) {
  return AParser::IsWordToken(token) || AParser::IsIntegerToken(token);
}

bool AParser::IsSpecialCharToken(std::shared_ptr<Token> token) {
  return AParser::IsTokenType(token, TokenType::SPECIAL_CHAR_TOKEN);
}

bool AParser::IsEOFToken(std::shared_ptr<Token> token) {
  return AParser::IsTokenType(token, TokenType::EOF_TOKEN);
}

bool AParser::isCurrTokenType(TokenType tokenType) {
  return IsTokenType(getCurrToken(), tokenType);
}

bool AParser::isCurrTokenValue(std::string const& tokenValue) {
  return tokenValue == getCurrToken()->getTokenVal();
}

bool AParser::isPeekTokenValue(std::string const& tokenValue) {
  return tokenValue == peekToken()->getTokenVal();
}

bool AParser::isCurrTokenTypeAndValue(TokenType tokenType,
                                      std::string const& tokenValue) {
  return IsTokenType(getCurrToken(), tokenType) &&
         tokenValue == getCurrToken()->getTokenVal();
}

void AParser::assertCurrTokenTypeAndValue(TokenType expectedType,
                                          std::string const& expectedValue,
                                          std::string const& errorMessage) {
  if (!isCurrTokenTypeAndValue(expectedType, expectedValue)) {
    throw std::invalid_argument(errorMessage);
  }
}
