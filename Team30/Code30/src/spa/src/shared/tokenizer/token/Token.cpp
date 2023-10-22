#include "Token.h"

Token::Token(std::string val) : value(val) {}

std::string Token::GetTokenVal() {
  return value;
}
