#include "EntityExtractor.h"
#include "FollowsExtractor.h"
#include "ModifiesExtractor.h"
#include "ParentExtractor.h"
#include "UsesExtractor.h"
#include "ExtractionController.h"


ExtractionController::ExtractionController(PkbApi& pkb) : pkb(pkb) {
  extractors.push_back(std::make_shared<FollowsExtractor>(pkb));
  extractors.push_back(std::make_shared<ParentExtractor>(pkb));
  extractors.push_back(std::make_shared<UsesExtractor>(pkb));
  extractors.push_back(std::make_shared<ModifiesExtractor>(pkb));
  extractors.push_back(std::make_shared<EntityExtractor>(pkb));
}

void ExtractionController::executeProgramExtraction(std::shared_ptr<ProgramNode> node) {

  for (std::shared_ptr<IDesignExtractor> e : extractors) {
    node->accept(*e);
  }
  std::vector<std::shared_ptr<ProcedureNode>> children = node->getChildren();
  if (!children.empty()) {
    for (std::shared_ptr<ProcedureNode> child : children) {
      executeProcedureExtraction(child);
    }
  }
}

void ExtractionController::executeProcedureExtraction(std::shared_ptr<ProcedureNode> node) {
  for (std::shared_ptr<IDesignExtractor> e : extractors) {
    node->accept(*e);
  }
  std::vector<std::shared_ptr<StmtLstNode>> children = node->getChildren();
  if (!children.empty()) {
    for (std::shared_ptr<StmtLstNode> child : children) {
      executeStmtLstExtraction(child);
    }
  }
}

void ExtractionController::executeStmtLstExtraction(std::shared_ptr<StmtLstNode> node) {
  for (std::shared_ptr<IDesignExtractor> e : extractors) {
    node->accept(*e);
  }
  std::vector<std::shared_ptr<StmtNode>> children = node->getChildren();  
  if (!children.empty()) {
    for (std::shared_ptr<StmtNode> child : children) {
      executeStmtExtraction(child);
    }
  }
}

void ExtractionController::executeStmtExtraction(std::shared_ptr<StmtNode> node) {
  for (std::shared_ptr<IDesignExtractor> e : extractors) {
    node->accept(*e);
  }
}

