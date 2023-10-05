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

  static std::vector<std::string> CreateInOrderTraversal(
      std::shared_ptr<TreeNode> const& root);

  static std::vector<std::string> CreatePreOrderTraversal(
      std::shared_ptr<TreeNode> const& root);

  static bool IsSubTree(std::shared_ptr<TreeNode> const& baseTreeRoot,
                        std::shared_ptr<TreeNode> const& subTreeRoot);

  static bool IsSubString(std::vector<std::string> const& baseTreeInOrderTraversal,
                          std::vector<std::string> const& subTreeInOrderTraversal);

  ~TreeNode() = default;

 private:
  std::string val;
  std::shared_ptr<TreeNode> leftSubTree;
  std::shared_ptr<TreeNode> rightSubTree;
};