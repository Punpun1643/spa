#include "GraphRelationTraverser.h"

#include <iostream>
#include <queue>
#include <stack>
#include <utility>
#include <vector>

#include "program_knowledge_base/utils/PairHashValueSet.h"

bool GraphRelationTraverser::ShouldVisit(
    std::shared_ptr<CFGNode> const& node,
    std ::string var_modified_in_start_node) {
  bool should_visit = true;
  if (CFGNode::IsAssignOrReadNode(node)) {
    should_visit =
        HandleAssignOrReadOutgoingNode(node, var_modified_in_start_node);
  } else if (CFGNode::IsCallNode(node)) {
    should_visit = HandleCallOutgoingNode(node, var_modified_in_start_node);
  }
  return should_visit;
}

bool GraphRelationTraverser::ValidateStartAndEndNodes(
    std::shared_ptr<CFGNode> const& start_node,
    std::shared_ptr<CFGNode> const& end_node) {
  return CFGNode::IsAssignNode(start_node) && CFGNode::IsAssignNode(end_node);
}

bool GraphRelationTraverser::ValidatePossibleAffectsRelationship(
    std::string var_modified_in_start_node,
    std::unordered_set<std::string> const& var_used_in_end_node) {
  return var_used_in_end_node.find(var_modified_in_start_node) !=
         var_used_in_end_node.end();
}

bool GraphRelationTraverser::HandleAssignOrReadOutgoingNode(
    std::shared_ptr<CFGNode> const& outgoing_node,
    std::string const& var_modified_in_start_node) {
  std::unordered_set<std::string> vars_modified_in_outgoing_node =
      outgoing_node->GetModifiesVars();
  std::string var_modified_in_outgoing_node =
      *vars_modified_in_outgoing_node.begin();

  return var_modified_in_start_node != var_modified_in_outgoing_node;
}

bool GraphRelationTraverser::HandleCallOutgoingNode(
    std::shared_ptr<CFGNode> const& outgoing_node,
    std::string const& var_modified_in_start_node) {
  std::unordered_set<std::string> vars_modified_in_call_outgoing_node =
      outgoing_node->GetModifiesVars();

  return vars_modified_in_call_outgoing_node.find(var_modified_in_start_node) ==
         vars_modified_in_call_outgoing_node.end();
}

bool GraphRelationTraverser::HasImmediatePath(
    std::shared_ptr<CFGNode> const& start_node,
    std::shared_ptr<CFGNode> const& end_node) {
  std::vector<std::shared_ptr<CFGNode>> outgoing_nodes =
      start_node->GetOutgoingNodes();

  for (std::shared_ptr<CFGNode> outgoingNode : outgoing_nodes) {
    if (outgoingNode == end_node) {
      return true;
    }
  }
  return false;
}

bool GraphRelationTraverser::HasPath(std::shared_ptr<CFGNode> const& start_node,
                                     std::shared_ptr<CFGNode> const& end_node) {
  std::unordered_set<std::shared_ptr<CFGNode>> visited_nodes;
  std::queue<std::shared_ptr<CFGNode>> nodes_to_visit;

  nodes_to_visit.push(start_node);

  while (!nodes_to_visit.empty()) {
    std::shared_ptr<CFGNode> curr_node = nodes_to_visit.front();
    nodes_to_visit.pop();
    visited_nodes.insert(curr_node);

    if (curr_node == end_node && curr_node != start_node) {
      return true;
    }

    std::vector<std::shared_ptr<CFGNode>> outgoing_nodes =
        curr_node->GetOutgoingNodes();

    for (std::shared_ptr<CFGNode> out_going_node : outgoing_nodes) {
      if (visited_nodes.count(out_going_node) == 0) {
        nodes_to_visit.push(out_going_node);
      } else if (out_going_node == end_node &&
                 visited_nodes.count(out_going_node) > 0) {
        return true;
      }
    }
  }
  return false;
}

std::unordered_set<std::string> GraphRelationTraverser::GetAllStmtsWithPathFrom(
    std::shared_ptr<CFGNode> const& start_node) {
  std::unordered_set<std::shared_ptr<CFGNode>> visited_nodes;
  std::queue<std::shared_ptr<CFGNode>> nodes_to_visit;
  std::unordered_set<std::string> stmts_with_valid_path;

  nodes_to_visit.push(start_node);

  while (!nodes_to_visit.empty()) {
    std::shared_ptr<CFGNode> curr_node = nodes_to_visit.front();
    nodes_to_visit.pop();

    std::vector<std::shared_ptr<CFGNode>> outgoing_nodes =
        curr_node->GetOutgoingNodes();

    for (std::shared_ptr<CFGNode> out_going_node : outgoing_nodes) {
      if (!visited_nodes.count(out_going_node)) {
        nodes_to_visit.push(out_going_node);
        visited_nodes.insert(out_going_node);
        stmts_with_valid_path.insert(
            std::to_string(out_going_node->GetNodeStmtIndex()));
      }
    }
  }
  return stmts_with_valid_path;
}

