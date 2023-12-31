#pragma once

#include <iostream>
#include <memory>
#include <string>
#include <unordered_set>
#include <vector>

#include "token/Token.h"

class Tokenizer {
 private:
  std::istream& input;
  std::unordered_set<std::string> keywords;
  std::shared_ptr<Token> HandleSpecialChar(char c);
  std::shared_ptr<Token> Next();
  bool HasNext();

 public:
  explicit Tokenizer(std::istream& input);
  ~Tokenizer() = default;

  std::vector<std::shared_ptr<Token>> Tokenize();
};
