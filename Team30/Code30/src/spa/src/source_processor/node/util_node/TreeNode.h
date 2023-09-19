#pragma once

#include <memory>
#include <string>

class TreeNode {
 public:
  TreeNode(std::string val, std::shared_ptr<TreeNode> leftSubTree,
           std::shared_ptr<TreeNode> rightSubTree);

  std::string getVal() const;

  std::shared_ptr<TreeNode> getLeftSubTree() const;

  std::shared_ptr<TreeNode> getRightSubTree() const;

  ~TreeNode() = default;

 private:
  std::string val;
  std::shared_ptr<TreeNode> leftSubTree;
  std::shared_ptr<TreeNode> rightSubTree;
};