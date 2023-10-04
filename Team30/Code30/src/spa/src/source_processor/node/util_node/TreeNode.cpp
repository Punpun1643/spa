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

std::vector<std::string> TreeNode::createInOrderTraversal(
    std::shared_ptr<TreeNode> const& root) {
  std::vector<std::string> inOrderTraversal;

  if (root == nullptr) {
    return inOrderTraversal;
  }

  inOrderTraversal =
      createInOrderTraversal(root->getLeftSubTree());
  inOrderTraversal.push_back(root->getVal());

  std::vector<std::string> rightSubTreeInOrderTraversal =
      createInOrderTraversal(root->getRightSubTree());

  inOrderTraversal.insert(
      inOrderTraversal.end(),
      std::make_move_iterator(rightSubTreeInOrderTraversal.begin()),
      std::make_move_iterator(rightSubTreeInOrderTraversal.end()));

  return inOrderTraversal;
}
