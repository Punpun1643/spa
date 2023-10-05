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
#include "UsesModifiesTypeExtractor.h"

class ModifiesExtractor : public UsesModifiesTypeExtractor {
 public:
  explicit ModifiesExtractor(PkbApi& pkb, std::shared_ptr<CallsManager> callsManager);

  void extractFromProgram(std::shared_ptr<ProgramNode> node) override;

  void extractFromRead(std::shared_ptr<ReadNode> node) override;

  void extractFromAssign(std::shared_ptr<AssignNode> node) override;

  ~ModifiesExtractor() = default;

 private:
  PkbApi& pkb;

  std::shared_ptr<CallsManager> callsManager;

  void insertCondVars(std::unordered_set<std::string> condVars,
                      std::string stmtIndex);

  void insertVarWithActors(std::string var);
};