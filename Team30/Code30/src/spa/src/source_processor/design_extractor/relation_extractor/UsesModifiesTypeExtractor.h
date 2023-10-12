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
#include "../CallsManager.h"
#include "../DesignExtractor.h"
#include "../IDesignExtractor.h"

class UsesModifiesTypeExtractor : public DesignExtractor {
 public:
  explicit UsesModifiesTypeExtractor(
      PKBSPInterface& pkb, std::shared_ptr<CallsManager> callsManager);

  void extractFromProcedure(std::shared_ptr<ProcedureNode> node) override;

  void extractFromWhile(std::shared_ptr<WhileNode> node) override;

  void extractFromIf(std::shared_ptr<IfNode> node) override;

  void popActor();

  ~UsesModifiesTypeExtractor() = default;

 private:
  PKBSPInterface& pkb;

  std::shared_ptr<CallsManager> callsManager;

 protected:
  std::vector<std::string> actors;
};