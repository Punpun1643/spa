#pragma once

#include <iostream>
#include <memory>
#include <string>
#include <vector>

#include "token/Token.h"

class Tokenizer {
 private:
  std::istream& input;
  std::shared_ptr<Token> handleSpecialChar(char c);
  std::shared_ptr<Token> next();
  bool hasNext();

 public:
  explicit Tokenizer(std::istream& input);
  ~Tokenizer() = default;

  std::vector<std::shared_ptr<Token>> tokenize();
};
