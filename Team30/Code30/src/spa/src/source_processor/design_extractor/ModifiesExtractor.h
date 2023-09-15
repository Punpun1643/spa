#pragma once

#include <memory>

#include "../node/ProcedureNode.h"
#include "../node/ProgramNode.h"
#include "../node/StmtLstNode.h"
#include "../node/stmt_node/CallNode.h"
#include "../node/stmt_node/PrintNode.h"
#include "../node/stmt_node/ReadNode.h"
#include "IDesignExtractor.h"

class ModifiesExtractor : public IDesignExtractor {
 public:
  explicit ModifiesExtractor(PkbApi& pkb);

  explicit ModifiesExtractor();

  void extractFromProgram(std::shared_ptr<ProgramNode> node) override;

  void extractFromProcedure(std::shared_ptr<ProcedureNode> node) override;

  void extractFromStmtLst(std::shared_ptr<StmtLstNode> node) override;

  void extractFromCall(std::shared_ptr<CallNode> node) override;

  void extractFromPrint(std::shared_ptr<PrintNode> node) override;

  void extractFromRead(std::shared_ptr<ReadNode> node) override;

  // void extractFromWhile(std::shared_ptr<WhileNode> node) override;
  //
  // void extractFromIf(std::shared_ptr<IfNode> node) override;
  //
  // void extractFromAssign(std::shared_ptr<AssignNode> node) override;

  ~ModifiesExtractor() = default;

 private:
  PkbApi& pkb;

  std::vector<std::string> modifyActors;
};