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

class ModifiesExtractor : public UsesModifiesTypeExtractor {
 public:
  explicit ModifiesExtractor(PKBSPInterface& pkb,
                             std::shared_ptr<CallsManager> callsManager);

  void ExtractFromRead(std::shared_ptr<ReadNode> node) override;

  void ExtractFromAssign(std::shared_ptr<AssignNode> node) override;

  ~ModifiesExtractor() = default;

 private:
  PKBSPInterface& pkb;

  std::shared_ptr<CallsManager> callsManager;

  void InsertCondVars(std::unordered_set<std::string> condVars,
                      std::string stmtIndex);

  void InsertVarWithActors(std::string var);
};