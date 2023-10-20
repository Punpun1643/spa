#pragma once

#include <memory>

#include "../../node/ProcedureNode.h"
#include "../../node/ProgramNode.h"
#include "../../node/StmtLstNode.h"
#include "../../node/stmt_node/AssignNode.h"
#include "../../node/stmt_node/CallNode.h"
#include "../../node/stmt_node/IfNode.h"
#include "../../node/stmt_node/PrintNode.h"
#include "../../node/stmt_node/ReadNode.h"
#include "../../node/stmt_node/WhileNode.h"
#include "../DesignExtractor.h"

class ConstVarExtractor : public DesignExtractor {
 public:
  explicit ConstVarExtractor(PKBSPInterface& pkb);

  void ExtractFromProcedure(std::shared_ptr<ProcedureNode> node) override;

  void ExtractFromPrint(std::shared_ptr<PrintNode> node) override;

  void ExtractFromRead(std::shared_ptr<ReadNode> node) override;

  void ExtractFromWhile(std::shared_ptr<WhileNode> node) override;

  void ExtractFromIf(std::shared_ptr<IfNode> node) override;

  void ExtractFromAssign(std::shared_ptr<AssignNode> node) override;

  ~ConstVarExtractor() = default;

 private:
  PKBSPInterface& pkb;

  void InsertConstsVars(std::unordered_set<int> consts,
                        std::unordered_set<std::string> vars);
};
