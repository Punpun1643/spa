#pragma once

#include <memory>
#include <vector>

#include "../node/ANode.h"
#include "CallsManager.h"
#include "IDesignExtractor.h"
#include "IExtractionController.h"

class ExtractionController : public IExtractionController {
 public:
  explicit ExtractionController(PKBSPInterface& pkb);

  void executeProgramExtraction(std::shared_ptr<ProgramNode> node);
  void executeProcedureExtraction(std::shared_ptr<ProcedureNode> node);
  void executeStmtLstExtraction(std::shared_ptr<StmtLstNode> node);
  void executeStmtExtraction(std::shared_ptr<StmtNode> node);

  ~ExtractionController() = default;

 private:
  std::vector<std::shared_ptr<IDesignExtractor>> extractors;
  PKBSPInterface& pkb;
  std::shared_ptr<CallsManager> callsManager;

  void handleContainerStmts(std::shared_ptr<StmtNode> node);
  void popActors();
  void executePostProcessing();
};