std::unordered_set<std::string> GraphRelationTraverser::GetAllStmtsWithPathTo(
    std::shared_ptr<CFGNode> const& end_node) {
  std::unordered_set<std::shared_ptr<CFGNode>> visited_nodes;
  std::queue<std::shared_ptr<CFGNode>> nodes_to_visit;
  std::unordered_set<std::string> stmts_with_valid_path;

  nodes_to_visit.push(end_node);

  while (!nodes_to_visit.empty()) {
    std::shared_ptr<CFGNode> curr_node = nodes_to_visit.front();
    nodes_to_visit.pop();

    std::vector<std::shared_ptr<CFGNode>> incoming_nodes =
        curr_node->GetIncomingNodes();

    for (std::shared_ptr<CFGNode> in_coming_node : incoming_nodes) {
      if (!visited_nodes.count(in_coming_node)) {
        nodes_to_visit.push(in_coming_node);
        visited_nodes.insert(in_coming_node);
        stmts_with_valid_path.insert(
            std::to_string(in_coming_node->GetNodeStmtIndex()));
      }
    }
  }
  return stmts_with_valid_path;
}

bool GraphRelationTraverser::IsValidPath(
    std::shared_ptr<CFGNode> const& start_node,
    std::shared_ptr<CFGNode> const& end_node) {
  return ValidateStartAndEndNodes(start_node, end_node) &&
         ValidatePossibleAffectsRelationship(
             CFGNode::GetVarModifiedInStartNode(start_node),
             CFGNode::GetVarUsedInEndNode(end_node));
}

void GraphRelationTraverser::HandleVisitOutgoingNode(
    std::shared_ptr<CFGNode> const& start_node,
    std::shared_ptr<CFGNode> const& outgoing_node,
    std::queue<std::shared_ptr<CFGNode>>& nodes_to_visit) {
  bool should_visit = ShouldVisit(
      outgoing_node, CFGNode::GetVarModifiedInStartNode(start_node));
  if (should_visit) {
    nodes_to_visit.push(outgoing_node);
  }
}

void GraphRelationTraverser::HandleCacheOutgoingNode(
    std::shared_ptr<CFGNode> const& start_node,
    std::shared_ptr<CFGNode> const& outgoing_node,
    std::string const& var_modified_in_start,
    std::shared_ptr<AffectsCache> cache) {
  if (CFGNode::IsAssignNode(outgoing_node)) {
    if (outgoing_node->GetUsesVars().count(var_modified_in_start)) {
      cache->CacheAffects(start_node, outgoing_node);
    } else {
      cache->CacheNotAffects(start_node, outgoing_node);
    }
  }
}

bool GraphRelationTraverser::PerformNodeTraversal(
    std::shared_ptr<CFGNode> const& start_node,
    std::shared_ptr<CFGNode> const& end_node,
    std::shared_ptr<AffectsCache> cache) {
  std::queue<std::shared_ptr<CFGNode>> nodes_to_visit;
  std::unordered_set<std::shared_ptr<CFGNode>> visited_nodes;
  std::string var_modified_in_start = *start_node->GetModifiesVars().begin();
  nodes_to_visit.push(start_node);
  while (!nodes_to_visit.empty()) {
    std::shared_ptr<CFGNode> curr_node = nodes_to_visit.front();
    nodes_to_visit.pop();
    for (std::shared_ptr<CFGNode> outgoing_node :
         curr_node->GetOutgoingNodes()) {
      if (outgoing_node == end_node) {
        cache->CacheAffects(start_node, end_node);
        return true;
      }
      if (!visited_nodes.count(outgoing_node)) {
        visited_nodes.insert(outgoing_node);
        HandleCacheOutgoingNode(start_node, outgoing_node,
                                var_modified_in_start, cache);
        HandleVisitOutgoingNode(start_node, outgoing_node, nodes_to_visit);
      }
    }
  }
  cache->CacheNotAffects(start_node, end_node);
  return false;
}

