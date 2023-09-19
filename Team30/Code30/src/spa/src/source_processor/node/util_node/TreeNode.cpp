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
