#include "CFGGenerator.h"

#include <source_processor/node/stmt_node/IfNode.h>
#include <source_processor/node/stmt_node/WhileNode.h>

#include <iostream>

CFGGenerator::CFGGenerator(PKBSPInterface& pkb) : pkb(pkb) {}

void CFGGenerator::ExecuteCFGGeneration(
    std::shared_ptr<ProcedureNode> procNode) {
  std::vector<std::shared_ptr<StmtNode>> stmts =
      procNode->getChildren()[0]->getChildren();
  std::vector<std::shared_ptr<CFGNode>> lastNodePointsTo;
  GenerateCFG(stmts, lastNodePointsTo);

  //for (auto kv : nodeMap) {
  //  std::cout << "NODE " + std::to_string(kv.first) + " contains: \n";

  //  std::shared_ptr<CFGNode> node = kv.second;

  //  std::cout << "Incoming: ";
  //  for (std::shared_ptr<CFGNode> in : node->getIncomingNodes()) {
  //    std::cout << std::to_string(in->getNode()->getStmtIndex()) + ", ";
  //  }
  //  std::cout << "\n";

  //  std::cout << "Outgoing: ";
  //  for (std::shared_ptr<CFGNode> in : node->getOutgoingNodes()) {
  //    std::cout << std::to_string(in->getNode()->getStmtIndex()) + ", ";
  //  }
  //  std::cout << "\n";
  //  std::cout << "\n";

  //  pkb.insertCFGNode(std::to_string(kv.first), kv.second);
  //}
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
  std::shared_ptr<CFGNode> newNode = std::make_shared<CFGNode>(currStmt);
  stmts.erase(stmts.begin());

  if (nodeMap.find(currStmt->getStmtIndex()) == nodeMap.end()) {
    nodeMap.insert({currStmt->getStmtIndex(), newNode});
  }

  std::vector<std::shared_ptr<CFGNode>> localLastNodePointsTo =
      lastNodePointsTo;

  // std::cout << "CURR STMT: " + std::to_string(currStmt->getStmtIndex()) +
  //                  "----------------\n";

  // std::cout << std::to_string(currStmt->getStmtIndex()) +
  //                  " - LAST NODE POINTS TO: ";
  // for (std::shared_ptr<CFGNode> node : lastNodePointsTo) {
  //   std::cout << std::to_string(node->getNode()->getStmtIndex()) + ", ";
  // }
  // std::cout << "\n";

  // Nodes after the stmt
  std::vector<std::shared_ptr<CFGNode>> nextNodes;
  std::shared_ptr<CFGNode> generatedNode = GenerateCFG(stmts, lastNodePointsTo);

  if (generatedNode != nullptr) {
    nextNodes.push_back(generatedNode);
  }

  // std::cout << std::to_string(currStmt->getStmtIndex()) + " - NEXT NODES A:
  // "; for (std::shared_ptr<CFGNode> nextNode : nextNodes) {
  //   std::cout << std::to_string(nextNode->getNode()->getStmtIndex()) + ", ";
  // }
  // std::cout << "\n";

  if (currType == StmtType::WHILE_STMT) {
    std::shared_ptr<WhileNode> asWhile =
        std::dynamic_pointer_cast<WhileNode>(currStmt);

    std::vector<std::shared_ptr<StmtNode>> whileBodyStmts =
        asWhile->getStmtLst()->getChildren();

    lastNodePointsTo.push_back(newNode);
    lastNodePointsTo.insert(lastNodePointsTo.end(), nextNodes.begin(),
                            nextNodes.end());

    std::shared_ptr<CFGNode> whileBodyCFG =
        GenerateCFG(whileBodyStmts, lastNodePointsTo);

    // std::cout << std::to_string(currStmt->getStmtIndex()) +
    //                  " - WHILE BODY CFG RETURNED: " +
    //                  std::to_string(whileBodyCFG->getNode()->getStmtIndex())
    //                  +
    //                  "\n";

    // CheckAndInsertNode(nextNodes, whileBodyCFG);

    if (whileBodyCFG != nullptr) {
      // std::cout << "SUCCESSFULLY INSERTED \n";
      nextNodes.push_back(whileBodyCFG);
    }

    // std::cout << std::to_string(currStmt->getStmtIndex()) +
    //                  " - NEXT NODES WHILE: ";
    // for (std::shared_ptr<CFGNode> nextNode : nextNodes) {
    //   std::cout << std::to_string(nextNode->getNode()->getStmtIndex()) + ",
    //   ";
    // }
    // std::cout << "\n";

  } else if (currType == StmtType::IF_STMT) {
    std::shared_ptr<IfNode> asIf = std::dynamic_pointer_cast<IfNode>(currStmt);

    lastNodePointsTo.insert(lastNodePointsTo.end(), nextNodes.begin(),
                            nextNodes.end());

    std::vector<std::shared_ptr<StmtNode>> thenBodyStmts =
        asIf->getThenStmtLst()->getChildren();
    std::shared_ptr<CFGNode> thenBodyCFG =
        GenerateCFG(thenBodyStmts, lastNodePointsTo);

    std::vector<std::shared_ptr<StmtNode>> elseBodyStmts =
        asIf->getElseStmtLst()->getChildren();
    std::shared_ptr<CFGNode> elseBodyCFG =
        GenerateCFG(elseBodyStmts, lastNodePointsTo);

    // CheckAndInsertNode(nextNodes, thenBodyCFG);
    // CheckAndInsertNode(nextNodes, elseBodyCFG);

    if (thenBodyCFG != nullptr) {
      // std::cout << "SUCCESSFULLY INSERTED \n";
      nextNodes.push_back(thenBodyCFG);
    }
    if (elseBodyCFG != nullptr) {
      // std::cout << "SUCCESSFULLY INSERTED \n";
      nextNodes.push_back(elseBodyCFG);
    }

    // std::cout << std::to_string(currStmt->getStmtIndex()) +
    //                  " - NEXT NODES IF: ";
    // for (std::shared_ptr<CFGNode> nextNode : nextNodes) {
    //   std::cout << std::to_string(nextNode->getNode()->getStmtIndex()) + ",
    //   ";
    // }
    // std::cout << "\n";
  }

  if (stmts.empty()) {
    nextNodes.insert(nextNodes.end(), localLastNodePointsTo.begin(),
                     localLastNodePointsTo.end());

    // for (std::shared_ptr<CFGNode> exitNode : localLastNodePointsTo) {
    //   exitNode->addIncomingNode(newNode);
    //   newNode->addOutgoingNode(exitNode);

    //  if (newNode->getNode()->getStmtIndex() == 2) {
    //    std::cout << std::to_string(exitNode->getNode()->getStmtIndex()) +
    //                     " inserted into " +
    //                     std::to_string(newNode->getNode()->getStmtIndex()) +
    //                     "\n";
    //  }

    //  // if (exitNode->getNode()->getStmtIndex() == 2) {
    //  //   std::cout << std::to_string(newNode.getNode()->getStmtIndex()) +
    //  //                    " inserted into " +
    //  // std::to_string(exitNode->getNode()->getStmtIndex())
    //  //                    +
    //  //                    "\n";
    //  // }
    //}
  }

  // std::cout << std::to_string(currStmt->getStmtIndex()) + " - NEXT NODES B:";
  // for (std::shared_ptr<CFGNode> nextNode : nextNodes) {
  //   std::cout << std::to_string(nextNode->getNode()->getStmtIndex()) + ", ";
  // }
  // std::cout << "\n";

  for (std::shared_ptr<CFGNode> nextNode : nextNodes) {
    nextNode->addIncomingNode(newNode);
    newNode->addOutgoingNode(nextNode);

    //nodeMap.find(nextNode->getNode()->getStmtIndex())
    //    ->second->addIncomingNode(
    //        nodeMap.find(newNode->getNode()->getStmtIndex())->second);

    //nodeMap.find(newNode->getNode()->getStmtIndex())
    //    ->second->addOutgoingNode(
    //        nodeMap.find(nextNode->getNode()->getStmtIndex())->second);

    // if (newNode.getNode()->getStmtIndex() == 5) {
    //   std::cout << std::to_string(nextNode->getNode()->getStmtIndex()) +
    //                    " inserted into " +
    //                    std::to_string(newNode.getNode()->getStmtIndex()) +
    //                    "\n";
    // }

    // if (nextNode->getNode()->getStmtIndex() == 2) {
    //   std::cout << std::to_string(newNode->getNode()->getStmtIndex()) +
    //                    " inserted into " +
    //                    std::to_string(nextNode->getNode()->getStmtIndex()) +
    //                    "\n";
    // }

    // std::cout << " -- " + std::to_string(newNode->getNode()->getStmtIndex())
    // +
    //                  " contains (outgoing)";
    // for (std::shared_ptr<CFGNode> nextNode : newNode->getOutgoingNodes()) {
    //   std::cout << std::to_string(nextNode->getNode()->getStmtIndex()) + ",
    //   ";
    // }
    // std::cout << "\n";

    // std::cout << " -- " + std::to_string(nextNode->getNode()->getStmtIndex())
    // +
    //                  " contains (incoming)";
    // for (std::shared_ptr<CFGNode> nn : nextNode->getIncomingNodes()) {
    //   std::cout << std::to_string(nn->getNode()->getStmtIndex()) + ", ";
    // }
    // std::cout << "\n";
  }

  // insert into pkb somewhere here

  // std::cout << "Inserting node: " + std::to_string(currStmt->getStmtIndex())
  // +
  //                  "\n";
  // for (std::shared_ptr<CFGNode> currNode : newNode->getIncomingNodes()) {
  //   std::cout << std::to_string(currNode->getNode()->getStmtIndex()) +
  //                    " has incoming nodes: ";
  //   for (std::shared_ptr<CFGNode> incomingNode :
  //   currNode->getIncomingNodes()) {
  //     std::cout << std::to_string(incomingNode->getNode()->getStmtIndex()) +
  //                      ", ";
  //   }
  //   std::cout << "\n";
  // }

  pkb.insertCFGNode(std::to_string(currStmt->getStmtIndex()), newNode);
  return newNode;
}

void CFGGenerator::CheckAndInsertNode(
    std::vector<std::shared_ptr<CFGNode>> nextNodes,
    std::shared_ptr<CFGNode> newNode) {
  std::cout << " - INSERTING AFT CHECK " +
                   std::to_string(newNode->getNode()->getStmtIndex()) + "\n";

  if (newNode != nullptr) {
    std::cout << "SUCCESSFULLY INSERTED \n";
    nextNodes.push_back(newNode);
  }
}
