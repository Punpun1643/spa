#include "../../spa/src/shared/tokenizer/token/EofToken.h"
#include "../../spa/src/shared/tokenizer/token/IntegerToken.h"
#include "../../spa/src/shared/tokenizer/token/SpecialCharToken.h"
#include "../../spa/src/shared/tokenizer/token/Token.h"
#include "../../spa/src/shared/tokenizer/token/WordToken.h"
#include "../../spa/src/source_processor/node/stmt_node/AssignNode.h"
#include "../../spa/src/source_processor/node/util_node/CondExprNode.h"
#include "../../spa/src/source_processor/parser/SpParser.h"
#include "catch.hpp"

TEST_CASE("Test creating of in-order traversal from expression tree",
          "[CreateInOrderTraversal]") {
  SECTION("Test creation of in-order traversal from expression tree of x + y") {
    std::shared_ptr<TreeNode> left =
        std::make_shared<TreeNode>("x", nullptr, nullptr);
    std::shared_ptr<TreeNode> right =
        std::make_shared<TreeNode>("y", nullptr, nullptr);
    std::shared_ptr<TreeNode> root =
        std::make_shared<TreeNode>("+", left, right);

    std::vector<std::string> inOrderTraversalString =
        TreeNode::CreateInOrderTraversal(root);

    REQUIRE(inOrderTraversalString.size() == 3);
    REQUIRE(inOrderTraversalString.at(0) == "x");
    REQUIRE(inOrderTraversalString.at(1) == "+");
    REQUIRE(inOrderTraversalString.at(2) == "y");
  }

  SECTION(
      "Test creation of in-order traversal from expression tree of v + x * y + "
      "z * t") {
    std::shared_ptr<TreeNode> leftRightLeft =
        std::make_shared<TreeNode>("x", nullptr, nullptr);
    std::shared_ptr<TreeNode> leftRightRight =
        std::make_shared<TreeNode>("y", nullptr, nullptr);

    std::shared_ptr<TreeNode> leftLeft =
        std::make_shared<TreeNode>("v", nullptr, nullptr);
    std::shared_ptr<TreeNode> leftRight =
        std::make_shared<TreeNode>("*", leftRightLeft, leftRightRight);
    std::shared_ptr<TreeNode> rightLeft =
        std::make_shared<TreeNode>("z", nullptr, nullptr);
    std::shared_ptr<TreeNode> rightRight =
        std::make_shared<TreeNode>("t", nullptr, nullptr);

    std::shared_ptr<TreeNode> left =
        std::make_shared<TreeNode>("+", leftLeft, leftRight);
    std::shared_ptr<TreeNode> right =
        std::make_shared<TreeNode>("*", rightLeft, rightRight);

    std::shared_ptr<TreeNode> root =
        std::make_shared<TreeNode>("+", left, right);

    std::vector<std::string> inOrderTraversalString =
        TreeNode::CreateInOrderTraversal(root);

    REQUIRE(inOrderTraversalString.size() == 9);
    REQUIRE(inOrderTraversalString.at(0) == "v");
    REQUIRE(inOrderTraversalString.at(1) == "+");
    REQUIRE(inOrderTraversalString.at(2) == "x");
    REQUIRE(inOrderTraversalString.at(3) == "*");
    REQUIRE(inOrderTraversalString.at(4) == "y");
    REQUIRE(inOrderTraversalString.at(5) == "+");
    REQUIRE(inOrderTraversalString.at(6) == "z");
    REQUIRE(inOrderTraversalString.at(7) == "*");
    REQUIRE(inOrderTraversalString.at(8) == "t");
  }
}
