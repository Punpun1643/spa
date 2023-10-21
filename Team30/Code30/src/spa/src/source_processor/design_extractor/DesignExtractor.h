#pragma once

#include <memory>

#include "../node/ProcedureNode.h"
#include "../node/ProgramNode.h"
#include "../node/StmtLstNode.h"
#include "../node/stmt_node/AssignNode.h"
#include "../node/stmt_node/CallNode.h"
#include "../node/stmt_node/IfNode.h"
#include "../node/stmt_node/PrintNode.h"
#include "../node/stmt_node/ReadNode.h"
#include "../node/stmt_node/WhileNode.h"
#include "IDesignExtractor.h"

class DesignExtractor : public IDesignExtractor {
 public:
  explicit DesignExtractor(PKBSPInterface& pkb);

  void ExtractFromProgram(std::shared_ptr<ProgramNode> node) override;

  void ExtractFromProcedure(std::shared_ptr<ProcedureNode> node) override;

  void ExtractFromStmtLst(std::shared_ptr<StmtLstNode> node) override;

  void ExtractFromCall(std::shared_ptr<CallNode> node) override;

  void ExtractFromPrint(std::shared_ptr<PrintNode> node) override;

  void ExtractFromRead(std::shared_ptr<ReadNode> node) override;

  void ExtractFromWhile(std::shared_ptr<WhileNode> node) override;

  void ExtractFromIf(std::shared_ptr<IfNode> node) override;

  void ExtractFromAssign(std::shared_ptr<AssignNode> node) override;

  ~DesignExtractor() = default;

 private:
  PKBSPInterface& pkb;
};
