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

  std::unordered_map<int, std::shared_ptr<CFGNode>> node_map;

  void GenerateCFG(std::vector<std::shared_ptr<StmtNode>> stmts);

  void InsertCFGNodes();

  void MapCFGNodes(std::vector<std::shared_ptr<StmtNode>> stmts);

  void HandleStmt(std::shared_ptr<StmtNode> curr_stmt,
                  std::shared_ptr<StmtNode> next_stmt);

  void HandleWhileStmt(std::shared_ptr<StmtNode> curr_stmt);

  void HandleIfStmt(std::shared_ptr<StmtNode> curr_stmt,
                    std::shared_ptr<StmtNode> next_stmt);


  std::vector<std::shared_ptr<CFGNode>> CheckAndInsert(
      std::vector<std::shared_ptr<CFGNode>> node_list,
      std::shared_ptr<CFGNode> node);
};
