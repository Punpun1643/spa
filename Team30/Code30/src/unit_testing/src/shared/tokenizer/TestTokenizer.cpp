#include <filesystem>
#include <fstream>
#include <iostream>
#include <sstream>

#include "../../../../spa/src/shared/tokenizer/Tokenizer.h"
#include "../../../../spa/src/shared/tokenizer/token/EofToken.h"
#include "../../../../spa/src/shared/tokenizer/token/IntegerToken.h"
#include "../../../../spa/src/shared/tokenizer/token/SpecialCharToken.h"
#include "../../../../spa/src/shared/tokenizer/token/WordToken.h"
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
  // std::ifstream
  // inputFile("<dir>/23s1-cp-spa-team-30/Team30/Tests30/Sample_source.txt");
  // Tokenizer tokenizer = Tokenizer(inputFile);

  // std::vector<std::shared_ptr<Token>> tokens = tokenizer.tokenize();

  // REQUIRE(tokens.size() == 168);

  // REQUIRE(tokens[0]->getTokenVal() == "procedure");
  // REQUIRE(tokens[2]->getTokenVal() == "{");
  // REQUIRE(tokens[6]->getTokenVal() == ";");
  // REQUIRE(tokens[18]->getTokenVal() == "!=");
  // REQUIRE(tokens[166]->getTokenVal() == "}");

  // REQUIRE(tokens[3]->getTokenType() == TokenType::WORD_TOKEN);
  // REQUIRE(tokens[4]->getTokenType() == TokenType::SPECIAL_CHAR_TOKEN);
  // REQUIRE(tokens[5]->getTokenType() == TokenType::INTEGER_TOKEN);
  // REQUIRE(tokens[tokens.size() - 1]->getTokenType() == TokenType::EOF_TOKEN);

  // valid NAME followed by valid integer with random spacing
  std::string test_str = "  v2ar1     12 ";
  std::istringstream test_stream(test_str);
  Tokenizer tokenizer = Tokenizer(test_stream);
  std::vector<std::shared_ptr<Token>> tokens = tokenizer.tokenize();

  REQUIRE(tokens.size() == 3);
  REQUIRE(tokens[0]->getTokenVal() == "v2ar1");
  REQUIRE(tokens[1]->getTokenVal() == "12");
  REQUIRE(tokens[0]->getTokenType() == TokenType::WORD_TOKEN);
  REQUIRE(tokens[1]->getTokenType() == TokenType::INTEGER_TOKEN);
  REQUIRE(tokens[2]->getTokenType() == TokenType::EOF_TOKEN);

  // invalid integer or name
  test_str = "12procedure";
  std::istringstream test_stream2(test_str);
  Tokenizer tokenizer2 = Tokenizer(test_stream2);
  REQUIRE_THROWS_AS(tokenizer2.tokenize(), std::invalid_argument);

  // valid name
  test_str = "procedure";
  std::istringstream test_stream3(test_str);
  Tokenizer tokenizer3 = Tokenizer(test_stream3);
  tokens = tokenizer3.tokenize();
  REQUIRE(tokens.size() == 2);
  REQUIRE(tokens[0]->getTokenType() == TokenType::WORD_TOKEN);

  // valid integer
  test_str = "6";
  std::istringstream test_stream4(test_str);
  Tokenizer tokenizer4 = Tokenizer(test_stream4);
  tokens = tokenizer4.tokenize();
  REQUIRE(tokens.size() == 2);
  REQUIRE(tokens[0]->getTokenType() == TokenType::INTEGER_TOKEN);

  // multiple symbols
  test_str = ";!= ==&&> =<=||=";
  std::istringstream test_stream5(test_str);
  Tokenizer tokenizer5 = Tokenizer(test_stream5);
  tokens = tokenizer5.tokenize();
  REQUIRE(tokens.size() == 10);
  REQUIRE(tokens[0]->getTokenVal() == ";");
  REQUIRE(tokens[1]->getTokenVal() == "!=");
  REQUIRE(tokens[2]->getTokenVal() == "==");
  REQUIRE(tokens[3]->getTokenVal() == "&&");
  REQUIRE(tokens[4]->getTokenVal() == ">");
  REQUIRE(tokens[5]->getTokenVal() == "=");
  REQUIRE(tokens[6]->getTokenVal() == "<=");
  REQUIRE(tokens[7]->getTokenVal() == "||");
  REQUIRE(tokens[8]->getTokenVal() == "=");

  // Combination of symbols, names and integers
  test_str = "procedure proc21 {call \"sub\"; x= 4;\n}";
  std::istringstream test_stream6(test_str);
  Tokenizer tokenizer6 = Tokenizer(test_stream6);
  tokens = tokenizer6.tokenize();

  REQUIRE(tokens.size() == 14);
  REQUIRE(tokens[0]->getTokenVal() == "procedure");
  REQUIRE(tokens[0]->getTokenType() == TokenType::WORD_TOKEN);
  REQUIRE(tokens[1]->getTokenVal() == "proc21");
  REQUIRE(tokens[1]->getTokenType() == TokenType::WORD_TOKEN);
  REQUIRE(tokens[2]->getTokenVal() == "{");
  REQUIRE(tokens[2]->getTokenType() == TokenType::SPECIAL_CHAR_TOKEN);
  REQUIRE(tokens[3]->getTokenVal() == "call");
  REQUIRE(tokens[3]->getTokenType() == TokenType::WORD_TOKEN);
  REQUIRE(tokens[4]->getTokenVal() == "\"");
  REQUIRE(tokens[4]->getTokenType() == TokenType::SPECIAL_CHAR_TOKEN);
  REQUIRE(tokens[5]->getTokenVal() == "sub");
  REQUIRE(tokens[5]->getTokenType() == TokenType::WORD_TOKEN);
  REQUIRE(tokens[6]->getTokenVal() == "\"");
  REQUIRE(tokens[6]->getTokenType() == TokenType::SPECIAL_CHAR_TOKEN);
  REQUIRE(tokens[7]->getTokenVal() == ";");
  REQUIRE(tokens[7]->getTokenType() == TokenType::SPECIAL_CHAR_TOKEN);
  REQUIRE(tokens[8]->getTokenVal() == "x");
  REQUIRE(tokens[8]->getTokenType() == TokenType::WORD_TOKEN);
  REQUIRE(tokens[9]->getTokenVal() == "=");
  REQUIRE(tokens[9]->getTokenType() == TokenType::SPECIAL_CHAR_TOKEN);
  REQUIRE(tokens[10]->getTokenVal() == "4");
  REQUIRE(tokens[10]->getTokenType() == TokenType::INTEGER_TOKEN);
  REQUIRE(tokens[11]->getTokenVal() == ";");
  REQUIRE(tokens[11]->getTokenType() == TokenType::SPECIAL_CHAR_TOKEN);
  REQUIRE(tokens[12]->getTokenVal() == "}");
  REQUIRE(tokens[12]->getTokenType() == TokenType::SPECIAL_CHAR_TOKEN);
  REQUIRE(tokens[13]->getTokenType() == TokenType::EOF_TOKEN);


  test_str = "Select s such that Follows*(1, 2) and Parent *(1, 3)";
  std::istringstream test_stream7(test_str);
  Tokenizer tokenizer7 = Tokenizer(test_stream7);
  tokens = tokenizer7.tokenize();
  REQUIRE(tokens[0]->getTokenVal() == "Select");
  REQUIRE(tokens[0]->getTokenType() == TokenType::WORD_TOKEN);
  REQUIRE(tokens[1]->getTokenVal() == "s");
  REQUIRE(tokens[1]->getTokenType() == TokenType::WORD_TOKEN);
  REQUIRE(tokens[4]->getTokenVal() == "Follows*");
  REQUIRE(tokens[4]->getTokenType() == TokenType::WORD_TOKEN);
  REQUIRE(tokens[5]->getTokenVal() == "(");
  REQUIRE(tokens[5]->getTokenType() == TokenType::SPECIAL_CHAR_TOKEN);
  REQUIRE(tokens[9]->getTokenVal() == ")");
  REQUIRE(tokens[9]->getTokenType() == TokenType::SPECIAL_CHAR_TOKEN);
  REQUIRE(tokens[11]->getTokenVal() == "Parent");
  REQUIRE(tokens[11]->getTokenType() == TokenType::WORD_TOKEN);
  REQUIRE(tokens[12]->getTokenVal() == "*");
  REQUIRE(tokens[12]->getTokenType() == TokenType::SPECIAL_CHAR_TOKEN);
  REQUIRE(tokens[13]->getTokenVal() == "(");
  REQUIRE(tokens[13]->getTokenType() == TokenType::SPECIAL_CHAR_TOKEN);
}
