#include "GraphRelationTraverser.h"

#include <queue>
#include <stack>
#include <utility>
#include <vector>

#include "program_knowledge_base/utils/PairHashValueSet.h"

bool GraphRelationTraverser::ShouldVisit(
    std::shared_ptr<CFGNode> const& node,
    std ::string var_modified_in_start_node) {
  bool should_visit = true;
  if (CFGNode::IsAssignOrReadOutgoingNode(node)) {
    should_visit =
        HandleAssignOrReadOutgoingNode(node, var_modified_in_start_node);
  } else if (CFGNode::IsCallOutgoingNode(node)) {
    should_visit = HandleCallOutgoingNode(node, var_modified_in_start_node);
  }
  return should_visit;
}

bool GraphRelationTraverser::ValidateStartAndEndNodes(
    std::shared_ptr<CFGNode> const& start_node,
    std::shared_ptr<CFGNode> const& end_node) {
  return start_node->GetStmtType() == StmtType::ASSIGN_STMT &&
         end_node->GetStmtType() == StmtType::ASSIGN_STMT;
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
      if (visited_nodes.find(out_going_node) == visited_nodes.end()) {
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
      if (visited_nodes.find(in_coming_node) == visited_nodes.end()) {
        nodes_to_visit.push(in_coming_node);
        visited_nodes.insert(in_coming_node);
        stmts_with_valid_path.insert(
            std::to_string(in_coming_node->GetNodeStmtIndex()));
      }
    }
  }
  return stmts_with_valid_path;
}

bool GraphRelationTraverser::HasAffectsPath(
    std::shared_ptr<CFGNode> const& start_node,
    std::shared_ptr<CFGNode> const& end_node,
    std::shared_ptr<AffectsCache> cache) {
  if (!ValidateStartAndEndNodes(start_node, end_node)) {
    return false;
  }
  if (!ValidatePossibleAffectsRelationship(
          CFGNode::GetVarModifiedInStartNode(start_node),
          CFGNode::GetVarUsedInEndNode(end_node))) {
    return false;
  }

  if (cache->IsCachedAndAffects(start_node, end_node)) {
    return true;
  } else if (cache->IsCachedAndNotAffects(start_node, end_node)) {
    return false;
  }

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

        if (outgoing_node->GetNodeType() == StmtType::ASSIGN_STMT) {
          if (outgoing_node->GetUsesVars().count(var_modified_in_start)) {
            cache->CacheAffects(start_node, outgoing_node);
          } else {
            cache->CacheNotAffects(start_node, outgoing_node);
          }
        }

        bool should_visit = true;
        if (CFGNode::IsAssignOrReadOutgoingNode(outgoing_node)) {
          should_visit = HandleAssignOrReadOutgoingNode(
              outgoing_node, CFGNode::GetVarModifiedInStartNode(start_node));
        } else if (CFGNode::IsCallOutgoingNode(outgoing_node)) {
          should_visit = HandleCallOutgoingNode(
              outgoing_node, CFGNode::GetVarModifiedInStartNode(start_node));
        }

        if (should_visit) {
          nodes_to_visit.push(outgoing_node);
        }
      }
    }
  }
  cache->CacheNotAffects(start_node, end_node);
  return false;
}

