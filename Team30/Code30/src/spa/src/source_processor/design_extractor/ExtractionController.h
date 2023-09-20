#pragma once

#include <memory>
#include <vector>

#include "IExtractionController.h"
#include "IDesignExtractor.h"
#include "../node/ANode.h"
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

  void handleContainerStmts(std::shared_ptr<StmtNode> node);
  void popActors();
};