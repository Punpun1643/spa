#pragma once

#include <memory>
#include <string>
#include <unordered_set>
#include <vector>

#include "../../../query_processing_system/common/EntityType.h"
#include "../stmt_node/StmtNode.h"
#include "../stmt_node/StmtType.h"
#include "../util_node/CFGNode.h"

class CFGNode {
 public:
  CFGNode(std::shared_ptr<StmtNode> node, StmtType stmtType,
          std::unordered_set<std::string> uses_vars,
          std::unordered_set<std::string> modifies_vars);

  std::vector<std::shared_ptr<CFGNode>> GetOutgoingNodes();

  std::vector<std::shared_ptr<CFGNode>> GetIncomingNodes();

  std::shared_ptr<StmtNode> GetNode();

  StmtType GetNodeType();

  std::unordered_set<std::string> GetUsesVars();

  std::unordered_set<std::string> GetModifiesVars();

  StmtType GetStmtType();

  bool UsesVar(std::string var);
  bool ModifiesVar(std::string var);

  void AddOutgoingNode(std::shared_ptr<CFGNode>);

  void AddIncomingNode(std::shared_ptr<CFGNode>);

  static bool HasImmediatePath(std::shared_ptr<CFGNode> startNode,
                               std::shared_ptr<CFGNode> endNode);

  static bool HasPath(std::shared_ptr<CFGNode> startNode,
                      std::shared_ptr<CFGNode> endNode);

  static bool HasAffectsPath(std::shared_ptr<CFGNode> startNode,
                             std::shared_ptr<CFGNode> endNode);

  static bool IsAssignOrReadOutgoingNode(std::shared_ptr<CFGNode> node);

  static bool IsCallOutgoingNode(std::shared_ptr<CFGNode> node);

  static bool HandleAssignOrReadOutgoingNode(
      std::shared_ptr<CFGNode> outgoingNode,
      std::string const& varModifiedInStartNode);

  static bool HandleCallOutgoingNode(std::shared_ptr<CFGNode> outgoingNode,
                                     std::string const& varModifiedInStartNode);

  static bool ValidateStartAndEndNodes(std::shared_ptr<CFGNode> startNode,
                                       std::shared_ptr<CFGNode> endNode);

  static std::string GetVarModifiedInStartNode(
      std::shared_ptr<CFGNode> startNode);

  static std::unordered_set<std::string> GetVarUsedInEndNode(
      std::shared_ptr<CFGNode> endNode);

  static bool ValidatePossibleAffectsRelationship(
      std::string varModifiedInStartNode,
      std::unordered_set<std::string> varsUsedInEndNode);

 private:
  std::vector<std::shared_ptr<CFGNode>> outgoing_nodes;
  std::vector<std::shared_ptr<CFGNode>> incoming_nodes;
  std::shared_ptr<StmtNode> node;

  std::unordered_set<std::string> uses_vars;
  std::unordered_set<std::string> modifies_vars;
  StmtType stmtType;
};