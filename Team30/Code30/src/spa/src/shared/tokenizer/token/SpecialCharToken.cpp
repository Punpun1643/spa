#include "SpecialCharToken.h"

SpecialCharToken::SpecialCharToken(std::string val) : Token(val) {}

TokenType SpecialCharToken::getTokenType() {
  return TokenType::SPECIAL_CHAR_TOKEN;
};
