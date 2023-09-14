#include "../../spa/src/shared/tokenizer/token/EofToken.h"
#include "../../spa/src/shared/tokenizer/token/IntegerToken.h"
#include "../../spa/src/shared/tokenizer/token/SpecialCharToken.h"
#include "../../spa/src/shared/tokenizer/token/Token.h"
#include "../../spa/src/shared/tokenizer/token/WordToken.h"
#include "../../spa/src/source_processor/node/stmt_node/StmtType.h"
#include "../../spa/src/source_processor/parser/SpParser.h"
#include "catch.hpp"

using Catch::Matchers::Contains;

TEST_CASE("Test parseWhile", "[parseWhile]") {
  SECTION(
      "Test invalid while keyword throws error and invalid while as the error "
      "message") {
    std::vector<std::shared_ptr<Token>> tokens;

    tokens.push_back(std::static_pointer_cast<Token>(
        std::make_shared<WordToken>("not while")));
    tokens.push_back(std::static_pointer_cast<Token>(
        std::make_shared<SpecialCharToken>("(")));
    tokens.push_back(
        std::static_pointer_cast<Token>(std::make_shared<WordToken>("i")));
    tokens.push_back(std::static_pointer_cast<Token>(
        std::make_shared<SpecialCharToken>("!=")));
    tokens.push_back(
        std::static_pointer_cast<Token>(std::make_shared<IntegerToken>("0")));
    tokens.push_back(std::static_pointer_cast<Token>(
        std::make_shared<SpecialCharToken>(")")));
    tokens.push_back(std::static_pointer_cast<Token>(
        std::make_shared<SpecialCharToken>("{")));
    tokens.push_back(
        std::static_pointer_cast<Token>(std::make_shared<WordToken>("read")));
    tokens.push_back(
        std::static_pointer_cast<Token>(std::make_shared<WordToken>("i")));
    tokens.push_back(std::static_pointer_cast<Token>(
        std::make_shared<SpecialCharToken>(";")));
    tokens.push_back(
        std::static_pointer_cast<Token>(std::make_shared<WordToken>("print")));
    tokens.push_back(std::static_pointer_cast<Token>(
        std::make_shared<WordToken>("variable")));
    tokens.push_back(std::static_pointer_cast<Token>(
        std::make_shared<SpecialCharToken>(";")));
    tokens.push_back(std::static_pointer_cast<Token>(
        std::make_shared<SpecialCharToken>("}")));
    tokens.push_back(
        std::static_pointer_cast<Token>(std::make_shared<EofToken>()));

    SpParser parser = SpParser(tokens);

    REQUIRE_THROWS_WITH(parser.parseWhile(), Contains("Invalid while"));
  }

  SECTION(
      "Test missing open bracket after while keyword throws invalid while as "
      "the error message") {
    std::vector<std::shared_ptr<Token>> tokens;
    tokens.push_back(
        std::static_pointer_cast<Token>(std::make_shared<WordToken>("i")));
    tokens.push_back(std::static_pointer_cast<Token>(
        std::make_shared<SpecialCharToken>("!=")));
    tokens.push_back(
        std::static_pointer_cast<Token>(std::make_shared<IntegerToken>("0")));
    tokens.push_back(std::static_pointer_cast<Token>(
        std::make_shared<SpecialCharToken>(")")));
    tokens.push_back(std::static_pointer_cast<Token>(
        std::make_shared<SpecialCharToken>("{")));
    tokens.push_back(
        std::static_pointer_cast<Token>(std::make_shared<WordToken>("read")));
    tokens.push_back(
        std::static_pointer_cast<Token>(std::make_shared<WordToken>("i")));
    tokens.push_back(std::static_pointer_cast<Token>(
        std::make_shared<SpecialCharToken>(";")));
    tokens.push_back(
        std::static_pointer_cast<Token>(std::make_shared<WordToken>("print")));
    tokens.push_back(std::static_pointer_cast<Token>(
        std::make_shared<WordToken>("variable")));
    tokens.push_back(std::static_pointer_cast<Token>(
        std::make_shared<SpecialCharToken>(";")));
    tokens.push_back(std::static_pointer_cast<Token>(
        std::make_shared<SpecialCharToken>("}")));
    tokens.push_back(
        std::static_pointer_cast<Token>(std::make_shared<EofToken>()));

    SpParser parser = SpParser(tokens);

    REQUIRE_THROWS_WITH(parser.parseWhile(), Contains("Invalid while"));
  }

  SECTION(
      "Test other special character at the open bracket position after while "
      "keyword throws invalid while as the error message") {
    std::vector<std::shared_ptr<Token>> tokens;
    tokens.push_back(std::static_pointer_cast<Token>(
        std::make_shared<SpecialCharToken>(")")));
    tokens.push_back(
        std::static_pointer_cast<Token>(std::make_shared<WordToken>("i")));
    tokens.push_back(std::static_pointer_cast<Token>(
        std::make_shared<SpecialCharToken>("!=")));
    tokens.push_back(
        std::static_pointer_cast<Token>(std::make_shared<IntegerToken>("0")));
    tokens.push_back(std::static_pointer_cast<Token>(
        std::make_shared<SpecialCharToken>(")")));
    tokens.push_back(std::static_pointer_cast<Token>(
        std::make_shared<SpecialCharToken>("{")));
    tokens.push_back(
        std::static_pointer_cast<Token>(std::make_shared<WordToken>("read")));
    tokens.push_back(
        std::static_pointer_cast<Token>(std::make_shared<WordToken>("i")));
    tokens.push_back(std::static_pointer_cast<Token>(
        std::make_shared<SpecialCharToken>(";")));
    tokens.push_back(
        std::static_pointer_cast<Token>(std::make_shared<WordToken>("print")));
    tokens.push_back(std::static_pointer_cast<Token>(
        std::make_shared<WordToken>("variable")));
    tokens.push_back(std::static_pointer_cast<Token>(
        std::make_shared<SpecialCharToken>(";")));
    tokens.push_back(std::static_pointer_cast<Token>(
        std::make_shared<SpecialCharToken>("}")));
    tokens.push_back(
        std::static_pointer_cast<Token>(std::make_shared<EofToken>()));

    SpParser parser = SpParser(tokens);

    REQUIRE_THROWS_WITH(parser.parseWhile(), Contains("Invalid while"));
  }

  SECTION(
      "Test missing close bracket throws invalid while as the error message") {
    std::vector<std::shared_ptr<Token>> tokens;
    tokens.push_back(std::static_pointer_cast<Token>(
        std::make_shared<SpecialCharToken>("(")));
    tokens.push_back(
        std::static_pointer_cast<Token>(std::make_shared<WordToken>("i")));
    tokens.push_back(std::static_pointer_cast<Token>(
        std::make_shared<SpecialCharToken>("!=")));
    tokens.push_back(
        std::static_pointer_cast<Token>(std::make_shared<IntegerToken>("0")));
    tokens.push_back(std::static_pointer_cast<Token>(
        std::make_shared<SpecialCharToken>("{")));
    tokens.push_back(
        std::static_pointer_cast<Token>(std::make_shared<WordToken>("read")));
    tokens.push_back(
        std::static_pointer_cast<Token>(std::make_shared<WordToken>("i")));
    tokens.push_back(std::static_pointer_cast<Token>(
        std::make_shared<SpecialCharToken>(";")));
    tokens.push_back(
        std::static_pointer_cast<Token>(std::make_shared<WordToken>("print")));
    tokens.push_back(std::static_pointer_cast<Token>(
        std::make_shared<WordToken>("variable")));
    tokens.push_back(std::static_pointer_cast<Token>(
        std::make_shared<SpecialCharToken>(";")));
    tokens.push_back(std::static_pointer_cast<Token>(
        std::make_shared<SpecialCharToken>("}")));
    tokens.push_back(
        std::static_pointer_cast<Token>(std::make_shared<EofToken>()));

    SpParser parser = SpParser(tokens);

    REQUIRE_THROWS_WITH(parser.parseWhile(), Contains("Invalid while"));
  }

  SECTION(
      "Test double close bracket throws invalid while as the error message") {
    std::vector<std::shared_ptr<Token>> tokens;
    tokens.push_back(std::static_pointer_cast<Token>(
        std::make_shared<SpecialCharToken>("(")));
    tokens.push_back(
        std::static_pointer_cast<Token>(std::make_shared<WordToken>("i")));
    tokens.push_back(std::static_pointer_cast<Token>(
        std::make_shared<SpecialCharToken>("!=")));
    tokens.push_back(
        std::static_pointer_cast<Token>(std::make_shared<IntegerToken>("0")));
    tokens.push_back(std::static_pointer_cast<Token>(
        std::make_shared<SpecialCharToken>(")")));
    tokens.push_back(std::static_pointer_cast<Token>(
        std::make_shared<SpecialCharToken>(")")));
    tokens.push_back(std::static_pointer_cast<Token>(
        std::make_shared<SpecialCharToken>("{")));
    tokens.push_back(
        std::static_pointer_cast<Token>(std::make_shared<WordToken>("read")));
    tokens.push_back(
        std::static_pointer_cast<Token>(std::make_shared<WordToken>("i")));
    tokens.push_back(std::static_pointer_cast<Token>(
        std::make_shared<SpecialCharToken>(";")));
    tokens.push_back(
        std::static_pointer_cast<Token>(std::make_shared<WordToken>("print")));
    tokens.push_back(std::static_pointer_cast<Token>(
        std::make_shared<WordToken>("variable")));
    tokens.push_back(std::static_pointer_cast<Token>(
        std::make_shared<SpecialCharToken>(";")));
    tokens.push_back(std::static_pointer_cast<Token>(
        std::make_shared<SpecialCharToken>("}")));
    tokens.push_back(
        std::static_pointer_cast<Token>(std::make_shared<EofToken>()));

    SpParser parser = SpParser(tokens);

    REQUIRE_THROWS_WITH(parser.parseWhile(), Contains("Invalid while"));
  }

  SECTION(
      "Test wrong character at open brace throws invalid while as the error "
      "message") {
    std::vector<std::shared_ptr<Token>> tokens;
    tokens.push_back(std::static_pointer_cast<Token>(
        std::make_shared<SpecialCharToken>("(")));
    tokens.push_back(
        std::static_pointer_cast<Token>(std::make_shared<WordToken>("i")));
    tokens.push_back(std::static_pointer_cast<Token>(
        std::make_shared<SpecialCharToken>("!=")));
    tokens.push_back(
        std::static_pointer_cast<Token>(std::make_shared<IntegerToken>("0")));
    tokens.push_back(std::static_pointer_cast<Token>(
        std::make_shared<SpecialCharToken>(")")));
    tokens.push_back(std::static_pointer_cast<Token>(
        std::make_shared<SpecialCharToken>("wrong char")));
    tokens.push_back(
        std::static_pointer_cast<Token>(std::make_shared<WordToken>("read")));
    tokens.push_back(
        std::static_pointer_cast<Token>(std::make_shared<WordToken>("i")));
    tokens.push_back(std::static_pointer_cast<Token>(
        std::make_shared<SpecialCharToken>(";")));
    tokens.push_back(
        std::static_pointer_cast<Token>(std::make_shared<WordToken>("print")));
    tokens.push_back(std::static_pointer_cast<Token>(
        std::make_shared<WordToken>("variable")));
    tokens.push_back(std::static_pointer_cast<Token>(
        std::make_shared<SpecialCharToken>(";")));
    tokens.push_back(std::static_pointer_cast<Token>(
        std::make_shared<SpecialCharToken>("}")));
    tokens.push_back(
        std::static_pointer_cast<Token>(std::make_shared<EofToken>()));

    SpParser parser = SpParser(tokens);

    REQUIRE_THROWS_WITH(parser.parseWhile(), Contains("Invalid while"));
  }

  SECTION("Test missing open brace throws invalid while as the error message") {
    std::vector<std::shared_ptr<Token>> tokens;
    tokens.push_back(std::static_pointer_cast<Token>(
        std::make_shared<SpecialCharToken>("(")));
    tokens.push_back(
        std::static_pointer_cast<Token>(std::make_shared<WordToken>("i")));
    tokens.push_back(std::static_pointer_cast<Token>(
        std::make_shared<SpecialCharToken>("!=")));
    tokens.push_back(
        std::static_pointer_cast<Token>(std::make_shared<IntegerToken>("0")));
    tokens.push_back(std::static_pointer_cast<Token>(
        std::make_shared<SpecialCharToken>(")")));
    tokens.push_back(
        std::static_pointer_cast<Token>(std::make_shared<WordToken>("read")));
    tokens.push_back(
        std::static_pointer_cast<Token>(std::make_shared<WordToken>("i")));
    tokens.push_back(std::static_pointer_cast<Token>(
        std::make_shared<SpecialCharToken>(";")));
    tokens.push_back(
        std::static_pointer_cast<Token>(std::make_shared<WordToken>("print")));
    tokens.push_back(std::static_pointer_cast<Token>(
        std::make_shared<WordToken>("variable")));
    tokens.push_back(std::static_pointer_cast<Token>(
        std::make_shared<SpecialCharToken>(";")));
    tokens.push_back(std::static_pointer_cast<Token>(
        std::make_shared<SpecialCharToken>("}")));
    tokens.push_back(
        std::static_pointer_cast<Token>(std::make_shared<EofToken>()));

    SpParser parser = SpParser(tokens);

    REQUIRE_THROWS_WITH(parser.parseWhile(), Contains("Invalid while"));
  }

  SECTION("Test missing braces throws invalid while as the error message") {
    std::vector<std::shared_ptr<Token>> tokens;
    tokens.push_back(std::static_pointer_cast<Token>(
        std::make_shared<SpecialCharToken>("(")));
    tokens.push_back(
        std::static_pointer_cast<Token>(std::make_shared<WordToken>("i")));
    tokens.push_back(std::static_pointer_cast<Token>(
        std::make_shared<SpecialCharToken>("!=")));
    tokens.push_back(
        std::static_pointer_cast<Token>(std::make_shared<IntegerToken>("0")));
    tokens.push_back(std::static_pointer_cast<Token>(
        std::make_shared<SpecialCharToken>(")")));
    tokens.push_back(std::static_pointer_cast<Token>(
        std::make_shared<SpecialCharToken>("{")));
    tokens.push_back(
        std::static_pointer_cast<Token>(std::make_shared<WordToken>("read")));
    tokens.push_back(
        std::static_pointer_cast<Token>(std::make_shared<WordToken>("i")));
    tokens.push_back(std::static_pointer_cast<Token>(
        std::make_shared<SpecialCharToken>(";")));
    tokens.push_back(
        std::static_pointer_cast<Token>(std::make_shared<WordToken>("print")));
    tokens.push_back(std::static_pointer_cast<Token>(
        std::make_shared<WordToken>("variable")));
    tokens.push_back(std::static_pointer_cast<Token>(
        std::make_shared<SpecialCharToken>(";")));
    tokens.push_back(
        std::static_pointer_cast<Token>(std::make_shared<WordToken>("")));
    tokens.push_back(
        std::static_pointer_cast<Token>(std::make_shared<EofToken>()));

    SpParser parser = SpParser(tokens);

    REQUIRE_THROWS_AS(parser.parseWhile(), std::invalid_argument);
  }

  SECTION("Test correct while") {
    std::vector<std::shared_ptr<Token>> tokens;
    tokens.push_back(std::static_pointer_cast<Token>(
        std::make_shared<SpecialCharToken>("(")));
    tokens.push_back(
        std::static_pointer_cast<Token>(std::make_shared<WordToken>("i")));
    tokens.push_back(std::static_pointer_cast<Token>(
        std::make_shared<SpecialCharToken>("!=")));
    tokens.push_back(
        std::static_pointer_cast<Token>(std::make_shared<IntegerToken>("0")));
    tokens.push_back(std::static_pointer_cast<Token>(
        std::make_shared<SpecialCharToken>(")")));
    tokens.push_back(std::static_pointer_cast<Token>(
        std::make_shared<SpecialCharToken>("{")));
    tokens.push_back(
        std::static_pointer_cast<Token>(std::make_shared<WordToken>("read")));
    tokens.push_back(
        std::static_pointer_cast<Token>(std::make_shared<WordToken>("i")));
    tokens.push_back(std::static_pointer_cast<Token>(
        std::make_shared<SpecialCharToken>(";")));
    tokens.push_back(
        std::static_pointer_cast<Token>(std::make_shared<WordToken>("print")));
    tokens.push_back(std::static_pointer_cast<Token>(
        std::make_shared<WordToken>("variable")));
    tokens.push_back(std::static_pointer_cast<Token>(
        std::make_shared<SpecialCharToken>(";")));
    tokens.push_back(std::static_pointer_cast<Token>(
        std::make_shared<SpecialCharToken>("}")));
    tokens.push_back(
        std::static_pointer_cast<Token>(std::make_shared<EofToken>()));

    SpParser parser = SpParser(tokens);
    std::shared_ptr<WhileNode> whileNode = parser.parseWhile();

    REQUIRE(whileNode->getStmtLst()->getChildren().size() == 2);
  }
}