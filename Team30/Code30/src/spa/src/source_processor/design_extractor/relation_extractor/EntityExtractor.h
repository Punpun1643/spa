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

class EntityExtractor : public DesignExtractor {
 public:
  explicit EntityExtractor(PKBSPInterface& pkb);

  void ExtractFromProcedure(std::shared_ptr<ProcedureNode> node) override;

  void ExtractFromCall(std::shared_ptr<CallNode> node) override;

  void ExtractFromPrint(std::shared_ptr<PrintNode> node) override;

  void ExtractFromRead(std::shared_ptr<ReadNode> node) override;

  void ExtractFromWhile(std::shared_ptr<WhileNode> node) override;

  void ExtractFromIf(std::shared_ptr<IfNode> node) override;

  void ExtractFromAssign(std::shared_ptr<AssignNode> node) override;

  ~EntityExtractor() = default;

 private:
  PKBSPInterface& pkb;
};
