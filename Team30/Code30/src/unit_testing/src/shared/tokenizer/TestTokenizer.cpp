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

  REQUIRE(intToken.GetTokenType() == TokenType::INTEGER_TOKEN);
  REQUIRE(intToken.GetTokenVal() == "33");

  REQUIRE(wordToken.GetTokenType() == TokenType::WORD_TOKEN);
  REQUIRE(specToken.GetTokenType() == TokenType::SPECIAL_CHAR_TOKEN);
  REQUIRE(eofToken.GetTokenType() == TokenType::EOF_TOKEN);
}

TEST_CASE("Tokenizer") {
  // std::ifstream
  // inputFile("<dir>/23s1-cp-spa-team-30/Team30/Tests30/Sample_source.txt");
  // Tokenizer tokenizer = Tokenizer(inputFile);

  // std::vector<std::shared_ptr<Token>> tokens = tokenizer.Tokenize();

  // REQUIRE(tokens.size() == 168);

  // REQUIRE(tokens[0]->GetTokenVal() == "procedure");
  // REQUIRE(tokens[2]->GetTokenVal() == "{");
  // REQUIRE(tokens[6]->GetTokenVal() == ";");
  // REQUIRE(tokens[18]->GetTokenVal() == "!=");
  // REQUIRE(tokens[166]->GetTokenVal() == "}");

  // REQUIRE(tokens[3]->GetTokenType() == TokenType::WORD_TOKEN);
  // REQUIRE(tokens[4]->GetTokenType() == TokenType::SPECIAL_CHAR_TOKEN);
  // REQUIRE(tokens[5]->GetTokenType() == TokenType::INTEGER_TOKEN);
  // REQUIRE(tokens[tokens.size() - 1]->GetTokenType() == TokenType::EOF_TOKEN);

  // valid NAME followed by valid integer with random spacing
  std::string test_str = "  v2ar1     12 ";
  std::istringstream test_stream(test_str);
  Tokenizer tokenizer = Tokenizer(test_stream);
  std::vector<std::shared_ptr<Token>> tokens = tokenizer.Tokenize();

  REQUIRE(tokens.size() == 3);
  REQUIRE(tokens[0]->GetTokenVal() == "v2ar1");
  REQUIRE(tokens[1]->GetTokenVal() == "12");
  REQUIRE(tokens[0]->GetTokenType() == TokenType::WORD_TOKEN);
  REQUIRE(tokens[1]->GetTokenType() == TokenType::INTEGER_TOKEN);
  REQUIRE(tokens[2]->GetTokenType() == TokenType::EOF_TOKEN);

  // invalid integer or name
  test_str = "12procedure";
  std::istringstream test_stream2(test_str);
  Tokenizer tokenizer2 = Tokenizer(test_stream2);
  REQUIRE_THROWS_AS(tokenizer2.Tokenize(), std::invalid_argument);

  // valid name
  test_str = "procedure";
  std::istringstream test_stream3(test_str);
  Tokenizer tokenizer3 = Tokenizer(test_stream3);
  tokens = tokenizer3.Tokenize();
  REQUIRE(tokens.size() == 2);
  REQUIRE(tokens[0]->GetTokenType() == TokenType::WORD_TOKEN);

  // valid integer
  test_str = "6";
  std::istringstream test_stream4(test_str);
  Tokenizer tokenizer4 = Tokenizer(test_stream4);
  tokens = tokenizer4.Tokenize();
  REQUIRE(tokens.size() == 2);
  REQUIRE(tokens[0]->GetTokenType() == TokenType::INTEGER_TOKEN);

  // multiple symbols
  test_str = ";!= ==&&> =<=||=";
  std::istringstream test_stream5(test_str);
  Tokenizer tokenizer5 = Tokenizer(test_stream5);
  tokens = tokenizer5.Tokenize();
  REQUIRE(tokens.size() == 10);
  REQUIRE(tokens[0]->GetTokenVal() == ";");
  REQUIRE(tokens[1]->GetTokenVal() == "!=");
  REQUIRE(tokens[2]->GetTokenVal() == "==");
  REQUIRE(tokens[3]->GetTokenVal() == "&&");
  REQUIRE(tokens[4]->GetTokenVal() == ">");
  REQUIRE(tokens[5]->GetTokenVal() == "=");
  REQUIRE(tokens[6]->GetTokenVal() == "<=");
  REQUIRE(tokens[7]->GetTokenVal() == "||");
  REQUIRE(tokens[8]->GetTokenVal() == "=");

  // Combination of symbols, names and integers
  test_str = "procedure proc21 {call \"sub\"; x= 4;\n}";
  std::istringstream test_stream6(test_str);
  Tokenizer tokenizer6 = Tokenizer(test_stream6);
  tokens = tokenizer6.Tokenize();

  REQUIRE(tokens.size() == 14);
  REQUIRE(tokens[0]->GetTokenVal() == "procedure");
  REQUIRE(tokens[0]->GetTokenType() == TokenType::WORD_TOKEN);
  REQUIRE(tokens[1]->GetTokenVal() == "proc21");
  REQUIRE(tokens[1]->GetTokenType() == TokenType::WORD_TOKEN);
  REQUIRE(tokens[2]->GetTokenVal() == "{");
  REQUIRE(tokens[2]->GetTokenType() == TokenType::SPECIAL_CHAR_TOKEN);
  REQUIRE(tokens[3]->GetTokenVal() == "call");
  REQUIRE(tokens[3]->GetTokenType() == TokenType::WORD_TOKEN);
  REQUIRE(tokens[4]->GetTokenVal() == "\"");
  REQUIRE(tokens[4]->GetTokenType() == TokenType::SPECIAL_CHAR_TOKEN);
  REQUIRE(tokens[5]->GetTokenVal() == "sub");
  REQUIRE(tokens[5]->GetTokenType() == TokenType::WORD_TOKEN);
  REQUIRE(tokens[6]->GetTokenVal() == "\"");
  REQUIRE(tokens[6]->GetTokenType() == TokenType::SPECIAL_CHAR_TOKEN);
  REQUIRE(tokens[7]->GetTokenVal() == ";");
  REQUIRE(tokens[7]->GetTokenType() == TokenType::SPECIAL_CHAR_TOKEN);
  REQUIRE(tokens[8]->GetTokenVal() == "x");
  REQUIRE(tokens[8]->GetTokenType() == TokenType::WORD_TOKEN);
  REQUIRE(tokens[9]->GetTokenVal() == "=");
  REQUIRE(tokens[9]->GetTokenType() == TokenType::SPECIAL_CHAR_TOKEN);
  REQUIRE(tokens[10]->GetTokenVal() == "4");
  REQUIRE(tokens[10]->GetTokenType() == TokenType::INTEGER_TOKEN);
  REQUIRE(tokens[11]->GetTokenVal() == ";");
  REQUIRE(tokens[11]->GetTokenType() == TokenType::SPECIAL_CHAR_TOKEN);
  REQUIRE(tokens[12]->GetTokenVal() == "}");
  REQUIRE(tokens[12]->GetTokenType() == TokenType::SPECIAL_CHAR_TOKEN);
  REQUIRE(tokens[13]->GetTokenType() == TokenType::EOF_TOKEN);

  test_str = "Select s such that Follows*(1, 2) and Parent *(1, 3)";
  std::istringstream test_stream7(test_str);
  Tokenizer tokenizer7 = Tokenizer(test_stream7);
  tokens = tokenizer7.Tokenize();
  REQUIRE(tokens[0]->GetTokenVal() == "Select");
  REQUIRE(tokens[0]->GetTokenType() == TokenType::WORD_TOKEN);
  REQUIRE(tokens[1]->GetTokenVal() == "s");
  REQUIRE(tokens[1]->GetTokenType() == TokenType::WORD_TOKEN);
  REQUIRE(tokens[4]->GetTokenVal() == "Follows*");
  REQUIRE(tokens[4]->GetTokenType() == TokenType::WORD_TOKEN);
  REQUIRE(tokens[5]->GetTokenVal() == "(");
  REQUIRE(tokens[5]->GetTokenType() == TokenType::SPECIAL_CHAR_TOKEN);
  REQUIRE(tokens[9]->GetTokenVal() == ")");
  REQUIRE(tokens[9]->GetTokenType() == TokenType::SPECIAL_CHAR_TOKEN);
  REQUIRE(tokens[11]->GetTokenVal() == "Parent");
  REQUIRE(tokens[11]->GetTokenType() == TokenType::WORD_TOKEN);
  REQUIRE(tokens[12]->GetTokenVal() == "*");
  REQUIRE(tokens[12]->GetTokenType() == TokenType::SPECIAL_CHAR_TOKEN);
  REQUIRE(tokens[13]->GetTokenVal() == "(");
  REQUIRE(tokens[13]->GetTokenType() == TokenType::SPECIAL_CHAR_TOKEN);

  // stmt.stmt#
  test_str = "stmt.stmt#";
  std::istringstream test_stream8(test_str);
  Tokenizer tokenizer8 = Tokenizer(test_stream8);
  tokens = tokenizer8.Tokenize();
  REQUIRE(tokens.size() == 4);
  REQUIRE(tokens[0]->GetTokenVal() == "stmt");
  REQUIRE(tokens[1]->GetTokenVal() == ".");
  REQUIRE(tokens[2]->GetTokenVal() == "stmt#");
  REQUIRE(tokens[2]->GetTokenType() == TokenType::WORD_TOKEN);
  REQUIRE(tokens[3]->GetTokenType() == TokenType::EOF_TOKEN);

  // stmt.stmt #
  test_str = "stmt.stmt #";
  std::istringstream test_stream9(test_str);
  Tokenizer tokenizer9 = Tokenizer(test_stream9);
  tokens = tokenizer9.Tokenize();
  REQUIRE(tokens.size() == 5);
  REQUIRE(tokens[0]->GetTokenVal() == "stmt");
  REQUIRE(tokens[1]->GetTokenVal() == ".");
  REQUIRE(tokens[2]->GetTokenVal() == "stmt");
  REQUIRE(tokens[2]->GetTokenType() == TokenType::WORD_TOKEN);
  REQUIRE(tokens[3]->GetTokenVal() == "#");
  REQUIRE(tokens[3]->GetTokenType() == TokenType::SPECIAL_CHAR_TOKEN);
  REQUIRE(tokens[4]->GetTokenType() == TokenType::EOF_TOKEN);
}
