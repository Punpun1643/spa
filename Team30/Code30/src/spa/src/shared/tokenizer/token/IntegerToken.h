#pragma once

#include <string>

#include "Token.h"

class IntegerToken : public Token {
 public:
  explicit IntegerToken(std::string val);
  ~IntegerToken() = default;
  TokenType GetTokenType();
};
