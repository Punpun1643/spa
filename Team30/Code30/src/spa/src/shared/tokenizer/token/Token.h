#pragma once

#include <string>

enum class TokenType {
  WORD_TOKEN,
  SPECIAL_CHAR_TOKEN,
  INTEGER_TOKEN,
  EOF_TOKEN
};

class Token {
 private:
  std::string value;

 public:
  explicit Token(std::string val);

  ~Token() = default;

  std::string getTokenVal();

  virtual TokenType getTokenType() = 0;
};