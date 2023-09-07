#pragma once

#include "IDesignExtractor.h"

class ModifiesExtractor : public DesignExtractor {
 public:
  void extractFromProgram(std::shared_ptr<ProgramNode> node) override;

  void extractFromProcedure(std::shared_ptr<ProcedureNode> node) override;

  void extractFromStmtLst(std::shared_ptr<StmtLstNode> node) override;

  void extractFromCall(std::shared_ptr<CallNode> node) override;

  void extractFromPrint(std::shared_ptr<PrintNode> node) override;

  void extractFromRead(std::shared_ptr<ReadNode> node) override;
};