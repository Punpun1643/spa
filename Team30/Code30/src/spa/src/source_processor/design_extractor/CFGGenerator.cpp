#include "CFGGenerator.h"

#include <source_processor/node/stmt_node/IfNode.h>
#include <source_processor/node/stmt_node/WhileNode.h>

CFGGenerator::CFGGenerator(PKBSPInterface& pkb) : pkb(pkb) {}

void CFGGenerator::GenerateAndInsertCFG(
    std::shared_ptr<ProcedureNode> procNode) {
  std::shared_ptr<CFGNode> cfg;
  std::vector<std::shared_ptr<StmtNode>> stmts =
      procNode->getChildren()[0]->getChildren();
  GenerateCFG(stmts);
}

// returns a vector of all possible nodes that the current node must now point
// to
std::vector<std::shared_ptr<CFGNode>> CFGGenerator::GenerateCFG(
    std::vector<std::shared_ptr<StmtNode>> stmts) {
  // check if vector empty or sth
  if (stmts.empty()) {
    std::vector<std::shared_ptr<CFGNode>> emptyRes;
    return emptyRes;
  }

  // get the first node in the vector, check what kind, if normal control flow,
  // recursively call this and point to the result
  std::shared_ptr<StmtNode> currStmt = stmts.front();
  StmtType currType = currStmt->getStmtType();
  CFGNode newNode = CFGNode(currStmt);
  stmts.erase(stmts.begin());

  // Nodes after the stmt
  std::vector<std::shared_ptr<CFGNode>> nextNodes = GenerateCFG(stmts);

  if (currType == StmtType::WHILE_STMT) {
    std::vector<std::shared_ptr<StmtNode>> whileBodyStmts =
        std::dynamic_pointer_cast<WhileNode>(currStmt)
            ->getStmtLst()
            ->getChildren();
    std::vector<std::shared_ptr<CFGNode>> whileBodyNodes = GenerateCFG(stmts);

    nextNodes.insert(nextNodes.end(), whileBodyNodes.begin(),
                     whileBodyNodes.end());

  } else if (currType == StmtType::IF_STMT) {
    std::shared_ptr<IfNode> asIf = std::dynamic_pointer_cast<IfNode>(currStmt);

    std::vector<std::shared_ptr<StmtNode>> thenBodyStmts =
        asIf->getThenStmtLst()->getChildren();
    std::vector<std::shared_ptr<CFGNode>> thenBodyNodes =
        GenerateCFG(thenBodyStmts);

    std::vector<std::shared_ptr<StmtNode>> elseBodyStmts =
        asIf->getElseStmtLst()->getChildren();
    std::vector<std::shared_ptr<CFGNode>> elseBodyNodes =
        GenerateCFG(elseBodyStmts);

    nextNodes.insert(nextNodes.end(), thenBodyNodes.begin(),
                     thenBodyNodes.end());
    nextNodes.insert(nextNodes.end(), elseBodyNodes.begin(),
                     elseBodyNodes.end());
  }
  for (std::shared_ptr<CFGNode> nextNode : nextNodes) {
    nextNode->addIncomingNode(std::make_shared<CFGNode>(newNode));
    newNode.addOutgoingNode(nextNode);
  }

  // insert into pkb somewhere here
  std::vector<std::shared_ptr<CFGNode>> resNodes;
  resNodes.push_back(std::make_shared<CFGNode>(newNode));
  return resNodes;
}
