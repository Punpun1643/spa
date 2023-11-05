#pragma once

#include <memory>
#include <string>
#include <unordered_set>

#include "../node/util_node/CFGNode.h"

class GraphRelationTraverser {
 public:
  static bool HasImmediatePath(std::shared_ptr<CFGNode> start_node,
                               std::shared_ptr<CFGNode> end_node);

  static bool HasPath(std::shared_ptr<CFGNode> start_node,
                      std::shared_ptr<CFGNode> end_node);

  static bool HasAffectsPath(std::shared_ptr<CFGNode> start_node,
                             std::shared_ptr<CFGNode> end_node);

  static bool HandleAssignOrReadOutgoingNode(
      std::shared_ptr<CFGNode> outgoing_node,
      std::string const& var_modified_in_start_node);

  static bool HandleCallOutgoingNode(
      std::shared_ptr<CFGNode> outgoing_node,
      std::string const& var_modified_in_start_node);

  static bool ValidateStartAndEndNodes(std::shared_ptr<CFGNode> start_node,
                                       std::shared_ptr<CFGNode> end_node);

  static bool ValidatePossibleAffectsRelationship(
      std::string var_modified_in_start_node,
      std::unordered_set<std::string> var_used_in_end_node);

  static std::unordered_set<std::string> GetAllStmtsWithPathFrom(
      std::shared_ptr<CFGNode> start_node);

  static std::unordered_set<std::string> GetAllStmtsWithPathTo(
      std::shared_ptr<CFGNode> end_node);

  static std::unordered_set<std::string> GetAllStmtsWithAffectsPathFrom(
      std::shared_ptr<CFGNode> start_node);
};
