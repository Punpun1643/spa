#include <string>

#include "SpecialCharToken.h"

SpecialCharToken::SpecialCharToken(std::string val) : Token(val) {}

TokenType SpecialCharToken::GetTokenType() {
  return TokenType::SPECIAL_CHAR_TOKEN;
}
