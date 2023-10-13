#include "ParserHelperFunctions.h"

void AddWordVector(std::vector<std::shared_ptr<Token>>& tokens,
                   std::vector<std::string> word_vector) {
  for (std::string word : word_vector) {
    tokens.push_back(
        std::static_pointer_cast<Token>(std::make_shared<WordToken>(word)));
  }
}

void AddSpecialCharVector(std::vector<std::shared_ptr<Token>>& tokens,
                          std::vector<std::string> special_char_vector) {
  for (std::string special_char : special_char_vector) {
    tokens.push_back(std::static_pointer_cast<Token>(
        std::make_shared<SpecialCharToken>(special_char)));
  }
}

void AddInteger(std::vector<std::shared_ptr<Token>>& tokens,
                std::string integer) {
  tokens.push_back(std::make_shared<IntegerToken>(integer));
}

void AddEOF(std::vector<std::shared_ptr<Token>>& tokens) {
  tokens.push_back(
      std::static_pointer_cast<Token>(std::make_shared<EofToken>()));
}
