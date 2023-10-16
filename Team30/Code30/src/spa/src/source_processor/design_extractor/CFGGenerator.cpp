#include "CFGGenerator.h"

#include <iostream>

#include "../node/stmt_node/IfNode.h"
#include "../node/stmt_node/WhileNode.h"

CFGGenerator::CFGGenerator(PKBSPInterface& pkb) : pkb(pkb) {}

void CFGGenerator::ExecuteCFGGeneration(
    std::shared_ptr<ProcedureNode> procNode) {
  std::shared_ptr<StmtLstNode> stmtLst = procNode->GetChildren()[0];
  std::vector<std::shared_ptr<StmtNode>> stmts = stmtLst->GetChildren();
  std::vector<std::shared_ptr<CFGNode>> lastNodePointsTo;
  GenerateCFG(stmts, lastNodePointsTo);
}

// returns the node that the current node should point to
std::shared_ptr<CFGNode> CFGGenerator::GenerateCFG(
    std::vector<std::shared_ptr<StmtNode>> stmts,
    std::vector<std::shared_ptr<CFGNode>> lastNodePointsTo) {
  if (stmts.empty()) {
    return nullptr;
  }

  // get the first node in the vector, check what kind, if normal control flow,
  // recursively call this and point to the result
  std::shared_ptr<StmtNode> currStmt = stmts.front();
  StmtType currType = currStmt->GetStmtType();
  std::unordered_set uses_vars =
      pkb.getStatementUses(std::to_string(currStmt->GetStmtIndex()));
  std::unordered_set modifies_vars =
      pkb.getStatementModifies(std::to_string(currStmt->GetStmtIndex()));
  std::shared_ptr<CFGNode> newNode =
      std::make_shared<CFGNode>(currStmt, currType, uses_vars, modifies_vars);
  stmts.erase(stmts.begin());

  std::vector<std::shared_ptr<CFGNode>> localLastNodePointsTo =
      lastNodePointsTo;

  // Nodes after the stmt
  std::vector<std::shared_ptr<CFGNode>> nextNodes;
  std::shared_ptr<CFGNode> generatedNode = GenerateCFG(stmts, lastNodePointsTo);

  if (generatedNode != nullptr) {
    nextNodes.push_back(generatedNode);
  }

  if (currType == StmtType::WHILE_STMT) {
    std::shared_ptr<WhileNode> asWhile =
        std::dynamic_pointer_cast<WhileNode>(currStmt);

    std::vector<std::shared_ptr<StmtNode>> whileBodyStmts =
        asWhile->GetStmtLst()->GetChildren();

    lastNodePointsTo.push_back(newNode);
    lastNodePointsTo.insert(lastNodePointsTo.end(), nextNodes.begin(),
                            nextNodes.end());

    std::shared_ptr<CFGNode> whileBodyCFG =
        GenerateCFG(whileBodyStmts, lastNodePointsTo);

    if (whileBodyCFG != nullptr) {
      nextNodes.push_back(whileBodyCFG);
    }

  } else if (currType == StmtType::IF_STMT) {
    std::shared_ptr<IfNode> asIf = std::dynamic_pointer_cast<IfNode>(currStmt);

    lastNodePointsTo.insert(lastNodePointsTo.end(), nextNodes.begin(),
                            nextNodes.end());

    std::vector<std::shared_ptr<StmtNode>> thenBodyStmts =
        asIf->GetThenStmtLst()->GetChildren();
    std::shared_ptr<CFGNode> thenBodyCFG =
        GenerateCFG(thenBodyStmts, lastNodePointsTo);

    std::vector<std::shared_ptr<StmtNode>> elseBodyStmts =
        asIf->GetElseStmtLst()->GetChildren();
    std::shared_ptr<CFGNode> elseBodyCFG =
        GenerateCFG(elseBodyStmts, lastNodePointsTo);

    if (thenBodyCFG != nullptr) {
      nextNodes.push_back(thenBodyCFG);
    }
    if (elseBodyCFG != nullptr) {
      nextNodes.push_back(elseBodyCFG);
    }
  }

  if (stmts.empty()) {
    nextNodes.insert(nextNodes.end(), localLastNodePointsTo.begin(),
                     localLastNodePointsTo.end());
  }

  for (std::shared_ptr<CFGNode> nextNode : nextNodes) {
    nextNode->addIncomingNode(newNode);
    newNode->addOutgoingNode(nextNode);
  }

  pkb.insertCFGNode(std::to_string(currStmt->GetStmtIndex()), newNode);
  return newNode;
}
