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

// TEST_CASE("Tokenizer")
//{
//     std::ifstream inputFile("Sample_source.txt");
//     ATokenizer tokenizer = ATokenizer(inputFile);
//     REQUIRE(tokenizer.next()->getTokenVal() == "procedure");
//     REQUIRE(tokenizer.next()->getTokenVal() == "Example");
//     REQUIRE(tokenizer.next()->getTokenVal() == "{");
//     REQUIRE(tokenizer.next()->getTokenType() == TokenType::WORD_TOKEN);
//     REQUIRE(tokenizer.next()->getTokenType() ==
//     TokenType::SPECIAL_CHAR_TOKEN); REQUIRE(tokenizer.next()->getTokenType()
//     == TokenType::INTEGER_TOKEN);
//
//     std::vector<std::shared_ptr<Token>> tokens = tokenizer.tokenize();
//     REQUIRE(tokens[0]->getTokenVal() == ";");
//     REQUIRE(tokens[tokens.size() - 1]->getTokenType() ==
//     TokenType::EOF_TOKEN); REQUIRE(tokens.size() == 162);
// }