bool GraphRelationTraverser::TraverseAndEvaluatePath(
    std::shared_ptr<CFGNode> const& start_node,
    std::shared_ptr<CFGNode> const& end_node,
    std::shared_ptr<AffectsCache> cache) {
  if (cache->IsCachedAndAffects(start_node, end_node)) {
    return true;
  }
  if (cache->IsCachedAndNotAffects(start_node, end_node)) {
    return false;
  }
  return PerformNodeTraversal(start_node, end_node, cache);
}

bool GraphRelationTraverser::HasAffectsPath(
    std::shared_ptr<CFGNode> const& start_node,
    std::shared_ptr<CFGNode> const& end_node,
    std::shared_ptr<AffectsCache> cache) {
  if (!IsValidPath(start_node, end_node)) {
    return false;
  }

  return TraverseAndEvaluatePath(start_node, end_node, cache);
}

bool GraphRelationTraverser::AffectsAnyOutgoingNode(
    std::shared_ptr<CFGNode> const& start_node,
    std::shared_ptr<CFGNode> const& outgoing_node,
    std::string const& var_modified_in_start_node,
    std::shared_ptr<AffectsCache> cache) {
  if (ValidatePossibleAffectsRelationship(
          var_modified_in_start_node,
          CFGNode::GetVarUsedInEndNode(outgoing_node)) &&
      CFGNode::IsAssignNode(outgoing_node)) {
    cache->CacheAffects(start_node, outgoing_node);
    return true;
  }
  return false;
}

bool GraphRelationTraverser::PerformAffectsFromTraversal(
    std::shared_ptr<CFGNode> const& start_node,
    std::shared_ptr<AffectsCache> cache) {
  std::queue<std::shared_ptr<CFGNode>> nodes_to_visit;
  std::unordered_set<std::shared_ptr<CFGNode>> visited_nodes;
  std::string var_modified_in_start_node =
      CFGNode::GetVarModifiedInStartNode(start_node);

  nodes_to_visit.push(start_node);
  while (!nodes_to_visit.empty()) {
    std::shared_ptr<CFGNode> curr_node = nodes_to_visit.front();
    nodes_to_visit.pop();
    for (std::shared_ptr<CFGNode> outgoing_node :
         curr_node->GetOutgoingNodes()) {
      if (!visited_nodes.count(outgoing_node)) {
        visited_nodes.insert(outgoing_node);
        // if start node affects any outgoing node, return true
        if (AffectsAnyOutgoingNode(start_node, outgoing_node,
                                   var_modified_in_start_node, cache)) {
          return true;
        }
        if (ShouldVisit(outgoing_node, var_modified_in_start_node)) {
          nodes_to_visit.push(outgoing_node);
        }
      }
    }
  }
  cache->CacheHasNoAffectsFrom(start_node);
  return false;
}

bool GraphRelationTraverser::HasAnyAffectsPathFrom(
    std::shared_ptr<CFGNode> const& start_node,
    std::shared_ptr<AffectsCache> cache) {
  if (!CFGNode::IsAssignNode(start_node) ||
      cache->HasNoAffectsPathFrom(start_node)) {
    return false;
  } else if (cache->HasAnyAffectsPathFrom(start_node)) {
    return true;
  }
  return PerformAffectsFromTraversal(start_node, cache);
}

bool GraphRelationTraverser::IsValidAffectsRelation(
    std::shared_ptr<CFGNode> const& curr_node,
    std::shared_ptr<CFGNode> const& end_node,
    std::unordered_set<std::string> const& unmodified_vars_used_in_end_node,
    std::shared_ptr<AffectsCache> cache) {
  if (CFGNode::IsAssignNode(curr_node) &&
      ValidatePossibleAffectsRelationship(
          CFGNode::GetVarModifiedInStartNode(curr_node),
          unmodified_vars_used_in_end_node)) {
    cache->CacheAffects(curr_node, end_node);
    return true;
  }
  return false;
}

void GraphRelationTraverser::InitializeTraversal(
    AffectsTraversalContext& context,
    std::unordered_set<std::string> const& unmodified_vars_used_in_end_node) {
  for (auto& incoming_node : context.end_node->GetIncomingNodes()) {
    context.nodes_to_visit.push(incoming_node);
    context.vars_used_in_nodes.push(unmodified_vars_used_in_end_node);
    context.visited.insert(make_pair(incoming_node->GetNodeStmtIndex(),
                                     unmodified_vars_used_in_end_node));
  }
}

