#pragma once

#include <memory>
#include <string>
#include <vector>

class TreeNode {
 public:
  TreeNode(std::string val, std::shared_ptr<TreeNode> leftSubTree,
           std::shared_ptr<TreeNode> rightSubTree);

  std::string getVal() const;

  std::shared_ptr<TreeNode> getLeftSubTree() const;

  std::shared_ptr<TreeNode> getRightSubTree() const;

  static std::vector<std::string> createInOrderTraversal(
      std::shared_ptr<TreeNode> const& root);

  static std::vector<std::string> createPreOrderTraversal(
      std::shared_ptr<TreeNode> const& root);

  static bool isSubTree(std::shared_ptr<TreeNode> const& baseTreeRoot,
                        std::shared_ptr<TreeNode> const& subTreeRoot);

  ~TreeNode() = default;

 private:
  std::string val;
  std::shared_ptr<TreeNode> leftSubTree;
  std::shared_ptr<TreeNode> rightSubTree;
};