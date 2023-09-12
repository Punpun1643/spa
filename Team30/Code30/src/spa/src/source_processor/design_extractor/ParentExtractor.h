#pragma once

#include <memory>

#include "IDesignExtractor.h"
#include "../node/ProgramNode.h"
#include "../node/ProcedureNode.h"
#include "../node/StmtLstNode.h"
#include "../node/stmt_node/CallNode.h"
#include "../node/stmt_node/PrintNode.h"
#include "../node/stmt_node/ReadNode.h"

class ParentExtractor : public IDesignExtractor {
 public:
  explicit ParentExtractor(PkbApi& pkb);

  void extractFromProgram(std::shared_ptr<ProgramNode> node) override;

  void extractFromProcedure(std::shared_ptr<ProcedureNode> node) override;

  void extractFromStmtLst(std::shared_ptr<StmtLstNode> node) override;

  void extractFromCall(std::shared_ptr<CallNode> node) override;

  void extractFromPrint(std::shared_ptr<PrintNode> node) override;

  void extractFromRead(std::shared_ptr<ReadNode> node) override;

  ~ParentExtractor() = default;

 private:
  PkbApi& pkb;
};