void GraphRelationTraverser::UpdateUnmodifiedVars(
    std::shared_ptr<CFGNode> const& curr_node,
    std::unordered_set<std::string>& unmodified_vars_used_in_end_node) {
  if (CFGNode::IsAssignOrReadOrCallNode(curr_node)) {
    for (auto v : curr_node->GetModifiesVars()) {
      unmodified_vars_used_in_end_node.erase(v);
    }
  }
}

void GraphRelationTraverser::AddIncomingNodesToTraversal(
    std::shared_ptr<CFGNode> const& curr_node,
    std::unordered_set<std::string> const& unmodified_vars_used_in_end_node,
    AffectsTraversalContext& context) {
  for (auto& incoming_node : curr_node->GetIncomingNodes()) {
    if (!context.visited.count(make_pair(incoming_node->GetNodeStmtIndex(),
                                         unmodified_vars_used_in_end_node))) {
      context.nodes_to_visit.push(incoming_node);
      context.vars_used_in_nodes.push(unmodified_vars_used_in_end_node);
      context.visited.insert(make_pair(incoming_node->GetNodeStmtIndex(),
                                       unmodified_vars_used_in_end_node));
    }
  }
}

bool GraphRelationTraverser::ProcessCurrNode(AffectsTraversalContext& context) {
  auto curr_node = context.nodes_to_visit.top();
  auto unmodified_vars_used_in_end_node = context.vars_used_in_nodes.top();

  context.nodes_to_visit.pop();
  context.vars_used_in_nodes.pop();

  if (IsValidAffectsRelation(curr_node, context.end_node,
                             unmodified_vars_used_in_end_node, context.cache)) {
    return true;
  }

  UpdateUnmodifiedVars(curr_node, unmodified_vars_used_in_end_node);
  AddIncomingNodesToTraversal(curr_node, unmodified_vars_used_in_end_node,
                              context);

  return false;
}

bool GraphRelationTraverser::PerformAffectsToTraversal(
    std::shared_ptr<CFGNode> const& end_node,
    std::shared_ptr<AffectsCache> cache) {
  AffectsTraversalContext context;
  context.end_node = end_node;
  context.cache = cache;

  InitializeTraversal(context, CFGNode::GetVarUsedInEndNode(end_node));

  while (!context.nodes_to_visit.empty()) {
    if (ProcessCurrNode(context)) {
      return true;
    }
  }
  cache->CacheHasNoAffectsTo(end_node);
  return false;
}

bool GraphRelationTraverser::HasAnyAffectsPathTo(
    std::shared_ptr<CFGNode> const& end_node,
    std::shared_ptr<AffectsCache> cache) {
  if (!CFGNode::IsAssignNode(end_node) || cache->HasNoAffectsPathTo(end_node)) {
    return false;
  } else if (cache->HasAnyAffectsPathTo(end_node)) {
    return true;
  }
  return PerformAffectsToTraversal(end_node, cache);
}

std::unordered_set<std::string>
GraphRelationTraverser::GetAllStmtsWithAffectsPathFrom(
    std::shared_ptr<CFGNode> const& start_node,
    std::shared_ptr<AffectsCache> cache) {
  if (!CFGNode::IsAssignNode(start_node)) {
    return std::unordered_set<std::string>();
  }
  if (cache->CompletedForwardTraversal(start_node)) {
    return cache->GetAllAffects(start_node);
  }
  AffectsFromTraversalContext context{
      std::queue<std::shared_ptr<CFGNode>>(),
      std::unordered_set<std::shared_ptr<CFGNode>>(), start_node, cache};
  return PerformForwardTraversal(context);
}

std::unordered_set<std::string> GraphRelationTraverser::PerformForwardTraversal(
    AffectsFromTraversalContext& context) {
  std::unordered_set<std::string> stmts_with_valid_path;
  std::string var_modified_in_start_node =
      CFGNode::GetVarModifiedInStartNode(context.start_node);
  context.nodes_to_visit.push(context.start_node);

  while (!context.nodes_to_visit.empty()) {
    auto curr_node = context.nodes_to_visit.front();
    context.nodes_to_visit.pop();
    ProcessOutgoingNode(curr_node, var_modified_in_start_node,
                        stmts_with_valid_path, context);
  }

  context.cache->InsertCompletedForwardTraversal(context.start_node);
  return stmts_with_valid_path;
}

