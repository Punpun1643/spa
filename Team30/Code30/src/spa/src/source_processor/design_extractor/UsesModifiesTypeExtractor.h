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
#include "DesignExtractor.h"

class UsesModifiesTypeExtractor : public DesignExtractor {
 public:
  explicit UsesModifiesTypeExtractor(PkbApi& pkb);

  void extractFromProcedure(std::shared_ptr<ProcedureNode> node) override;

  void extractFromCall(std::shared_ptr<CallNode> node) override;

  void extractFromWhile(std::shared_ptr<WhileNode> node) override;

  void extractFromIf(std::shared_ptr<IfNode> node) override;

  void popActor();

  std::vector<std::shared_ptr<CallStmtCacheObject>> getCallStmtCache();

  ~UsesModifiesTypeExtractor() = default;

 private:
  PkbApi& pkb;

  std::vector<std::shared_ptr<CallStmtCacheObject>> callStmtCache;

 protected:
  std::vector<std::string> actors;
};