#pragma once

#include <memory>
#include <vector>

#include "../node/ANode.h"
#include "CFGGenerator.h"
#include "CallsManager.h"
#include "IDesignExtractor.h"
#include "IExtractionController.h"

class ExtractionController : public IExtractionController {
 public:
  explicit ExtractionController(PKBSPInterface& pkb);

  void ExecuteProgramExtraction(std::shared_ptr<ProgramNode> node);
  void ExecuteProcedureExtraction(std::shared_ptr<ProcedureNode> node);
  void ExecuteStmtLstExtraction(std::shared_ptr<StmtLstNode> node);
  void ExecuteStmtExtraction(std::shared_ptr<StmtNode> node);

  ~ExtractionController() = default;

 private:
  std::vector<std::shared_ptr<IDesignExtractor>> extractors;
  PKBSPInterface& pkb;
  std::shared_ptr<CallsManager> callsManager;
  std::shared_ptr<CFGGenerator> cfgGenerator;

  void HandleContainerStmts(std::shared_ptr<StmtNode> node);
  void PopActors();
  void ExecutePostProcessing();
};