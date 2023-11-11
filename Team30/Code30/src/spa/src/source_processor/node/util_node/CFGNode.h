#pragma once

#include <memory>
#include <string>
#include <unordered_set>
#include <vector>

#include "../stmt_node/StmtNode.h"
#include "../stmt_node/StmtType.h"
#include "../util_node/CFGNode.h"
#include "shared/types/EntityType.h"

class CFGNode {
 public:
  CFGNode(std::shared_ptr<StmtNode> node, StmtType stmt_type,
          std::unordered_set<std::string> uses_vars,
          std::unordered_set<std::string> modifies_vars);

  std::vector<std::shared_ptr<CFGNode>> GetOutgoingNodes();

  std::vector<std::shared_ptr<CFGNode>> GetIncomingNodes();

  std::shared_ptr<StmtNode> GetNode();

  StmtType GetNodeType();

  int GetNodeStmtIndex();

  std::unordered_set<std::string>& GetUsesVars();

  std::unordered_set<std::string>& GetModifiesVars();

  StmtType GetStmtType();

  bool UsesVar(std::string var);
  bool ModifiesVar(std::string var);

  void AddOutgoingNode(std::shared_ptr<CFGNode>);

  void AddIncomingNode(std::shared_ptr<CFGNode>);

  static bool IsAssignOrReadOutgoingNode(std::shared_ptr<CFGNode> node);

  static bool IsCallOutgoingNode(std::shared_ptr<CFGNode> node);

  static bool IsAssignOutgoingNode(std::shared_ptr<CFGNode> node);

  static std::string GetVarModifiedInStartNode(
      std::shared_ptr<CFGNode> start_node);

  static std::unordered_set<std::string>& GetVarUsedInEndNode(
      std::shared_ptr<CFGNode> end_node);

 private:
  std::vector<std::shared_ptr<CFGNode>> outgoing_nodes;
  std::vector<std::shared_ptr<CFGNode>> incoming_nodes;
  std::shared_ptr<StmtNode> node;

  std::unordered_set<std::string> uses_vars;
  std::unordered_set<std::string> modifies_vars;
  StmtType stmt_type;
};
