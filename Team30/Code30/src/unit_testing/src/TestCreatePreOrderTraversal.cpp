#include "../../spa/src/shared/parser/node/TreeNode.h"
#include "catch.hpp"

TEST_CASE("Test creating of pre-order traversal from expression tree",
          "[CreatePreOrderTraversal]") {
  SECTION(
      "Test creation of pre-order traversal from expression tree of x + y") {
    std::shared_ptr<TreeNode> left =
        std::make_shared<TreeNode>("x", nullptr, nullptr);
    std::shared_ptr<TreeNode> right =
        std::make_shared<TreeNode>("y", nullptr, nullptr);
    std::shared_ptr<TreeNode> root =
        std::make_shared<TreeNode>("+", left, right);

    std::vector<std::string> preOrderTraversalString =
        TreeNode::CreatePreOrderTraversal(root);

    REQUIRE(preOrderTraversalString.size() == 3);
    REQUIRE(preOrderTraversalString.at(0) == "+");
    REQUIRE(preOrderTraversalString.at(1) == "x");
    REQUIRE(preOrderTraversalString.at(2) == "y");
  }

  SECTION(
      "Test creation of pre-order traversal from expression tree of v + x * y "
      "+ "
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

    std::vector<std::string> preOrderTraversalString =
        TreeNode::CreatePreOrderTraversal(root);

    REQUIRE(preOrderTraversalString.size() == 9);
    REQUIRE(preOrderTraversalString.at(0) == "+");
    REQUIRE(preOrderTraversalString.at(1) == "+");
    REQUIRE(preOrderTraversalString.at(2) == "v");
    REQUIRE(preOrderTraversalString.at(3) == "*");
    REQUIRE(preOrderTraversalString.at(4) == "x");
    REQUIRE(preOrderTraversalString.at(5) == "y");
    REQUIRE(preOrderTraversalString.at(6) == "*");
    REQUIRE(preOrderTraversalString.at(7) == "z");
    REQUIRE(preOrderTraversalString.at(8) == "t");
  }
}
