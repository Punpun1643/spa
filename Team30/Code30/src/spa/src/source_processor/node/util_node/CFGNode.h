#pragma once

#include <query_processing_system/common/EntityType.h>

#include <memory>
#include <string>
#include <unordered_set>
#include <vector>

#include "../stmt_node/StmtNode.h"

class CFGNode {
 public:
  CFGNode(std::shared_ptr<StmtNode> node, StmtType stmtType,
          std::unordered_set<std::string> uses_vars,
          std::unordered_set<std::string> modifies_vars);

  std::vector<std::shared_ptr<CFGNode>> getOutgoingNodes();
  std::vector<std::shared_ptr<CFGNode>> getIncomingNodes();
  std::shared_ptr<StmtNode> getNode();
  StmtType getNodeType();
  std::unordered_set<std::string> getUsesVars();
  std::unordered_set<std::string> getModifiesVars();
  StmtType getStmtType();

  bool UsesVar(std::string var);
  bool ModifiesVar(std::string var);

  void addOutgoingNode(std::shared_ptr<CFGNode>);
  void addIncomingNode(std::shared_ptr<CFGNode>);

  static bool HasPath(std::shared_ptr<CFGNode> startNode,
                      std::shared_ptr<CFGNode> endNode);

  static bool HasAffectPath(std::shared_ptr<CFGNode> startNode,
                            std::shared_ptr<CFGNode> endNode);

 private:
  std::vector<std::shared_ptr<CFGNode>> outgoingNodes;
  std::vector<std::shared_ptr<CFGNode>> incomingNodes;
  std::shared_ptr<StmtNode> node;

  std::unordered_set<std::string> uses_vars;
  std::unordered_set<std::string> modifies_vars;
  StmtType stmtType;
};