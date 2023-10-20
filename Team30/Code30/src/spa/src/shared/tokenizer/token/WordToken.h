#pragma once

#include <string>

#include "Token.h"

class WordToken : public Token {
 public:
  explicit WordToken(std::string val);
  ~WordToken() = default;
  TokenType getTokenType();
};
