#include "CFGGenerator.h"

#include <source_processor/node/stmt_node/IfNode.h>
#include <source_processor/node/stmt_node/WhileNode.h>

CFGGenerator::CFGGenerator(PKBSPInterface& pkb) : pkb(pkb) {}

void CFGGenerator::ExecuteCFGGeneration(
    std::shared_ptr<ProcedureNode> procNode) {
  std::vector<std::shared_ptr<StmtNode>> stmts =
      procNode->getChildren()[0]->getChildren();
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
  StmtType currType = currStmt->getStmtType();
  CFGNode newNode = CFGNode(currStmt);
  stmts.erase(stmts.begin());

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
        asWhile->getStmtLst()->getChildren();

    lastNodePointsTo.push_back(std::make_shared<CFGNode>(newNode));
    lastNodePointsTo.insert(lastNodePointsTo.end(), nextNodes.begin(), nextNodes.end());

    std::shared_ptr<CFGNode> whileBodyCFG = GenerateCFG(whileBodyStmts, lastNodePointsTo);

    CheckAndInsertNode(nextNodes, whileBodyCFG);

  } else if (currType == StmtType::IF_STMT) {
    std::shared_ptr<IfNode> asIf = std::dynamic_pointer_cast<IfNode>(currStmt);

    lastNodePointsTo.insert(lastNodePointsTo.end(), nextNodes.begin(),
                            nextNodes.end());

    std::vector<std::shared_ptr<StmtNode>> thenBodyStmts =
        asIf->getThenStmtLst()->getChildren();
    std::shared_ptr<CFGNode> thenBodyCFG = GenerateCFG(thenBodyStmts, lastNodePointsTo);

    std::vector<std::shared_ptr<StmtNode>> elseBodyStmts =
        asIf->getElseStmtLst()->getChildren();
    std::shared_ptr<CFGNode> elseBodyCFG = GenerateCFG(elseBodyStmts, lastNodePointsTo);

    CheckAndInsertNode(nextNodes, thenBodyCFG);
    CheckAndInsertNode(nextNodes, elseBodyCFG);
  }

  for (std::shared_ptr<CFGNode> nextNode : nextNodes) {
    nextNode->addIncomingNode(std::make_shared<CFGNode>(newNode));
    newNode.addOutgoingNode(nextNode);
  }

  if (stmts.empty()) {
    for (std::shared_ptr<CFGNode> exitNode : lastNodePointsTo) {
      exitNode->addIncomingNode(std::make_shared<CFGNode>(newNode));
      newNode.addOutgoingNode(exitNode);
    }
  }

  // insert into pkb somewhere here
  pkb.insertCFGNode(std::to_string(currStmt->getStmtIndex()),
                    std::make_shared<CFGNode>(newNode));
  return std::make_shared<CFGNode>(newNode);
}

void CFGGenerator::CheckAndInsertNode(
    std::vector<std::shared_ptr<CFGNode>> nextNodes,
    std::shared_ptr<CFGNode> newNode) {
  if (newNode != nullptr) {
    nextNodes.push_back(newNode);
  }
}
