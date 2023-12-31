#include <memory>

#include "../../../../spa/src/shared/tokenizer/token/EofToken.h"
#include "../../../../spa/src/shared/tokenizer/token/IntegerToken.h"
#include "../../../../spa/src/shared/tokenizer/token/SpecialCharToken.h"
#include "../../../../spa/src/shared/tokenizer/token/Token.h"
#include "../../../../spa/src/shared/tokenizer/token/WordToken.h"
#include "../../../../spa/src/source_processor/node/util_node/CondExprNode.h"
#include "../../../../spa/src/source_processor/parser/SpParser.h"
#include "catch.hpp"

namespace TestParseCondExprNode {
class TestableParser : public SpParser {
 public:
  TestableParser(std::vector<std::shared_ptr<Token>> tokens)
      : SpParser(tokens) {}

  using SpParser::ParseCondExpr;

  void Parse() override {}
};
}  // namespace TestParseCondExprNode

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

    TestParseCondExprNode::TestableParser parser =
        TestParseCondExprNode::TestableParser(tokens);
    std::shared_ptr<CondExprNode> condExprNode = parser.ParseCondExpr();
    REQUIRE(condExprNode->GetVariables()->size() == 2);
    REQUIRE(condExprNode->GetConstants()->size() == 0);
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

    TestParseCondExprNode::TestableParser parser =
        TestParseCondExprNode::TestableParser(tokens);
    std::shared_ptr<CondExprNode> condExprNode = parser.ParseCondExpr();
    REQUIRE(condExprNode->GetVariables()->size() == 1);
    REQUIRE(condExprNode->GetConstants()->size() == 1);
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

    TestParseCondExprNode::TestableParser parser =
        TestParseCondExprNode::TestableParser(tokens);
    std::shared_ptr<CondExprNode> condExprNode = parser.ParseCondExpr();
    REQUIRE(condExprNode->GetVariables()->size() == 1);
    REQUIRE(condExprNode->GetConstants()->size() == 1);
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

    TestParseCondExprNode::TestableParser parser =
        TestParseCondExprNode::TestableParser(tokens);
    std::shared_ptr<CondExprNode> condExprNode = parser.ParseCondExpr();
    REQUIRE(condExprNode->GetVariables()->size() == 2);
    REQUIRE(condExprNode->GetConstants()->size() == 2);
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

    TestParseCondExprNode::TestableParser parser =
        TestParseCondExprNode::TestableParser(tokens);
    std::shared_ptr<CondExprNode> condExprNode = parser.ParseCondExpr();
    REQUIRE(condExprNode->GetVariables()->size() == 2);
    REQUIRE(condExprNode->GetConstants()->size() == 2);
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

    TestParseCondExprNode::TestableParser parser =
        TestParseCondExprNode::TestableParser(tokens);
    REQUIRE_THROWS_AS(parser.ParseCondExpr(), std::invalid_argument);
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

    TestParseCondExprNode::TestableParser parser =
        TestParseCondExprNode::TestableParser(tokens);
    REQUIRE_THROWS_AS(parser.ParseCondExpr(), std::invalid_argument);
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

    TestParseCondExprNode::TestableParser parser =
        TestParseCondExprNode::TestableParser(tokens);
    REQUIRE_THROWS_AS(parser.ParseCondExpr(), std::invalid_argument);
  }

  SECTION("Test empty condExpr should throw error") {
    std::vector<std::shared_ptr<Token>> tokens;
    tokens.push_back(std::static_pointer_cast<Token>(
        std::make_shared<SpecialCharToken>("(")));
    tokens.push_back(std::static_pointer_cast<Token>(
        std::make_shared<SpecialCharToken>(")")));

    TestParseCondExprNode::TestableParser parser =
        TestParseCondExprNode::TestableParser(tokens);
    REQUIRE_THROWS_AS(parser.ParseCondExpr(), std::invalid_argument);
  }
}
