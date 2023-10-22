#pragma once

#include <memory>
#include <unordered_map>
#include <vector>

#include "../../program_knowledge_base/PKBSPInterface.h"
#include "../node/ProcedureNode.h"
#include "../node/util_node/CFGNode.h"

class CFGGenerator {
 public:
  explicit CFGGenerator(PKBSPInterface& pkb);

  void ExecuteCFGGeneration(std::shared_ptr<ProcedureNode> proc_node);

 private:
  PKBSPInterface& pkb;

  std::shared_ptr<CFGNode> GenerateCFG(
      std::vector<std::shared_ptr<StmtNode>> stmts,
      std::vector<std::shared_ptr<CFGNode>> last_node_points_to);

  std::vector<std::shared_ptr<CFGNode>> CheckAndInsert(
      std::vector<std::shared_ptr<CFGNode>> node_list,
      std::shared_ptr<CFGNode> node);
};
