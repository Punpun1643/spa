#include "ExtractionController.h"

#include "relation_extractor/ConstVarExtractor.h"
#include "relation_extractor/EntityExtractor.h"
#include "relation_extractor/FollowsExtractor.h"
#include "relation_extractor/ModifiesExtractor.h"
#include "relation_extractor/ParentExtractor.h"
#include "relation_extractor/UsesExtractor.h"

ExtractionController::ExtractionController(PKBSPInterface& pkb) : pkb(pkb) {
  callsManager = std::make_shared<CallsManager>(pkb);
  cfgGenerator = std::make_shared<CFGGenerator>(pkb);

  extractors.push_back(std::make_shared<FollowsExtractor>(pkb));
  extractors.push_back(std::make_shared<ParentExtractor>(pkb));
  extractors.push_back(std::make_shared<UsesExtractor>(pkb, callsManager));
  extractors.push_back(std::make_shared<ModifiesExtractor>(pkb, callsManager));
  extractors.push_back(std::make_shared<EntityExtractor>(pkb));
  extractors.push_back(std::make_shared<ConstVarExtractor>(pkb));
}

void ExtractionController::ExecuteProgramExtraction(
    std::shared_ptr<ProgramNode> node) {
  for (std::shared_ptr<IDesignExtractor> e : extractors) {
    node->accept(*e);
  }
  std::vector<std::shared_ptr<ProcedureNode>> children = node->getChildren();
  if (!children.empty()) {

      // Support creation of the procedure calls graph
    for (std::shared_ptr<ProcedureNode> child : children) {
      callsManager->InsertProcNode(child->getProcedureName());
    }

    // Carry out the DFS extraction 
    for (std::shared_ptr<ProcedureNode> child : children) {
      ExecuteProcedureExtraction(child);
    }

    // Construct CFGs
    for (std::shared_ptr<ProcedureNode> child : children) {
       cfgGenerator->ExecuteCFGGeneration(child);
    }
  }
  ExecutePostProcessing();
}

void ExtractionController::ExecuteProcedureExtraction(
    std::shared_ptr<ProcedureNode> node) {
  for (std::shared_ptr<IDesignExtractor> e : extractors) {
    node->accept(*e);
  }
  std::vector<std::shared_ptr<StmtLstNode>> children = node->getChildren();
  if (!children.empty()) {
    for (std::shared_ptr<StmtLstNode> child : children) {
      ExecuteStmtLstExtraction(child);
    }
    PopActors();
  }
}

void ExtractionController::ExecuteStmtLstExtraction(
    std::shared_ptr<StmtLstNode> node) {
  for (std::shared_ptr<IDesignExtractor> e : extractors) {
    node->accept(*e);
  }
  std::vector<std::shared_ptr<StmtNode>> children = node->getChildren();
  if (!children.empty()) {
    for (std::shared_ptr<StmtNode> child : children) {
      ExecuteStmtExtraction(child);
    }
  }
}

void ExtractionController::ExecuteStmtExtraction(
    std::shared_ptr<StmtNode> node) {
  for (std::shared_ptr<IDesignExtractor> e : extractors) {
    node->accept(*e);
  }
  HandleContainerStmts(node);
}

void ExtractionController::HandleContainerStmts(
    std::shared_ptr<StmtNode> node) {
  // Handle whileNodes
  if (node->getStmtType() == StmtType::WHILE_STMT) {
    std::shared_ptr<WhileNode> asWhile =
        std::dynamic_pointer_cast<WhileNode>(node);
    std::shared_ptr<StmtLstNode> whileBody = asWhile->getStmtLst();
    ExecuteStmtLstExtraction(whileBody);
    PopActors();
  }

  // Handle ifNodes
  if (node->getStmtType() == StmtType::IF_STMT) {
    std::shared_ptr<IfNode> asIf = std::dynamic_pointer_cast<IfNode>(node);
    std::shared_ptr<StmtLstNode> thenBody = asIf->getThenStmtLst();
    std::shared_ptr<StmtLstNode> elseBody = asIf->getElseStmtLst();
    ExecuteStmtLstExtraction(thenBody);
    ExecuteStmtLstExtraction(elseBody);
    PopActors();
  }
}

void ExtractionController::PopActors() {
  std::dynamic_pointer_cast<UsesExtractor>(extractors.at(2))->PopActor();
  std::dynamic_pointer_cast<ModifiesExtractor>(extractors.at(3))->PopActor();
}

void ExtractionController::ExecutePostProcessing() {
  callsManager->ExecuteCallsExtraction();
  callsManager->ConnectProcsAndUpdateRelations();
}
