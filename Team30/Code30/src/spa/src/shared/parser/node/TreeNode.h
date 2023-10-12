#pragma once

#include <memory>
#include <string>
#include <vector>

class TreeNode {
 public:
  TreeNode(std::string val, std::shared_ptr<TreeNode> leftSubTree,
           std::shared_ptr<TreeNode> rightSubTree);

  std::string GetVal() const;

  std::shared_ptr<TreeNode> GetLeftSubTree() const;

  std::shared_ptr<TreeNode> GetRightSubTree() const;

  static std::vector<std::string> CreateInOrderTraversal(
      std::shared_ptr<TreeNode> const& root);

  static std::vector<std::string> CreatePreOrderTraversal(
      std::shared_ptr<TreeNode> const& root);

  static bool IsSubTree(std::shared_ptr<TreeNode> const& baseTreeRoot,
                        std::shared_ptr<TreeNode> const& subTreeRoot);

  static bool IsSubString(
      std::vector<std::string> const& baseTreeInOrderTraversal,
      std::vector<std::string> const& subTreeInOrderTraversal);

  static bool IsSameTree(std::shared_ptr<TreeNode> const& baseTreeRoot,
                         std::shared_ptr<TreeNode> const& subTreeRoot);

  ~TreeNode() = default;

 private:
  std::string val;
  std::shared_ptr<TreeNode> leftSubTree;
  std::shared_ptr<TreeNode> rightSubTree;
};