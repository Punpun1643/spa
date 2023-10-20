#include "EofToken.h"

EofToken::EofToken() : Token() {}

TokenType EofToken::getTokenType() {
  return TokenType::EOF_TOKEN;
};
