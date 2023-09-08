#pragma once
#include <string>
#include "shared/tokenizer/token/TokenType.h"

class Token {
 private:
  std::string value;

 public:
  explicit Token(std::string val = "");
  ~Token() = default;

  std::string getTokenVal();
  virtual TokenType getTokenType() = 0;
};