#include <fstream>
#include <iostream>

#include "../../spa/src/shared/tokenizer/ATokenizer.h"
#include "../../spa/src/shared/tokenizer/token/EofToken.h"
#include "../../spa/src/shared/tokenizer/token/IntegerToken.h"
#include "../../spa/src/shared/tokenizer/token/SpecialCharToken.h"
#include "../../spa/src/shared/tokenizer/token/WordToken.h"
#include "catch.hpp"

TEST_CASE("Tokens") {
  IntegerToken intToken = IntegerToken("33");
  WordToken wordToken = WordToken("Test");
  SpecialCharToken specToken = SpecialCharToken("&");
  EofToken eofToken = EofToken();

  REQUIRE(intToken.getTokenType() == TokenType::INTEGER_TOKEN);
  REQUIRE(intToken.getTokenVal() == "33");

  REQUIRE(wordToken.getTokenType() == TokenType::WORD_TOKEN);
  REQUIRE(specToken.getTokenType() == TokenType::SPECIAL_CHAR_TOKEN);
  REQUIRE(eofToken.getTokenType() == TokenType::EOF_TOKEN);
}

TEST_CASE("Tokenizer") {
  std::ifstream inputFile(
      "/Users/papattaradaapithanangsiri/23s1-cp-spa-team-30/Team30/Tests30/"
      "Sample_source.txt");
  ATokenizer tokenizer = ATokenizer(inputFile);

  std::vector<std::shared_ptr<Token>> tokens = tokenizer.tokenize();

  REQUIRE(tokens.size() == 168);

  REQUIRE(tokens[0]->getTokenVal() == "procedure");
  REQUIRE(tokens[2]->getTokenVal() == "{");
  REQUIRE(tokens[6]->getTokenVal() == ";");
  REQUIRE(tokens[18]->getTokenVal() == "!=");
  REQUIRE(tokens[166]->getTokenVal() == "}");

  REQUIRE(tokens[3]->getTokenType() == TokenType::WORD_TOKEN);
  REQUIRE(tokens[4]->getTokenType() == TokenType::SPECIAL_CHAR_TOKEN);
  REQUIRE(tokens[5]->getTokenType() == TokenType::INTEGER_TOKEN);
  REQUIRE(tokens[tokens.size() - 1]->getTokenType() == TokenType::EOF_TOKEN);
}