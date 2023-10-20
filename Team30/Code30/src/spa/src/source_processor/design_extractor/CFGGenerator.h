#pragma once

#include <unordered_map>
#include <memory>
#include <vector>

#include "../../program_knowledge_base/PKBSPInterface.h"
#include "../node/ProcedureNode.h"
#include "../node/util_node/CFGNode.h"

class CFGGenerator {
 public:
  explicit CFGGenerator(PKBSPInterface& pkb);

  void ExecuteCFGGeneration(std::shared_ptr<ProcedureNode> procNode);

 private:
  PKBSPInterface& pkb;

  std::shared_ptr<CFGNode> GenerateCFG(
      std::vector<std::shared_ptr<StmtNode>> stmts,
      std::vector<std::shared_ptr<CFGNode>> lastNodePointsTo);
};
