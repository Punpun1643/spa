#include <memory>
#include <queue>

#include "../../spa/src/shared/parser/AParser.h"
#include "../../spa/src/shared/tokenizer/token/EofToken.h"
#include "../../spa/src/shared/tokenizer/token/IntegerToken.h"
#include "../../spa/src/shared/tokenizer/token/SpecialCharToken.h"
#include "../../spa/src/shared/tokenizer/token/Token.h"
#include "../../spa/src/shared/tokenizer/token/WordToken.h"
#include "../../spa/src/source_processor/node/stmt_node/AssignNode.h"
#include "../../spa/src/source_processor/node/util_node/CondExprNode.h"
#include "catch.hpp"

namespace TestBuildExprTreeAndValidate {
class TestableParser : public AParser {
 public:
  TestableParser(std::vector<std::shared_ptr<Token>> tokens)
      : AParser(tokens) {}

  using AParser::BuildExprTreeAndValidate;

  void parse() override {}
};
}  // namespace TestBuildExprTreeAndValidate

TEST_CASE("Test build binary tree from postfix expression",
          "[BuildExprTreeAndValidate]") {
  SECTION(
      "Test build binary tree from postfix expression with x, y, + should not "
      "throw error (i.e. infix x + y)") {
    std::queue<std::shared_ptr<std::string>> postFixQueue;

    postFixQueue.push(std::make_shared<std::string>("x"));
    postFixQueue.push(std::make_shared<std::string>("y"));
    postFixQueue.push(std::make_shared<std::string>("+"));

    REQUIRE_NOTHROW(
        TestBuildExprTreeAndValidate::TestableParser::BuildExprTreeAndValidate(
            postFixQueue));
  }

  SECTION(
      "Test build binary tree from postfix expression with infix x + y should "
      "build a correct tree") {
    std::queue<std::shared_ptr<std::string>> postFixQueue;

    postFixQueue.push(std::make_shared<std::string>("x"));
    postFixQueue.push(std::make_shared<std::string>("y"));
    postFixQueue.push(std::make_shared<std::string>("+"));

    std::shared_ptr<TreeNode> treeNode =
        TestBuildExprTreeAndValidate::TestableParser::BuildExprTreeAndValidate(
            postFixQueue);

    REQUIRE(treeNode->getVal() == "+");
    REQUIRE(treeNode->getLeftSubTree()->getVal() == "x");
    REQUIRE(treeNode->getRightSubTree()->getVal() == "y");
  }

  SECTION(
      "Test build binary tree from postfix expression with infix x + y * z "
      "should build a correct tree") {
    std::queue<std::shared_ptr<std::string>> postFixQueue;

    postFixQueue.push(std::make_shared<std::string>("x"));
    postFixQueue.push(std::make_shared<std::string>("y"));
    postFixQueue.push(std::make_shared<std::string>("z"));
    postFixQueue.push(std::make_shared<std::string>("*"));
    postFixQueue.push(std::make_shared<std::string>("+"));

    std::shared_ptr<TreeNode> treeNode =
        TestBuildExprTreeAndValidate::TestableParser::BuildExprTreeAndValidate(
            postFixQueue);

    REQUIRE(treeNode->getVal() == "+");
    REQUIRE(treeNode->getLeftSubTree()->getVal() == "x");
    REQUIRE(treeNode->getRightSubTree()->getVal() == "*");
    REQUIRE(treeNode->getRightSubTree()->getLeftSubTree()->getVal() == "y");
    REQUIRE(treeNode->getRightSubTree()->getRightSubTree()->getVal() == "z");
  }

  SECTION(
      "Test build binary tree from postfix expression with infix ((x + z) * 5) "
      "should not throw error") {
    std::queue<std::shared_ptr<std::string>> postFixQueue;

    postFixQueue.push(std::make_shared<std::string>("x"));
    postFixQueue.push(std::make_shared<std::string>("z"));
    postFixQueue.push(std::make_shared<std::string>("+"));
    postFixQueue.push(std::make_shared<std::string>("5"));
    postFixQueue.push(std::make_shared<std::string>("*"));

    REQUIRE_NOTHROW(
        TestBuildExprTreeAndValidate::TestableParser::BuildExprTreeAndValidate(
            postFixQueue));
  }

  SECTION(
      "Test build binary tree from postfix expression with infix ((x + z) * 5) "
      "should build a correct binary tree") {
    std::queue<std::shared_ptr<std::string>> postFixQueue;

    postFixQueue.push(std::make_shared<std::string>("x"));
    postFixQueue.push(std::make_shared<std::string>("z"));
    postFixQueue.push(std::make_shared<std::string>("+"));
    postFixQueue.push(std::make_shared<std::string>("5"));
    postFixQueue.push(std::make_shared<std::string>("*"));

    std::shared_ptr<TreeNode> treeNode =
        TestBuildExprTreeAndValidate::TestableParser::BuildExprTreeAndValidate(
            postFixQueue);

    REQUIRE(treeNode->getVal() == "*");
    REQUIRE(treeNode->getLeftSubTree()->getVal() == "+");
    REQUIRE(treeNode->getRightSubTree()->getVal() == "5");
    REQUIRE(treeNode->getLeftSubTree()->getLeftSubTree()->getVal() == "x");
    REQUIRE(treeNode->getLeftSubTree()->getRightSubTree()->getVal() == "z");
  }

  SECTION(
      "Test invalid postfix queue with 2, +, x, + should throw error (i.e. "
      "infix 2 + + "
      "x") {
    std::queue<std::shared_ptr<std::string>> postFixQueue;

    postFixQueue.push(std::make_shared<std::string>("2"));
    postFixQueue.push(std::make_shared<std::string>("+"));
    postFixQueue.push(std::make_shared<std::string>("x"));
    postFixQueue.push(std::make_shared<std::string>("+"));

    REQUIRE_THROWS_AS(
        TestBuildExprTreeAndValidate::TestableParser::BuildExprTreeAndValidate(
            postFixQueue),
        std::invalid_argument);
  }
}