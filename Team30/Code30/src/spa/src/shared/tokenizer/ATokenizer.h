#pragma once
#include <iostream>
#include <string>
#include <vector>

#include "token/Token.h"

class ATokenizer {
 private:
  std::istream& input;
  std::shared_ptr<Token> handleSpecialChar(char c);

 public:
  explicit ATokenizer(std::istream& input);
  ~ATokenizer() = default;

  std::vector<std::shared_ptr<Token>> tokenize();
  std::shared_ptr<Token> next();
  bool hasNext();
};