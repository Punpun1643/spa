#include "TreeNode.h"

TreeNode::TreeNode(std::string val, std::shared_ptr<TreeNode> leftSubTree,
                   std::shared_ptr<TreeNode> rightSubTree)
    : val(val), leftSubTree(leftSubTree), rightSubTree(rightSubTree) {}

std::string TreeNode::getVal() const { return val; }

std::shared_ptr<TreeNode> TreeNode::getLeftSubTree() const {
  return leftSubTree;
}

std::shared_ptr<TreeNode> TreeNode::getRightSubTree() const {
  return rightSubTree;
}

std::vector<std::string> TreeNode::CreateInOrderTraversal(
    std::shared_ptr<TreeNode> const& root) {
  std::vector<std::string> inOrderTraversal;

  if (root == nullptr) {
    return inOrderTraversal;
  }

  inOrderTraversal = CreateInOrderTraversal(root->getLeftSubTree());
  inOrderTraversal.push_back(root->getVal());

  std::vector<std::string> rightSubTreeInOrderTraversal =
      CreateInOrderTraversal(root->getRightSubTree());

  inOrderTraversal.insert(
      inOrderTraversal.end(),
      std::make_move_iterator(rightSubTreeInOrderTraversal.begin()),
      std::make_move_iterator(rightSubTreeInOrderTraversal.end()));

  return inOrderTraversal;
}

std::vector<std::string> TreeNode::CreatePreOrderTraversal(
    std::shared_ptr<TreeNode> const& root) {
  std::vector<std::string> preOrderTraversal;

  if (root == nullptr) {
    return preOrderTraversal;
  }

  preOrderTraversal.push_back(root->getVal());

  std::vector<std::string> leftSubTreePreOrderTraversal =
      CreatePreOrderTraversal(root->getLeftSubTree());

  preOrderTraversal.insert(
      preOrderTraversal.end(),
      std::make_move_iterator(leftSubTreePreOrderTraversal.begin()),
      std::make_move_iterator(leftSubTreePreOrderTraversal.end()));

  std::vector<std::string> rightSubTreePreOrderTraversal =
      CreatePreOrderTraversal(root->getRightSubTree());

  preOrderTraversal.insert(
      preOrderTraversal.end(),
      std::make_move_iterator(rightSubTreePreOrderTraversal.begin()),
      std::make_move_iterator(rightSubTreePreOrderTraversal.end()));

  return preOrderTraversal;
}

bool TreeNode::IsSubString(
    std::vector<std::string> const& baseTreeInOrderTraversal,
    std::vector<std::string> const& subTreeInOrderTraversal) {
  int baseTreeInOrderTraversalSize = baseTreeInOrderTraversal.size();
  int subTreeInOrderTraversalSize = subTreeInOrderTraversal.size();

  if (subTreeInOrderTraversalSize > baseTreeInOrderTraversalSize) {
    return false;
  }

  std::vector<int> longestProperPrefixSuffix(subTreeInOrderTraversalSize, 0);
  int prevLength = 0;
  int pointerToElement = 1;

  while (pointerToElement < subTreeInOrderTraversalSize) {
    if (subTreeInOrderTraversal[pointerToElement] ==
        subTreeInOrderTraversal[prevLength]) {
      ++prevLength;
      longestProperPrefixSuffix[pointerToElement] = prevLength;
      ++pointerToElement;
    } else {
      if (prevLength == 0) {
        longestProperPrefixSuffix[pointerToElement] = 0;
        ++pointerToElement;
      } else {
        prevLength = longestProperPrefixSuffix[prevLength - 1];
      }
    }
  }

  int baseTreePointer = 0;
  int subTreePointer = 0;

  while (baseTreePointer < baseTreeInOrderTraversalSize) {
    if (baseTreeInOrderTraversal[baseTreePointer] ==
        subTreeInOrderTraversal[subTreePointer]) {
      ++baseTreePointer;
      ++subTreePointer;

      if (subTreePointer == subTreeInOrderTraversalSize) {
        return true;
      }
    } else {
      if (subTreePointer == 0) {
        ++baseTreePointer;
      } else {
        subTreePointer = longestProperPrefixSuffix[subTreePointer - 1];
      }
    }
  }

  return false;
}

bool TreeNode::IsSubTree(std::shared_ptr<TreeNode> const& baseTreeRoot,
                         std::shared_ptr<TreeNode> const& subTreeRoot) {
  if (subTreeRoot == nullptr) {
    return true;
  }

  std::vector<std::string> baseTreeInOrderTraversal =
      CreateInOrderTraversal(baseTreeRoot);
  std::vector<std::string> subTreeInOrderTraversal =
      CreateInOrderTraversal(subTreeRoot);

  std::vector<std::string> baseTreePreOrderTraversal =
      CreatePreOrderTraversal(baseTreeRoot);
  std::vector<std::string> subTreePreOrderTraversal =
      CreatePreOrderTraversal(subTreeRoot);

  return IsSubString(baseTreeInOrderTraversal, subTreeInOrderTraversal) &&
         IsSubString(baseTreePreOrderTraversal, subTreePreOrderTraversal);
}
