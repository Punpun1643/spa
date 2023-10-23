#include <string>

#include "WordToken.h"

WordToken::WordToken(std::string val) : Token(val) {}

TokenType WordToken::GetTokenType() {
  return TokenType::WORD_TOKEN;
}
