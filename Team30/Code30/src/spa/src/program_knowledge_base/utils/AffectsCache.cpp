#include "AffectsCache.h"

#include <memory>
#include <string>

#include "source_processor/node/util_node/CFGNode.h"

AffectsCache::AffectsCache() {}

void AffectsCache::CacheAffects(std::shared_ptr<CFGNode> const& node1,
                                std::shared_ptr<CFGNode> const& node2) {
  int start = node1->GetNodeStmtIndex();
  int end = node2->GetNodeStmtIndex();
  affects_cache[start].insert(end);
  inv_affects_cache[end].insert(start);
}

void AffectsCache::CacheNotAffects(std::shared_ptr<CFGNode> const& node1,
                                   std::shared_ptr<CFGNode> const& node2) {
  int start = node1->GetNodeStmtIndex();
  int end = node2->GetNodeStmtIndex();
  not_affects_cache[start].insert(end);
}

bool AffectsCache::IsCachedAndAffects(std::shared_ptr<CFGNode> const& node1,
                                      std::shared_ptr<CFGNode> const& node2) {
  int start = node1->GetNodeStmtIndex();
  int end = node2->GetNodeStmtIndex();
  return affects_cache.count(start) > 0 &&
         affects_cache.at(start).count(end) > 0;
}

bool AffectsCache::IsCachedAndNotAffects(
    std::shared_ptr<CFGNode> const& node1,
    std::shared_ptr<CFGNode> const& node2) {
  int start = node1->GetNodeStmtIndex();
  int end = node2->GetNodeStmtIndex();
  return not_affects_cache.count(start) > 0 &&
         not_affects_cache.at(start).count(end) > 0;
}

void AffectsCache::CacheHasNoAffectsFrom(std::shared_ptr<CFGNode> const& node) {
  has_no_affects_path_from_cache.insert(node->GetNodeStmtIndex());
}

bool AffectsCache::HasNoAffectsPathFrom(std::shared_ptr<CFGNode> const& node) {
  return has_no_affects_path_from_cache.count(node->GetNodeStmtIndex()) > 0;
}

bool AffectsCache::HasAnyAffectsPathFrom(std::shared_ptr<CFGNode> const& node) {
  return affects_cache.count(node->GetNodeStmtIndex()) > 0;
}

void AffectsCache::CacheHasNoAffectsTo(std::shared_ptr<CFGNode> const& node) {
  has_no_affects_path_to_cache.insert(node->GetNodeStmtIndex());
}

bool AffectsCache::HasNoAffectsPathTo(std::shared_ptr<CFGNode> const& node) {
  return has_no_affects_path_to_cache.count(node->GetNodeStmtIndex()) > 0;
}

bool AffectsCache::HasAnyAffectsPathTo(std::shared_ptr<CFGNode> const& node) {
  return inv_affects_cache.count(node->GetNodeStmtIndex()) > 0;
}

void AffectsCache::InsertCompletedForwardTraversal(
    std::shared_ptr<CFGNode> const& node) {
  completed_forward_traversal.insert(node->GetNodeStmtIndex());
}

void AffectsCache::InsertCompletedBackwardTraversal(
    std::shared_ptr<CFGNode> const& node) {
  completed_backward_traversal.insert(node->GetNodeStmtIndex());
}

bool AffectsCache::CompletedForwardTraversal(
    std::shared_ptr<CFGNode> const& node) {
  return completed_forward_traversal.count(node->GetNodeStmtIndex()) > 0;
}

bool AffectsCache::CompletedBackwardTraversal(
    std::shared_ptr<CFGNode> const& node) {
  return completed_backward_traversal.count(node->GetNodeStmtIndex()) > 0;
}

std::unordered_set<std::string> AffectsCache::GetAllAffects(
    std::shared_ptr<CFGNode> const& node) {
  int line = node->GetNodeStmtIndex();
  if (affects_cache.count(line) == 0) {
    return std::unordered_set<std::string>();
  }
  return ConvertToStrings(affects_cache.at(line));
}

std::unordered_set<std::string> AffectsCache::GetAllInvAffects(
    std::shared_ptr<CFGNode> const& node) {
  int line = node->GetNodeStmtIndex();
  if (inv_affects_cache.count(line) == 0) {
    return std::unordered_set<std::string>();
  }
  return ConvertToStrings(inv_affects_cache.at(line));
}

std::unordered_set<std::string> AffectsCache::ConvertToStrings(
    std::unordered_set<int> const& set) {
  std::unordered_set<std::string> res;
  for (auto i : set) {
    res.insert(std::to_string(i));
  }
  return res;
}
