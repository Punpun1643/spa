#pragma once
#include "Token.h"

class EofToken : public Token {
 public:
  EofToken();
  ~EofToken() = default;
  TokenType GetTokenType();
};