void GraphRelationTraverser::ProcessOutgoingNode(
    std::shared_ptr<CFGNode> curr_node,
    std::string const& var_modified_in_start_node,
    std::unordered_set<std::string>& stmts_with_valid_path,
    AffectsFromTraversalContext& context) {
  for (auto outgoing_node : curr_node->GetOutgoingNodes()) {
    if (!context.visited_nodes.count(outgoing_node)) {
      context.visited_nodes.insert(outgoing_node);
      if (ValidatePossibleAffectsRelationship(
              var_modified_in_start_node,
              CFGNode::GetVarUsedInEndNode(outgoing_node)) &&
          CFGNode::IsAssignNode(outgoing_node)) {
        stmts_with_valid_path.insert(
            std::to_string(outgoing_node->GetNodeStmtIndex()));
        context.cache->CacheAffects(context.start_node, outgoing_node);
      }
      HandleVisitOutgoingNode(context.start_node, outgoing_node,
                              context.nodes_to_visit);
    }
  }
}

void GraphRelationTraverser::InitializeBackwardTraversal(
    AffectsToTraversalContext& context) {
  std::unordered_set<std::string> unmodified_vars_used_in_end_node =
      CFGNode::GetVarUsedInEndNode(context.end_node);
  for (auto incoming_node : context.end_node->GetIncomingNodes()) {
    context.nodes_to_visit.push(incoming_node);
    context.vars_used_in_nodes.push(unmodified_vars_used_in_end_node);
    context.visited.insert(make_pair(incoming_node->GetNodeStmtIndex(),
                                     unmodified_vars_used_in_end_node));
  }
}

void GraphRelationTraverser::UpdateForNextNodesTo(
    std::shared_ptr<CFGNode> curr_node,
    std::unordered_set<std::string>& unmodified_vars_used_in_end_node,
    AffectsToTraversalContext& context) {
  if (CFGNode::IsAssignOrReadOrCallNode(curr_node)) {
    for (auto const& v : curr_node->GetModifiesVars()) {
      unmodified_vars_used_in_end_node.erase(v);
    }
  }
  // Explore the incoming nodes of the current node.
  for (std::shared_ptr<CFGNode> incoming_node : curr_node->GetIncomingNodes()) {
    if (!context.visited.count(make_pair(incoming_node->GetNodeStmtIndex(),
                                         unmodified_vars_used_in_end_node))) {
      context.nodes_to_visit.push(incoming_node);
      context.vars_used_in_nodes.push(unmodified_vars_used_in_end_node);
      context.visited.insert(make_pair(incoming_node->GetNodeStmtIndex(),
                                       unmodified_vars_used_in_end_node));
    }
  }
}

bool GraphRelationTraverser::ShouldCacheAffectsTo(
    std::shared_ptr<CFGNode> curr_node,
    std::unordered_set<std::string> const& unmodified_vars_used_in_end_node,
    AffectsToTraversalContext& context) {
  return CFGNode::IsAssignNode(curr_node) &&
         ValidatePossibleAffectsRelationship(
             CFGNode::GetVarModifiedInStartNode(curr_node),
             unmodified_vars_used_in_end_node);
}

void GraphRelationTraverser::ProcessCurrNodeTo(
    AffectsToTraversalContext& context) {
  auto curr_node = context.nodes_to_visit.top();
  auto unmodified_vars_used_in_end_node = context.vars_used_in_nodes.top();
  context.nodes_to_visit.pop();
  context.vars_used_in_nodes.pop();

  if (ShouldCacheAffectsTo(curr_node, unmodified_vars_used_in_end_node,
                           context)) {
    context.cache->CacheAffects(curr_node, context.end_node);
    context.stmts_with_valid_path.insert(
        std::to_string(curr_node->GetNodeStmtIndex()));
  }

  UpdateForNextNodesTo(curr_node, unmodified_vars_used_in_end_node, context);
}

std::unordered_set<std::string>
GraphRelationTraverser::GetAllStmtsWithAffectsPathTo(
    std::shared_ptr<CFGNode> const& end_node,
    std::shared_ptr<AffectsCache> cache) {
  if (!CFGNode::IsAssignNode(end_node)) {
    return std::unordered_set<std::string>();
  }

  if (cache->CompletedBackwardTraversal(end_node)) {
    return cache->GetAllInvAffects(end_node);
  }

  AffectsToTraversalContext context{
      std::stack<std::shared_ptr<CFGNode>>{},
      std::stack<std::unordered_set<std::string>>{},
      std::unordered_set<std::pair<int, std::unordered_set<std::string>>,
                         PairHashValueSet>{},
      end_node,
      cache,
      std::unordered_set<std::string>{}};
  InitializeBackwardTraversal(context);

  while (!context.nodes_to_visit.empty()) {
    ProcessCurrNodeTo(context);
  }

  cache->InsertCompletedBackwardTraversal(end_node);
  return context.stmts_with_valid_path;
}
