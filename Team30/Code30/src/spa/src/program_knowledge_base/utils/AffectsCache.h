#pragma once

#include <memory>
#include <string>
#include <unordered_map>
#include <unordered_set>

#include "source_processor/node/util_node/CFGNode.h"

class AffectsCache {
 private:
  static std::unordered_set<std::string> ConvertToStrings(
      std::unordered_set<int> const& set);
  std::unordered_map<int, std::unordered_set<int>> affects_cache = {};
  std::unordered_map<int, std::unordered_set<int>> inv_affects_cache = {};
  std::unordered_map<int, std::unordered_set<int>> not_affects_cache = {};

  std::unordered_set<int> has_no_affects_path_from_cache = {};
  std::unordered_set<int> has_no_affects_path_to_cache = {};

  std::unordered_set<int> completed_forward_traversal = {};
  std::unordered_set<int> completed_backward_traversal = {};

 public:
  AffectsCache();
  ~AffectsCache() = default;

  void CacheAffects(std::shared_ptr<CFGNode> const& node1,
                    std::shared_ptr<CFGNode> const& node2);
  void CacheNotAffects(std::shared_ptr<CFGNode> const& node1,
                       std::shared_ptr<CFGNode> const& node2);
  bool IsCachedAndAffects(std::shared_ptr<CFGNode> const& node1,
                          std::shared_ptr<CFGNode> const& node2);
  bool IsCachedAndNotAffects(std::shared_ptr<CFGNode> const& node1,
                             std::shared_ptr<CFGNode> const& node2);

  void CacheHasNoAffectsFrom(std::shared_ptr<CFGNode> const& node);
  bool HasAnyAffectsPathFrom(std::shared_ptr<CFGNode> const& node);
  bool HasNoAffectsPathFrom(std::shared_ptr<CFGNode> const& node);

  void CacheHasNoAffectsTo(std::shared_ptr<CFGNode> const& node);
  bool HasAnyAffectsPathTo(std::shared_ptr<CFGNode> const& node);
  bool HasNoAffectsPathTo(std::shared_ptr<CFGNode> const& node);

  void InsertCompletedForwardTraversal(std::shared_ptr<CFGNode> const& node);
  void InsertCompletedBackwardTraversal(std::shared_ptr<CFGNode> const& node);
  bool CompletedForwardTraversal(std::shared_ptr<CFGNode> const& node);
  bool CompletedBackwardTraversal(std::shared_ptr<CFGNode> const& node);

  std::unordered_set<std::string> GetAllAffects(
      std::shared_ptr<CFGNode> const& node);
  std::unordered_set<std::string> GetAllInvAffects(
      std::shared_ptr<CFGNode> const& node);
};
