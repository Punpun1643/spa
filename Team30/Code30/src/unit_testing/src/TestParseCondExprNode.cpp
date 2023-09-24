#include <memory>

#include "../../spa/src/shared/tokenizer/token/EofToken.h"
#include "../../spa/src/shared/tokenizer/token/IntegerToken.h"
#include "../../spa/src/shared/tokenizer/token/SpecialCharToken.h"
#include "../../spa/src/shared/tokenizer/token/Token.h"
#include "../../spa/src/shared/tokenizer/token/WordToken.h"
#include "../../spa/src/source_processor/node/util_node/CondExprNode.h"
#include "../../spa/src/source_processor/parser/SpParser.h"
#include "catch.hpp"

TEST_CASE("Test parseCondExpr", "[parseCondExpr]") {
  SECTION("Test rel_expr i.e. x > y") {
    std::vector<std::shared_ptr<Token>> tokens;

    tokens.push_back(std::static_pointer_cast<Token>(
        std::make_shared<SpecialCharToken>("(")));
    tokens.push_back(
        std::static_pointer_cast<Token>(std::make_shared<WordToken>("x")));
    tokens.push_back(std::static_pointer_cast<Token>(
        std::make_shared<SpecialCharToken>(">")));
    tokens.push_back(
        std::static_pointer_cast<Token>(std::make_shared<WordToken>("y")));
    tokens.push_back(std::static_pointer_cast<Token>(
        std::make_shared<SpecialCharToken>(")")));

    SpParser parser = SpParser(tokens);
    std::shared_ptr<CondExprNode> condExprNode = parser.parseCondExpr();
    REQUIRE(condExprNode->getVariables()->size() == 2);
    REQUIRE(condExprNode->getConstants()->size() == 0);
  }

  SECTION("Test rel_expr i.e. x > 1") {
    std::vector<std::shared_ptr<Token>> tokens;

    tokens.push_back(std::static_pointer_cast<Token>(
        std::make_shared<SpecialCharToken>("(")));
    tokens.push_back(
        std::static_pointer_cast<Token>(std::make_shared<WordToken>("x")));
    tokens.push_back(std::static_pointer_cast<Token>(
        std::make_shared<SpecialCharToken>(">")));
    tokens.push_back(
        std::static_pointer_cast<Token>(std::make_shared<IntegerToken>("1")));
    tokens.push_back(std::static_pointer_cast<Token>(
        std::make_shared<SpecialCharToken>(")")));

    SpParser parser = SpParser(tokens);
    std::shared_ptr<CondExprNode> condExprNode = parser.parseCondExpr();
    REQUIRE(condExprNode->getVariables()->size() == 1);
    REQUIRE(condExprNode->getConstants()->size() == 1);
  }

  SECTION("Test cond_expr i.e. !(x >= 32)") {
    std::vector<std::shared_ptr<Token>> tokens;

    tokens.push_back(std::static_pointer_cast<Token>(
        std::make_shared<SpecialCharToken>("(")));
    tokens.push_back(std::static_pointer_cast<Token>(
        std::make_shared<SpecialCharToken>("!")));
    tokens.push_back(std::static_pointer_cast<Token>(
        std::make_shared<SpecialCharToken>("(")));
    tokens.push_back(
        std::static_pointer_cast<Token>(std::make_shared<WordToken>("x")));
    tokens.push_back(std::static_pointer_cast<Token>(
        std::make_shared<SpecialCharToken>(">=")));
    tokens.push_back(
        std::static_pointer_cast<Token>(std::make_shared<IntegerToken>("32")));
    tokens.push_back(std::static_pointer_cast<Token>(
        std::make_shared<SpecialCharToken>(")")));
    tokens.push_back(std::static_pointer_cast<Token>(
        std::make_shared<SpecialCharToken>(")")));

    SpParser parser = SpParser(tokens);
    std::shared_ptr<CondExprNode> condExprNode = parser.parseCondExpr();
    REQUIRE(condExprNode->getVariables()->size() == 1);
    REQUIRE(condExprNode->getConstants()->size() == 1);
  }

  SECTION("Test cond_expr i.e. (x < 2) && (y > 3)") {
    std::vector<std::shared_ptr<Token>> tokens;

    tokens.push_back(std::static_pointer_cast<Token>(
        std::make_shared<SpecialCharToken>("(")));
    tokens.push_back(std::static_pointer_cast<Token>(
        std::make_shared<SpecialCharToken>("(")));
    tokens.push_back(
        std::static_pointer_cast<Token>(std::make_shared<WordToken>("x")));
    tokens.push_back(std::static_pointer_cast<Token>(
        std::make_shared<SpecialCharToken>("<")));
    tokens.push_back(
        std::static_pointer_cast<Token>(std::make_shared<IntegerToken>("2")));
    tokens.push_back(std::static_pointer_cast<Token>(
        std::make_shared<SpecialCharToken>(")")));
    tokens.push_back(std::static_pointer_cast<Token>(
        std::make_shared<SpecialCharToken>("&&")));
    tokens.push_back(std::static_pointer_cast<Token>(
        std::make_shared<SpecialCharToken>("(")));
    tokens.push_back(
        std::static_pointer_cast<Token>(std::make_shared<WordToken>("y")));
    tokens.push_back(std::static_pointer_cast<Token>(
        std::make_shared<SpecialCharToken>(">")));
    tokens.push_back(
        std::static_pointer_cast<Token>(std::make_shared<IntegerToken>("3")));
    tokens.push_back(std::static_pointer_cast<Token>(
        std::make_shared<SpecialCharToken>(")")));
    tokens.push_back(std::static_pointer_cast<Token>(
        std::make_shared<SpecialCharToken>(")")));

    SpParser parser = SpParser(tokens);
    std::shared_ptr<CondExprNode> condExprNode = parser.parseCondExpr();
    REQUIRE(condExprNode->getVariables()->size() == 2);
    REQUIRE(condExprNode->getConstants()->size() == 2);
  }

  SECTION("Test cond_expr i.e. (x <= 2) || (y >= 3)") {
    std::vector<std::shared_ptr<Token>> tokens;

    tokens.push_back(std::static_pointer_cast<Token>(
        std::make_shared<SpecialCharToken>("(")));
    tokens.push_back(std::static_pointer_cast<Token>(
        std::make_shared<SpecialCharToken>("(")));
    tokens.push_back(
        std::static_pointer_cast<Token>(std::make_shared<WordToken>("x")));
    tokens.push_back(std::static_pointer_cast<Token>(
        std::make_shared<SpecialCharToken>("<=")));
    tokens.push_back(
        std::static_pointer_cast<Token>(std::make_shared<IntegerToken>("2")));
    tokens.push_back(std::static_pointer_cast<Token>(
        std::make_shared<SpecialCharToken>(")")));
    tokens.push_back(std::static_pointer_cast<Token>(
        std::make_shared<SpecialCharToken>("||")));
    tokens.push_back(std::static_pointer_cast<Token>(
        std::make_shared<SpecialCharToken>("(")));
    tokens.push_back(
        std::static_pointer_cast<Token>(std::make_shared<WordToken>("y")));
    tokens.push_back(std::static_pointer_cast<Token>(
        std::make_shared<SpecialCharToken>(">=")));
    tokens.push_back(
        std::static_pointer_cast<Token>(std::make_shared<IntegerToken>("3")));
    tokens.push_back(std::static_pointer_cast<Token>(
        std::make_shared<SpecialCharToken>(")")));
    tokens.push_back(std::static_pointer_cast<Token>(
        std::make_shared<SpecialCharToken>(")")));

    SpParser parser = SpParser(tokens);
    std::shared_ptr<CondExprNode> condExprNode = parser.parseCondExpr();
    REQUIRE(condExprNode->getVariables()->size() == 2);
    REQUIRE(condExprNode->getConstants()->size() == 2);
  }

  SECTION("Test invalid cond_expr i.e. (x <= 2) (y >= 3)") {
    std::vector<std::shared_ptr<Token>> tokens;

    tokens.push_back(std::static_pointer_cast<Token>(
        std::make_shared<SpecialCharToken>("(")));
    tokens.push_back(std::static_pointer_cast<Token>(
        std::make_shared<SpecialCharToken>("(")));
    tokens.push_back(
        std::static_pointer_cast<Token>(std::make_shared<WordToken>("x")));
    tokens.push_back(std::static_pointer_cast<Token>(
        std::make_shared<SpecialCharToken>("<=")));
    tokens.push_back(
        std::static_pointer_cast<Token>(std::make_shared<IntegerToken>("2")));
    tokens.push_back(std::static_pointer_cast<Token>(
        std::make_shared<SpecialCharToken>(")")));
    tokens.push_back(std::static_pointer_cast<Token>(
        std::make_shared<SpecialCharToken>("(")));
    tokens.push_back(
        std::static_pointer_cast<Token>(std::make_shared<WordToken>("y")));
    tokens.push_back(std::static_pointer_cast<Token>(
        std::make_shared<SpecialCharToken>(">=")));
    tokens.push_back(
        std::static_pointer_cast<Token>(std::make_shared<IntegerToken>("3")));
    tokens.push_back(std::static_pointer_cast<Token>(
        std::make_shared<SpecialCharToken>(")")));
    tokens.push_back(std::static_pointer_cast<Token>(
        std::make_shared<SpecialCharToken>(")")));

    SpParser parser = SpParser(tokens);
    REQUIRE_THROWS_AS(parser.parseCondExpr(), std::invalid_argument);
  }

  SECTION("Test invalid rel_expr i.e. y !=== 3") {
    std::vector<std::shared_ptr<Token>> tokens;

    tokens.push_back(std::static_pointer_cast<Token>(
        std::make_shared<SpecialCharToken>("(")));
    tokens.push_back(
        std::static_pointer_cast<Token>(std::make_shared<WordToken>("y")));
    tokens.push_back(std::static_pointer_cast<Token>(
        std::make_shared<SpecialCharToken>("!===")));
    tokens.push_back(
        std::static_pointer_cast<Token>(std::make_shared<IntegerToken>("3")));
    tokens.push_back(std::static_pointer_cast<Token>(
        std::make_shared<SpecialCharToken>(")")));

    SpParser parser = SpParser(tokens);
    REQUIRE_THROWS_AS(parser.parseCondExpr(), std::invalid_argument);
  }

  SECTION("Test invalid rel_expr i.e. x > 2 && y > 3") {
    std::vector<std::shared_ptr<Token>> tokens;

    tokens.push_back(std::static_pointer_cast<Token>(
        std::make_shared<SpecialCharToken>("(")));
    tokens.push_back(
        std::static_pointer_cast<Token>(std::make_shared<WordToken>("x")));
    tokens.push_back(std::static_pointer_cast<Token>(
        std::make_shared<SpecialCharToken>(">")));
    tokens.push_back(
        std::static_pointer_cast<Token>(std::make_shared<IntegerToken>("2")));
    tokens.push_back(std::static_pointer_cast<Token>(
        std::make_shared<SpecialCharToken>("&&")));
    tokens.push_back(
        std::static_pointer_cast<Token>(std::make_shared<WordToken>("y")));
    tokens.push_back(std::static_pointer_cast<Token>(
        std::make_shared<SpecialCharToken>(">")));
    tokens.push_back(
        std::static_pointer_cast<Token>(std::make_shared<IntegerToken>("3")));
    tokens.push_back(std::static_pointer_cast<Token>(
        std::make_shared<SpecialCharToken>(")")));

    SpParser parser = SpParser(tokens);
    REQUIRE_THROWS_AS(parser.parseCondExpr(), std::invalid_argument);
  }

  SECTION("Test empty condExpr should throw error") {
    std::vector<std::shared_ptr<Token>> tokens;
    tokens.push_back(std::static_pointer_cast<Token>(
        std::make_shared<SpecialCharToken>("(")));
    tokens.push_back(std::static_pointer_cast<Token>(
        std::make_shared<SpecialCharToken>(")")));
    SpParser parser = SpParser(tokens);
    REQUIRE_THROWS_AS(parser.parseCondExpr(), std::invalid_argument);
  }
}