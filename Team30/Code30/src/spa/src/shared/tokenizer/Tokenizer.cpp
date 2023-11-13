#include "Tokenizer.h"

#include "token/EofToken.h"
#include "token/IntegerToken.h"
#include "token/SpecialCharToken.h"
#include "token/WordToken.h"

Tokenizer::Tokenizer(std::istream& input) : input(input) {
  keywords = {"Follows", "Parent", "Calls", "Next"};
}

std::vector<std::shared_ptr<Token>> Tokenizer::Tokenize() {
  std::vector<std::shared_ptr<Token>> tokens;
  while (HasNext()) {
    std::shared_ptr<Token> token = Next();
    tokens.push_back(token);
  }
  tokens.push_back(std::make_shared<EofToken>());
  return tokens;
}

std::shared_ptr<Token> Tokenizer::Next() {
  char c;

  while (input.get(c)) {
    if (std::isalpha(c)) {
      std::string word;
      word += c;
      while (std::isalnum(input.peek())) {
        input.get(c);
        word += c;
      }

      if ((keywords.find(word) != keywords.end() && input.peek() == '*') ||
          (word == "stmt" && input.peek() == '#')) {
        input.get(c);
        word += c;
      }
      return std::make_shared<WordToken>(word);
    } else if (std::isdigit(c)) {
      std::string number;
      number += c;
      while (std::isdigit(input.peek())) {
        input.get(c);
        number += c;
      }

      if (std::isalpha(input.peek())) {
        throw std::invalid_argument("Invalid Syntax, invalid integer or name");
      }

      if (number.length() > 1 && number[0] == '0') {
        throw std::invalid_argument("Integer has leading zero!");
      }

      return std::make_shared<IntegerToken>(number);
    } else if (c == '&' || c == '|' || c == '!' || c == '=' || c == '>' ||
               c == '<') {
      return HandleSpecialChar(c);
    } else if (std::isspace(c)) {
      continue;
    } else {
      return std::make_shared<SpecialCharToken>(std::string(1, c));
    }
  }
  throw std::runtime_error("Unexpected end of input in Tokenizer::next");
}

std::shared_ptr<Token> Tokenizer::HandleSpecialChar(char c) {
  char next_c = input.peek();

  if (c == '|' && next_c == '|') {
    input.get(next_c);
    return std::make_shared<SpecialCharToken>("||");
  } else if (c == '&' && next_c == '&') {
    input.get(next_c);
    return std::make_shared<SpecialCharToken>("&&");
  } else if (c == '!' || c == '=' || c == '>' || c == '<') {
    if (next_c == '=') {
      input.get(next_c);
      return std::make_shared<SpecialCharToken>(std::string(1, c) +
                                                std::string(1, next_c));
    }
  }

  return std::make_shared<SpecialCharToken>(std::string(1, c));
}

bool Tokenizer::HasNext() {
  char c;
  while (std::isspace(input.peek())) {
    input.get(c);
  }
  return input.peek() != EOF;
}
