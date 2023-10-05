#include "../../spa/src/source_processor/node/util_node/TreeNode.h"
#include "catch.hpp"

TEST_CASE("Test if a tree is the same tree as another tree", "[IsSameTree]") {
  SECTION("Test base tree: x + y, subtree: x + y, should return true") {
    std::shared_ptr<TreeNode> baseTreeLeft =
        std::make_shared<TreeNode>("x", nullptr, nullptr);
    std::shared_ptr<TreeNode> baseTreeRight =
        std::make_shared<TreeNode>("y", nullptr, nullptr);
    std::shared_ptr<TreeNode> baseTree =
        std::make_shared<TreeNode>("+", baseTreeLeft, baseTreeRight);

    std::shared_ptr<TreeNode> subTreeLeft =
        std::make_shared<TreeNode>("x", nullptr, nullptr);
    std::shared_ptr<TreeNode> subTreeRight =
        std::make_shared<TreeNode>("y", nullptr, nullptr);
    std::shared_ptr<TreeNode> subTree =
        std::make_shared<TreeNode>("+", subTreeLeft, subTreeRight);

    bool isSameTree = TreeNode::IsSameTree(baseTree, subTree);

    REQUIRE(isSameTree);
  }

  SECTION("Test base tree: x + y, subtree: z + y, should return true") {
    std::shared_ptr<TreeNode> baseTreeLeft =
        std::make_shared<TreeNode>("x", nullptr, nullptr);
    std::shared_ptr<TreeNode> baseTreeRight =
        std::make_shared<TreeNode>("y", nullptr, nullptr);
    std::shared_ptr<TreeNode> baseTree =
        std::make_shared<TreeNode>("+", baseTreeLeft, baseTreeRight);

    std::shared_ptr<TreeNode> subTreeLeft =
        std::make_shared<TreeNode>("z", nullptr, nullptr);
    std::shared_ptr<TreeNode> subTreeRight =
        std::make_shared<TreeNode>("y", nullptr, nullptr);
    std::shared_ptr<TreeNode> subTree =
        std::make_shared<TreeNode>("+", subTreeLeft, subTreeRight);

    bool isSameTree = TreeNode::IsSameTree(baseTree, subTree);

    REQUIRE(isSameTree == false);
  }

  SECTION(
      "Test base tree: v + x * y + z * t, subtree: x * y, should return "
      "false") {
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

    std::shared_ptr<TreeNode> baseTree =
        std::make_shared<TreeNode>("+", left, right);

    std::shared_ptr<TreeNode> subTreeLeft =
        std::make_shared<TreeNode>("x", nullptr, nullptr);
    std::shared_ptr<TreeNode> subTreeRight =
        std::make_shared<TreeNode>("y", nullptr, nullptr);
    std::shared_ptr<TreeNode> subTree =
        std::make_shared<TreeNode>("*", subTreeLeft, subTreeRight);

    bool isSameTree = TreeNode::IsSameTree(baseTree, subTree);

    REQUIRE(isSameTree == false);
  }

  SECTION(
      "Test base tree: v + x * y + z * t, subtree: x * y, should return "
      "false") {
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

    std::shared_ptr<TreeNode> baseTree =
        std::make_shared<TreeNode>("+", left, right);

    std::shared_ptr<TreeNode> subLeftRightLeft =
        std::make_shared<TreeNode>("x", nullptr, nullptr);
    std::shared_ptr<TreeNode> subLeftRightRight =
        std::make_shared<TreeNode>("y", nullptr, nullptr);

    std::shared_ptr<TreeNode> subLeftLeft =
        std::make_shared<TreeNode>("v", nullptr, nullptr);
    std::shared_ptr<TreeNode> subLeftRight =
        std::make_shared<TreeNode>("*", subLeftRightLeft, subLeftRightRight);
    std::shared_ptr<TreeNode> subRightLeft =
        std::make_shared<TreeNode>("z", nullptr, nullptr);
    std::shared_ptr<TreeNode> subRightRight =
        std::make_shared<TreeNode>("t", nullptr, nullptr);

    std::shared_ptr<TreeNode> subLeft =
        std::make_shared<TreeNode>("+", subLeftLeft, subLeftRight);
    std::shared_ptr<TreeNode> subRight =
        std::make_shared<TreeNode>("*", subRightLeft, subRightRight);

    std::shared_ptr<TreeNode> subTree =
        std::make_shared<TreeNode>("+", subLeft, subRight);

    bool isSameTree = TreeNode::IsSameTree(baseTree, subTree);

    REQUIRE(isSameTree == true);
  }
}