#pragma once

#include <memory>
#include <string>
#include <vector>

class TreeNode {
 public:
  TreeNode(std::string val, std::shared_ptr<TreeNode> leftSubTree,
           std::shared_ptr<TreeNode> rightSubTree);

  /**
   * @brief Returns the value of the node.
   *
   * @return The value of the node.
   */
  std::string GetVal() const;

  /**
   * @brief Returns the left subtree of the node.
   *
   * @return The left subtree of the node.
   */
  std::shared_ptr<TreeNode> GetLeftSubTree() const;

  /**
   * @brief Returns the right subtree of the node.
   *
   * @return The right subtree of the node.
   */
  std::shared_ptr<TreeNode> GetRightSubTree() const;

  /**
   * @brief Creates an in-order traversal of the tree.
   */
  static std::vector<std::string> CreateInOrderTraversal(
      std::shared_ptr<TreeNode> const& root);

  /**
   * @brief Creates a pre-order traversal of the tree.
   *
   * @return A pre-order traversal of the tree.
   */
  static std::vector<std::string> CreatePreOrderTraversal(
      std::shared_ptr<TreeNode> const& root);

  /**
   * @brief Checks if the sub-tree is a sub-tree of the base tree.
   *
   * @return True if the sub-tree is a sub-tree of the base tree, false
   * otherwise.
   */
  static bool IsSubTree(std::shared_ptr<TreeNode> const& baseTreeRoot,
                        std::shared_ptr<TreeNode> const& subTreeRoot);

  /**
   * @brief Checks if the sub-tree is a sub-string of the base tree.
   *
   * @return True if the sub-tree is a sub-string of the base tree, false
   * otherwise.
   */
  static bool IsSubString(
      std::vector<std::string> const& baseTreeInOrderTraversal,
      std::vector<std::string> const& subTreeInOrderTraversal);

  /**
   * @brief Checks if the sub-tree is a sub-tree of the base tree.
   *
   * @return True if the sub-tree is a sub-tree of the base tree, false
   * otherwise.
   */
  static bool IsSameTree(std::shared_ptr<TreeNode> const& baseTreeRoot,
                         std::shared_ptr<TreeNode> const& subTreeRoot);

  ~TreeNode() = default;

 private:
  std::string val;
  std::shared_ptr<TreeNode> leftSubTree;
  std::shared_ptr<TreeNode> rightSubTree;
};