bool GraphRelationTraverser::HasAnyAffectsPathFrom(
    std::shared_ptr<CFGNode> const& start_node,
    std::shared_ptr<AffectsCache> cache) {
  if (start_node->GetNodeType() != StmtType::ASSIGN_STMT) {
    return false;
  }

  if (cache->HasAnyAffectsPathFrom(start_node)) {
    return true;
  } else if (cache->HasNoAffectsPathFrom(start_node)) {
    return false;
  }

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
        if (ValidatePossibleAffectsRelationship(
                var_modified_in_start_node,
                CFGNode::GetVarUsedInEndNode(outgoing_node)) &&
            outgoing_node->GetNodeType() == StmtType::ASSIGN_STMT) {
          cache->CacheAffects(start_node, outgoing_node);
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

bool GraphRelationTraverser::HasAnyAffectsPathTo(
    std::shared_ptr<CFGNode> const& end_node,
    std::shared_ptr<AffectsCache> cache) {
  if (end_node->GetNodeType() != StmtType::ASSIGN_STMT) {
    return false;
  }

  if (cache->HasAnyAffectsPathTo(end_node)) {
    return true;
  } else if (cache->HasNoAffectsPathTo(end_node)) {
    return false;
  }

  std::stack<std::shared_ptr<CFGNode>> nodes_to_visit;
  std::unordered_set<std::pair<int, std::unordered_set<std::string>>,
                     PairHashValueSet>
      visited;
  std::stack<std::unordered_set<std::string>> vars_used_in_nodes;

  std::unordered_set<std::string> unmodified_vars_used_in_end_node =
      CFGNode::GetVarUsedInEndNode(end_node);
  for (std::shared_ptr<CFGNode> incoming_node : end_node->GetIncomingNodes()) {
    nodes_to_visit.push(incoming_node);
    vars_used_in_nodes.push(unmodified_vars_used_in_end_node);
    visited.insert(make_pair(incoming_node->GetNodeStmtIndex(),
                             unmodified_vars_used_in_end_node));
  }

  while (!nodes_to_visit.empty()) {
    std::shared_ptr<CFGNode> curr_node = nodes_to_visit.top();
    std::unordered_set<std::string> unmodified_vars_used_in_end_node =
        vars_used_in_nodes.top();
    nodes_to_visit.pop();
    vars_used_in_nodes.pop();

    // return true if has valid affects relation
    if (curr_node->GetNodeType() == StmtType::ASSIGN_STMT &&
        ValidatePossibleAffectsRelationship(
            CFGNode::GetVarModifiedInStartNode(curr_node),
            unmodified_vars_used_in_end_node)) {
      cache->CacheAffects(curr_node, end_node);
      return true;
    }

    // if incoming node changes a variable used by the end node, remove it from
    // unmodifed vars
    if (curr_node->GetNodeType() == StmtType::CALL_STMT ||
        curr_node->GetNodeType() == StmtType::READ_STMT ||
        curr_node->GetNodeType() == StmtType::ASSIGN_STMT) {
      for (auto v : curr_node->GetModifiesVars()) {
        unmodified_vars_used_in_end_node.erase(v);
      }
    }

    for (std::shared_ptr<CFGNode> incoming_node :
         curr_node->GetIncomingNodes()) {
      if (!visited.count(make_pair(incoming_node->GetNodeStmtIndex(),
                                   unmodified_vars_used_in_end_node))) {
        nodes_to_visit.push(incoming_node);
        vars_used_in_nodes.push(unmodified_vars_used_in_end_node);
        visited.insert(make_pair(incoming_node->GetNodeStmtIndex(),
                                 unmodified_vars_used_in_end_node));
      }
    }
  }
  cache->CacheHasNoAffectsTo(end_node);
  return false;
}

std::unordered_set<std::string>
GraphRelationTraverser::GetAllStmtsWithAffectsPathFrom(
    std::shared_ptr<CFGNode> const& start_node,
    std::shared_ptr<AffectsCache> cache) {
  if (start_node->GetNodeType() != StmtType::ASSIGN_STMT) {
    return std::unordered_set<std::string>();
  }

  if (cache->CompletedForwardTraversal(start_node)) {
    return cache->GetAllAffects(start_node);
  }

  std::queue<std::shared_ptr<CFGNode>> nodes_to_visit;
  std::unordered_set<std::shared_ptr<CFGNode>> visited_nodes;
  std::unordered_set<std::string> stmts_with_valid_path;
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

        // if start node affects outgoing node, add to stmts_with_valid_path
        if (ValidatePossibleAffectsRelationship(
                var_modified_in_start_node,
                CFGNode::GetVarUsedInEndNode(outgoing_node)) &&
            outgoing_node->GetNodeType() == StmtType::ASSIGN_STMT) {
          stmts_with_valid_path.insert(
              std::to_string(outgoing_node->GetNodeStmtIndex()));
          cache->CacheAffects(start_node, outgoing_node);
        }

        if (ShouldVisit(outgoing_node, var_modified_in_start_node)) {
          nodes_to_visit.push(outgoing_node);
        }
      }
    }
  }
  cache->InsertCompletedForwardTraversal(start_node);
  return stmts_with_valid_path;
}

std::unordered_set<std::string>
GraphRelationTraverser::GetAllStmtsWithAffectsPathTo(
    std::shared_ptr<CFGNode> const& end_node,
    std::shared_ptr<AffectsCache> cache) {
  if (end_node->GetNodeType() != StmtType::ASSIGN_STMT) {
    return std::unordered_set<std::string>();
  }

  if (cache->CompletedBackwardTraversal(end_node)) {
    return cache->GetAllInvAffects(end_node);
  }

  std::stack<std::shared_ptr<CFGNode>> nodes_to_visit;
  std::unordered_set<std::pair<int, std::unordered_set<std::string>>,
                     PairHashValueSet>
      visited;
  std::stack<std::unordered_set<std::string>> vars_used_in_nodes;
  std::unordered_set<std::string> stmts_with_valid_path;

  std::unordered_set<std::string> unmodified_vars_used_in_end_node =
      CFGNode::GetVarUsedInEndNode(end_node);
  for (std::shared_ptr<CFGNode> incoming_node : end_node->GetIncomingNodes()) {
    nodes_to_visit.push(incoming_node);
    vars_used_in_nodes.push(unmodified_vars_used_in_end_node);
    visited.insert(make_pair(incoming_node->GetNodeStmtIndex(),
                             unmodified_vars_used_in_end_node));
  }

  while (!nodes_to_visit.empty()) {
    std::shared_ptr<CFGNode> curr_node = nodes_to_visit.top();
    std::unordered_set<std::string> unmodified_vars_used_in_end_node =
        vars_used_in_nodes.top();
    nodes_to_visit.pop();
    vars_used_in_nodes.pop();

    if (curr_node->GetNodeType() == StmtType::ASSIGN_STMT &&
        ValidatePossibleAffectsRelationship(
            CFGNode::GetVarModifiedInStartNode(curr_node),
            unmodified_vars_used_in_end_node)) {
      cache->CacheAffects(curr_node, end_node);
      stmts_with_valid_path.insert(
          std::to_string(curr_node->GetNodeStmtIndex()));
    }

    // if incoming node changes a variable used by the end node, remove it from
    // unmodifed vars
    if (curr_node->GetNodeType() == StmtType::CALL_STMT ||
        curr_node->GetNodeType() == StmtType::READ_STMT ||
        curr_node->GetNodeType() == StmtType::ASSIGN_STMT) {
      for (auto v : curr_node->GetModifiesVars()) {
        unmodified_vars_used_in_end_node.erase(v);
      }
    }

    for (std::shared_ptr<CFGNode> incoming_node :
         curr_node->GetIncomingNodes()) {
      if (!visited.count(make_pair(incoming_node->GetNodeStmtIndex(),
                                   unmodified_vars_used_in_end_node))) {
        nodes_to_visit.push(incoming_node);
        vars_used_in_nodes.push(unmodified_vars_used_in_end_node);
        visited.insert(make_pair(incoming_node->GetNodeStmtIndex(),
                                 unmodified_vars_used_in_end_node));
      }
    }
  }
  cache->InsertCompletedBackwardTraversal(end_node);
  return stmts_with_valid_path;
}
