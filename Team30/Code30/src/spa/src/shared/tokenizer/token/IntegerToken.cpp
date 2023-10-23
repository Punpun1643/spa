#include <string>

#include "IntegerToken.h"

IntegerToken::IntegerToken(std::string val) : Token(val) {}

TokenType IntegerToken::GetTokenType() {
  return TokenType::INTEGER_TOKEN;
}
