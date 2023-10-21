#include "ExtractionController.h"

#include "relation_extractor/ConstVarExtractor.h"
#include "relation_extractor/EntityExtractor.h"
#include "relation_extractor/FollowsExtractor.h"
#include "relation_extractor/ModifiesExtractor.h"
#include "relation_extractor/ParentExtractor.h"
#include "relation_extractor/UsesExtractor.h"

ExtractionController::ExtractionController(PKBSPInterface& pkb) : pkb(pkb) {
  calls_manager = std::make_shared<CallsManager>(pkb);
  cfg_generator = std::make_shared<CFGGenerator>(pkb);

  extractors.push_back(std::make_shared<FollowsExtractor>(pkb));
  extractors.push_back(std::make_shared<ParentExtractor>(pkb));
  extractors.push_back(std::make_shared<UsesExtractor>(pkb, calls_manager));
  extractors.push_back(std::make_shared<ModifiesExtractor>(pkb, calls_manager));
  extractors.push_back(std::make_shared<EntityExtractor>(pkb));
  extractors.push_back(std::make_shared<ConstVarExtractor>(pkb));
}

void ExtractionController::ExecuteProgramExtraction(
    std::shared_ptr<ProgramNode> node) {
  for (std::shared_ptr<IDesignExtractor> e : extractors) {
    node->Accept(*e);
  }
  std::vector<std::shared_ptr<ProcedureNode>> children = node->GetChildren();
  if (!children.empty()) {

      // Support creation of the procedure calls graph
    for (std::shared_ptr<ProcedureNode> child : children) {
      calls_manager->InsertProcNode(child->GetProcedureName());
    }

    // Carry out the DFS extraction 
    for (std::shared_ptr<ProcedureNode> child : children) {
      ExecuteProcedureExtraction(child);
    }

    // Construct CFGs
    for (std::shared_ptr<ProcedureNode> child : children) {
       cfg_generator->ExecuteCFGGeneration(child);
    }
  }
  ExecutePostProcessing();
}

void ExtractionController::ExecuteProcedureExtraction(
    std::shared_ptr<ProcedureNode> node) {
  for (std::shared_ptr<IDesignExtractor> e : extractors) {
    node->Accept(*e);
  }
  std::vector<std::shared_ptr<StmtLstNode>> children = node->GetChildren();
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
    node->Accept(*e);
  }
  std::vector<std::shared_ptr<StmtNode>> children = node->GetChildren();
  if (!children.empty()) {
    for (std::shared_ptr<StmtNode> child : children) {
      ExecuteStmtExtraction(child);
    }
  }
}

void ExtractionController::ExecuteStmtExtraction(
    std::shared_ptr<StmtNode> node) {
  for (std::shared_ptr<IDesignExtractor> e : extractors) {
    node->Accept(*e);
  }
  HandleContainerStmts(node);
}

void ExtractionController::HandleContainerStmts(
    std::shared_ptr<StmtNode> node) {
  // Handle whileNodes
  if (node->GetStmtType() == StmtType::WHILE_STMT) {
    std::shared_ptr<WhileNode> as_while =
        std::dynamic_pointer_cast<WhileNode>(node);
    std::shared_ptr<StmtLstNode> while_body = as_while->GetStmtLst();
    ExecuteStmtLstExtraction(while_body);
    PopActors();
  }

  // Handle ifNodes
  if (node->GetStmtType() == StmtType::IF_STMT) {
    std::shared_ptr<IfNode> as_if = std::dynamic_pointer_cast<IfNode>(node);
    std::shared_ptr<StmtLstNode> then_body = as_if->GetThenStmtLst();
    std::shared_ptr<StmtLstNode> else_body = as_if->GetElseStmtLst();
    ExecuteStmtLstExtraction(then_body);
    ExecuteStmtLstExtraction(else_body);
    PopActors();
  }
}

void ExtractionController::PopActors() {
  std::dynamic_pointer_cast<UsesExtractor>(extractors.at(2))->PopActor();
  std::dynamic_pointer_cast<ModifiesExtractor>(extractors.at(3))->PopActor();
}

void ExtractionController::ExecutePostProcessing() {
  calls_manager->ExecuteCallsExtraction();
  calls_manager->ConnectProcsAndUpdateRelations();
}
