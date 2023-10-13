#pragma once
#include <program_knowledge_base/PKBSPInterface.h>
#include <source_processor/node/ProcedureNode.h>
#include <source_processor/node/util_node/CFGNode.h>
#include <unordered_map>

class CFGGenerator {
 public:
  explicit CFGGenerator(PKBSPInterface& pkb);

  void ExecuteCFGGeneration(std::shared_ptr<ProcedureNode> procNode);

 private:
  PKBSPInterface& pkb;

  std::unordered_map<int, std::shared_ptr<CFGNode>> nodeMap;

  std::shared_ptr<CFGNode> GenerateCFG(
      std::vector<std::shared_ptr<StmtNode>> stmts,
      std::vector<std::shared_ptr<CFGNode>> lastNodePointsTo);
};