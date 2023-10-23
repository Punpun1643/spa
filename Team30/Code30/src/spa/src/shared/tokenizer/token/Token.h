#pragma once
#include <string>

#include "TokenType.h"

class Token {
 private:
  std::string value;

 public:
  explicit Token(std::string val = "");
  virtual ~Token() = default;

  std::string GetTokenVal();
  virtual TokenType GetTokenType() = 0;
};
