#pragma once
#include <program_knowledge_base/PKBSPInterface.h>
#include <source_processor/node/ProcedureNode.h>

class CFGGenerator {
 public:
  explicit CFGGenerator(PKBSPInterface& pkb);

  void GenerateAndInsertCFG(std::shared_ptr<ProcedureNode> procNode);

 private:
  PKBSPInterface& pkb;
};