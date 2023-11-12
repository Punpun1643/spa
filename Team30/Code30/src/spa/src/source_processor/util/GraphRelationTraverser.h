#pragma once

#include <memory>
#include <queue>
#include <stack>
#include <string>
#include <unordered_map>
#include <unordered_set>
#include <vector>

#include "../../program_knowledge_base/utils/AffectsCache.h"
#include "../node/util_node/CFGNode.h"
#include "program_knowledge_base/utils/PairHashValueSet.h"

class GraphRelationTraverser {
 private:
  // Helper functions
  static bool ShouldVisit(std::shared_ptr<CFGNode> const& node,
                          std ::string var_modified_in_start_node);
  static bool HandleAssignOrReadOutgoingNode(
      std::shared_ptr<CFGNode> const& outgoing_node,
      std::string const& var_modified_in_start_node);
  static bool HandleCallOutgoingNode(
      std::shared_ptr<CFGNode> const& outgoing_node,
      std::string const& var_modified_in_start_node);
  static bool ValidateStartAndEndNodes(
      std::shared_ptr<CFGNode> const& start_node,
      std::shared_ptr<CFGNode> const& end_node);
  static bool ValidatePossibleAffectsRelationship(
      std::string var_modified_in_start_node,
      std::unordered_set<std::string> const& var_used_in_end_node);

 public:
  struct AffectsTraversalContext {
    std::stack<std::shared_ptr<CFGNode>> nodes_to_visit;
    std::stack<std::unordered_set<std::string>> vars_used_in_nodes;
    std::unordered_set<std::pair<int, std::unordered_set<std::string>>,
                       PairHashValueSet>
        visited;
    std::shared_ptr<AffectsCache> cache;
    std::shared_ptr<CFGNode> end_node;
  };
  // Next
  static bool HasImmediatePath(std::shared_ptr<CFGNode> const& start_node,
                               std::shared_ptr<CFGNode> const& end_node);

  // Next*
  static bool HasPath(std::shared_ptr<CFGNode> const& start_node,
                      std::shared_ptr<CFGNode> const& end_node);
  static std::unordered_set<std::string> GetAllStmtsWithPathFrom(
      std::shared_ptr<CFGNode> const& start_node);
  static std::unordered_set<std::string> GetAllStmtsWithPathTo(
      std::shared_ptr<CFGNode> const& end_node);

  // Affects
  static bool HasAffectsPath(std::shared_ptr<CFGNode> const& start_node,
                             std::shared_ptr<CFGNode> const& end_node,
                             std::shared_ptr<AffectsCache> cache);
  static bool HasAnyAffectsPathFrom(std::shared_ptr<CFGNode> const& start_node,
                                    std::shared_ptr<AffectsCache> cache);
  static bool HasAnyAffectsPathTo(std::shared_ptr<CFGNode> const& end_node,
                                  std::shared_ptr<AffectsCache> cache);
  static std::unordered_set<std::string> GetAllStmtsWithAffectsPathFrom(
      std::shared_ptr<CFGNode> const& start_node,
      std::shared_ptr<AffectsCache> cache);
  static std::unordered_set<std::string> GetAllStmtsWithAffectsPathTo(
      std::shared_ptr<CFGNode> const& end_node,
      std::shared_ptr<AffectsCache> cache);

  static bool IsValidPath(std::shared_ptr<CFGNode> const& start_node,
                          std::shared_ptr<CFGNode> const& end_node);

  static bool CheckCacheForAffectsPath(
      std::shared_ptr<CFGNode> const& start_node,
      std::shared_ptr<CFGNode> const& end_node,
      std::shared_ptr<AffectsCache> cache);

  static bool CheckCacheForAffectsPathFrom(
      std::shared_ptr<CFGNode> const& start_node,
      std::shared_ptr<AffectsCache> cache);

  static bool CheckCacheForAffectsPathTo(
      std::shared_ptr<CFGNode> const& end_node,
      std::shared_ptr<AffectsCache> cache);

  static bool TraverseAndEvaluatePath(
      std::shared_ptr<CFGNode> const& start_node,
      std::shared_ptr<CFGNode> const& end_node,
      std::shared_ptr<AffectsCache> cache);

  static bool PerformNodeTraversal(std::shared_ptr<CFGNode> const& start_node,
                                   std::shared_ptr<CFGNode> const& end_node,
                                   std::shared_ptr<AffectsCache> cache);

  static bool PerformAffectsFromTraversal(
      std::shared_ptr<CFGNode> const& start_node,
      std::shared_ptr<AffectsCache> cache);

  static bool PerformAffectsToTraversal(
      std::shared_ptr<CFGNode> const& start_node,
      std::shared_ptr<AffectsCache> cache);

  static void HandleVisitOutgoingNode(
      std::shared_ptr<CFGNode> const& start_node,
      std::shared_ptr<CFGNode> const& outgoing_node,
      std::queue<std::shared_ptr<CFGNode>>& nodes_to_visit);

  static bool AffectsAnyOutgoingNode(
      std::shared_ptr<CFGNode> const& start_node,
      std::shared_ptr<CFGNode> const& outgoing_node,
      std::string const& var_modified_in_start_node,
      std::shared_ptr<AffectsCache> cache);

  static void HandleCacheOutgoingNode(
      std::shared_ptr<CFGNode> const& start_node,
      std::shared_ptr<CFGNode> const& outgoing_node,
      std::string const& var_modified_in_start,
      std::shared_ptr<AffectsCache> cache);

  static bool IsValidAffectsRelation(
      std::shared_ptr<CFGNode> const& curr_node,
      std::shared_ptr<CFGNode> const& end_node,
      std::unordered_set<std::string> const& unmodified_vars_used_in_end_node,
      std::shared_ptr<AffectsCache> cache);

  static void InitializeTraversal(
      AffectsTraversalContext& context,
      std::unordered_set<std::string> const& unmodified_vars_used_in_end_node);

  static bool ProcessCurrNode(AffectsTraversalContext& context);

  static void UpdateUnmodifiedVars(
      std::shared_ptr<CFGNode> const& curr_node,
      std::unordered_set<std::string>& unmodified_vars_used_in_end_node);

  static void AddIncomingNodesToTraversal(
      std::shared_ptr<CFGNode> const& curr_node,
      std::unordered_set<std::string> const& unmodified_vars_used_in_end_node,
      AffectsTraversalContext& context);
};
