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

//bool TreeNode::isSubTree()