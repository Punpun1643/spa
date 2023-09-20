#include <memory>
#include <queue>

#include "../../spa/src/shared/tokenizer/token/EofToken.h"
#include "../../spa/src/shared/tokenizer/token/IntegerToken.h"
#include "../../spa/src/shared/tokenizer/token/SpecialCharToken.h"
#include "../../spa/src/shared/tokenizer/token/Token.h"
#include "../../spa/src/shared/tokenizer/token/WordToken.h"
#include "../../spa/src/source_processor/node/stmt_node/AssignNode.h"
#include "../../spa/src/source_processor/node/util_node/CondExprNode.h"
#include "../../spa/src/source_processor/parser/SpParser.h"
#include "catch.hpp"

TEST_CASE("Test buildExprTreeAndValidate", "[buildExprTreeAndValidate]") {
  SECTION(
      "postfix queue with x, y, + should not throw error (i.e. infix x + y)") {
    std::queue<std::shared_ptr<std::string>> postFixQueue;

    postFixQueue.push(std::make_shared<std::string>("x"));
    postFixQueue.push(std::make_shared<std::string>("y"));
    postFixQueue.push(std::make_shared<std::string>("+"));

    REQUIRE_NOTHROW(SpParser::buildExprTreeAndValidate(postFixQueue));
  }

  SECTION(
      "postfix queue with x, y, + should build a correct tree (i.e. infix x + "
      "y)") {
    std::queue<std::shared_ptr<std::string>> postFixQueue;

    postFixQueue.push(std::make_shared<std::string>("x"));
    postFixQueue.push(std::make_shared<std::string>("y"));
    postFixQueue.push(std::make_shared<std::string>("+"));

    // tree node from buildExprTreeAndValidate
    std::shared_ptr<TreeNode> treeNode =
        SpParser::buildExprTreeAndValidate(postFixQueue);
    REQUIRE(treeNode->getLeftSubTree()->getVal() == "x");
    REQUIRE(treeNode->getRightSubTree()->getVal() == "y");
  }

  SECTION("postfix queue with (i.e. infix x + y * z)") {
    std::queue<std::shared_ptr<std::string>> postFixQueue;

    postFixQueue.push(std::make_shared<std::string>("x"));
    postFixQueue.push(std::make_shared<std::string>("y"));
    postFixQueue.push(std::make_shared<std::string>("z"));
    postFixQueue.push(std::make_shared<std::string>("*"));
    postFixQueue.push(std::make_shared<std::string>("+"));

    // tree node from buildExprTreeAndValidate
    std::shared_ptr<TreeNode> treeNode =
        SpParser::buildExprTreeAndValidate(postFixQueue);
    REQUIRE(treeNode->getLeftSubTree()->getVal() == "x");
    REQUIRE(treeNode->getRightSubTree()->getVal() == "*");
    REQUIRE(treeNode->getRightSubTree()->getLeftSubTree()->getVal() == "y");
    REQUIRE(treeNode->getRightSubTree()->getRightSubTree()->getVal() == "z");
  }

  SECTION(
      "postfix queue with 2, +, x, + should not throw error (i.e. infix 2 + + "
      "x") {
    std::queue<std::shared_ptr<std::string>> postFixQueue;

    postFixQueue.push(std::make_shared<std::string>("2"));
    postFixQueue.push(std::make_shared<std::string>("+"));
    postFixQueue.push(std::make_shared<std::string>("x"));
    postFixQueue.push(std::make_shared<std::string>("+"));

    REQUIRE_THROWS(SpParser::buildExprTreeAndValidate(postFixQueue));
  }

  SECTION(
      "postfix queue with x, z, +, 5, * should not throw error (i.e. infix (x "
      "+ z) * 5)") {
    std::queue<std::shared_ptr<std::string>> postFixQueue;

    postFixQueue.push(std::make_shared<std::string>("x"));
    postFixQueue.push(std::make_shared<std::string>("z"));
    postFixQueue.push(std::make_shared<std::string>("+"));
    postFixQueue.push(std::make_shared<std::string>("5"));
    postFixQueue.push(std::make_shared<std::string>("*"));

    REQUIRE_NOTHROW(SpParser::buildExprTreeAndValidate(postFixQueue));
  }
}
