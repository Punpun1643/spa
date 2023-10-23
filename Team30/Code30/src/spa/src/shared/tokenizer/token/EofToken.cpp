#include "EofToken.h"

EofToken::EofToken() : Token() {}

TokenType EofToken::GetTokenType() {
  return TokenType::EOF_TOKEN;
}
