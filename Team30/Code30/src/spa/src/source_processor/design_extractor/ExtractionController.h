#pragma once

#include <memory>
#include <vector>

#include "../node/ANode.h"
#include "CallStmtCacheObject.h"
#include "CallsManager.h"
#include "IDesignExtractor.h"
#include "IExtractionController.h"

class ExtractionController : public IExtractionController {
 public:
  explicit ExtractionController(PkbApi& pkb);

  void executeProgramExtraction(std::shared_ptr<ProgramNode> node);
  void executeProcedureExtraction(std::shared_ptr<ProcedureNode> node);
  void executeStmtLstExtraction(std::shared_ptr<StmtLstNode> node);
  void executeStmtExtraction(std::shared_ptr<StmtNode> node);

  ~ExtractionController() = default;

 private:
  std::vector<std::shared_ptr<IDesignExtractor>> extractors;
  PkbApi& pkb;
  std::shared_ptr<CallsManager> callsManager;

  void handleContainerStmts(std::shared_ptr<StmtNode> node);
  void handleCallStmts();
  void handleCallStmtsHelper(
      std::vector<std::shared_ptr<CallStmtCacheObject>> cache,
      RelationType rel);
  void popActors();
};