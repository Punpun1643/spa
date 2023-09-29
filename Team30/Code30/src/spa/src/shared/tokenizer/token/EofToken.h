#pragma once
#include "Token.h"

class EofToken : public Token {
 public:
  explicit EofToken();
  ~EofToken() = default;
  TokenType getTokenType();
};