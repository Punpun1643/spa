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
#include "CallStmtCacheObject.h"
#include "UsesModifiesTypeExtractor.h"

class UsesExtractor : public UsesModifiesTypeExtractor {
 public:
  explicit UsesExtractor(PkbApi& pkb,
                         std::shared_ptr<CallsManager> callsManager);

  void extractFromPrint(std::shared_ptr<PrintNode> node) override;

  void extractFromWhile(std::shared_ptr<WhileNode> node) override;

  void extractFromIf(std::shared_ptr<IfNode> node) override;

  void extractFromAssign(std::shared_ptr<AssignNode> node) override;

  ~UsesExtractor() = default;

 private:
  PkbApi& pkb;

  std::shared_ptr<CallsManager> callsManager;

  std::vector<std::shared_ptr<CallStmtCacheObject>> callStmtCache;

  void insertMultipleVars(std::unordered_set<std::string> vars,
                          std::string stmtIndex);

  void insertVarWithActors(std::string var);
};