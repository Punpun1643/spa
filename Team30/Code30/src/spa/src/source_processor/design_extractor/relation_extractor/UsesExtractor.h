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
#include "UsesModifiesTypeExtractor.h"

class UsesExtractor : public UsesModifiesTypeExtractor {
 public:
  explicit UsesExtractor(PKBSPInterface& pkb,
                         std::shared_ptr<CallsManager> calls_manager);

  void ExtractFromPrint(std::shared_ptr<PrintNode> node) override;

  void ExtractFromCall(std::shared_ptr<CallNode> node) override;

  void ExtractFromWhile(std::shared_ptr<WhileNode> node) override;

  void ExtractFromIf(std::shared_ptr<IfNode> node) override;

  void ExtractFromAssign(std::shared_ptr<AssignNode> node) override;

  ~UsesExtractor() = default;

 private:
  PKBSPInterface& pkb;

  std::shared_ptr<CallsManager> calls_manager;

  void InsertMultipleVars(std::unordered_set<std::string> vars,
                          std::string stmt_index);

  void InsertVarWithActors(std::string var);
};