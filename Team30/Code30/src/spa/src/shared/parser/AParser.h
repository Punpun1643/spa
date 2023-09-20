#pragma once

#include <memory>
#include <vector>

#include "../tokenizer/token/Token.h"

class AParser {
 public:
  AParser(std::vector<std::shared_ptr<Token>> tokens);

  // move pointer to the next token
  std::shared_ptr<Token> nextToken();

  // explore the next token without moving the pointer
  std::shared_ptr<Token> peekToken();

  // get the current token
  std::shared_ptr<Token> getCurrToken();

  virtual void parse() = 0;

  virtual ~AParser() = default;

 protected:
  std::vector<std::shared_ptr<Token>> tokens;
  size_t currTokenIndex = 0;
  bool IsTokenType(std::shared_ptr<Token> token, TokenType token_type);
  bool IsWordToken(std::shared_ptr<Token> token);
  bool IsIntegerToken(std::shared_ptr<Token> token);
  bool IsSpecialCharToken(std::shared_ptr<Token> token);
  bool IsEOFToken(std::shared_ptr<Token> token);
};
