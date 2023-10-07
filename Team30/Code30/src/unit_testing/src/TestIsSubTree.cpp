#include "../../spa/src/shared/parser/node/TreeNode.h"
#include "catch.hpp"

TEST_CASE("Test if a tree is a subtree or same tree as another tree",
          "[IsSubTree]") {
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

    bool isSubTree = TreeNode::IsSubTree(baseTree, subTree);

    REQUIRE(isSubTree);
  }

  SECTION(
      "Test base tree: v + x * y + z * t, subtree: x * y, should return true") {
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

    bool isSubTree = TreeNode::IsSubTree(baseTree, subTree);

    REQUIRE(isSubTree);
  }

  SECTION(
      "Test base tree: v + x * y + z * t, subtree: x * y + z * t, should "
      "return false") {
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

    std::shared_ptr<TreeNode> subTreeLeftLeft =
        std::make_shared<TreeNode>("x", nullptr, nullptr);
    std::shared_ptr<TreeNode> subTreeLeftRight =
        std::make_shared<TreeNode>("y", nullptr, nullptr);
    std::shared_ptr<TreeNode> subTreeLeft =
        std::make_shared<TreeNode>("*", subTreeLeftLeft, subTreeLeftRight);
    std::shared_ptr<TreeNode> subTreeRightLeft =
        std::make_shared<TreeNode>("z", nullptr, nullptr);
    std::shared_ptr<TreeNode> subTreeRightRight =
        std::make_shared<TreeNode>("t", nullptr, nullptr);
    std::shared_ptr<TreeNode> subTreeRight =
        std::make_shared<TreeNode>("*", subTreeRightLeft, subTreeRightRight);
    std::shared_ptr<TreeNode> subTree =
        std::make_shared<TreeNode>("+", subTreeLeft, subTreeRight);

    bool isSubTree = TreeNode::IsSubTree(baseTree, subTree);

    REQUIRE(isSubTree == false);
  }
}
