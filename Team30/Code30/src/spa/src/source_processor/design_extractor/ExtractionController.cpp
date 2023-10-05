#include "ExtractionController.h"

#include "ConstVarExtractor.h"
#include "EntityExtractor.h"
#include "FollowsExtractor.h"
#include "ModifiesExtractor.h"
#include "ParentExtractor.h"
#include "UsesExtractor.h"

ExtractionController::ExtractionController(PKBSPInterface& pkb) : pkb(pkb) {
  callsManager = std::make_shared<CallsManager>(pkb);

  extractors.push_back(std::make_shared<FollowsExtractor>(pkb));
  extractors.push_back(std::make_shared<ParentExtractor>(pkb));
  extractors.push_back(std::make_shared<UsesExtractor>(pkb, callsManager));
  extractors.push_back(std::make_shared<ModifiesExtractor>(pkb, callsManager));
  extractors.push_back(std::make_shared<EntityExtractor>(pkb));
  extractors.push_back(std::make_shared<ConstVarExtractor>(pkb));
}

void ExtractionController::executeProgramExtraction(
    std::shared_ptr<ProgramNode> node) {
  for (std::shared_ptr<IDesignExtractor> e : extractors) {
    node->accept(*e);
  }
  std::vector<std::shared_ptr<ProcedureNode>> children = node->getChildren();
  if (!children.empty()) {
    for (std::shared_ptr<ProcedureNode> child : children) {
      callsManager->insertProcNode(child->getProcedureName());
    }
    for (std::shared_ptr<ProcedureNode> child : children) {
      executeProcedureExtraction(child);
    }
  }
  executePostProcessing();
}

void ExtractionController::executeProcedureExtraction(
    std::shared_ptr<ProcedureNode> node) {
  for (std::shared_ptr<IDesignExtractor> e : extractors) {
    node->accept(*e);
  }
  std::vector<std::shared_ptr<StmtLstNode>> children = node->getChildren();
  if (!children.empty()) {
    for (std::shared_ptr<StmtLstNode> child : children) {
      executeStmtLstExtraction(child);
    }
    popActors();
  }
}

void ExtractionController::executeStmtLstExtraction(
    std::shared_ptr<StmtLstNode> node) {
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

void ExtractionController::executeStmtExtraction(
    std::shared_ptr<StmtNode> node) {
  for (std::shared_ptr<IDesignExtractor> e : extractors) {
    node->accept(*e);
  }
  handleContainerStmts(node);
}

void ExtractionController::handleContainerStmts(
    std::shared_ptr<StmtNode> node) {
  // Handle whileNodes
  if (node->getStmtType() == StmtType::WHILE_STMT) {
    std::shared_ptr<WhileNode> asWhile =
        std::dynamic_pointer_cast<WhileNode>(node);
    std::shared_ptr<StmtLstNode> whileBody = asWhile->getStmtLst();
    executeStmtLstExtraction(whileBody);
    popActors();
  }

  // Handle ifNodes
  if (node->getStmtType() == StmtType::IF_STMT) {
    std::shared_ptr<IfNode> asIf = std::dynamic_pointer_cast<IfNode>(node);
    std::shared_ptr<StmtLstNode> thenBody = asIf->getThenStmtLst();
    std::shared_ptr<StmtLstNode> elseBody = asIf->getElseStmtLst();
    executeStmtLstExtraction(thenBody);
    executeStmtLstExtraction(elseBody);
    popActors();
  }
}

// void ExtractionController::handleCallStmts() {
//   std::vector<std::shared_ptr<CallStmtCacheObject>> usesCache =
//       std::dynamic_pointer_cast<UsesExtractor>(extractors.at(2))
//           ->getCallStmtCache();
//   handleCallStmtsHelper(usesCache, RelationType::USES_S);
//
//   std::vector<std::shared_ptr<CallStmtCacheObject>> modifiesCache =
//       std::dynamic_pointer_cast<UsesExtractor>(extractors.at(3))
//           ->getCallStmtCache();
//   handleCallStmtsHelper(modifiesCache, RelationType::MODIFIES_S);
// }
//
// void ExtractionController::handleCallStmtsHelper(
//     std::vector<std::shared_ptr<CallStmtCacheObject>> cache, RelationType
//     rel) {
//   bool isGettingUpdated = true;
//   while (isGettingUpdated) {
//     bool hasUpdateThisIteration = false;
//     for (std::shared_ptr<CallStmtCacheObject> callStmt : cache) {
//       std::vector<std::string> varsFromProc = *pkb.getRelationValues(
//           callStmt->getCallNode()->getProcName(), EntityType::VARIABLE, rel);
//
//       bool hasUpdate = callStmt->updateVars(varsFromProc);
//       bool hasUpdateThisIteration = hasUpdateThisIteration || hasUpdate;
//
//       for (std::string var : varsFromProc) {
//         for (std::string actor : callStmt->getActors()) {
//           pkb.insertRelation(rel, actor, var);
//         }
//       }
//     }
//     isGettingUpdated = hasUpdateThisIteration;
//   }
// }

void ExtractionController::popActors() {
  std::dynamic_pointer_cast<UsesExtractor>(extractors.at(2))->popActor();
  std::dynamic_pointer_cast<ModifiesExtractor>(extractors.at(3))->popActor();
}

void ExtractionController::executePostProcessing() {
  callsManager->executeCallsExtraction();
  callsManager->connectProcsAndUpdateRelations();
}
