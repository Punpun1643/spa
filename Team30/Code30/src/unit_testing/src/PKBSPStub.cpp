#include "PKBSPStub.h"

#include <iostream>

PKBSPStub::PKBSPStub()
    : insertEntityCallCount(0),
      insertVariableCallCount(0),
      insertConstantCallCount(0),
      insertRelationCallCount(0),
      insertFollowsCallCount(0),
      insertParentCallCount(0),
      insertUsesCallCount(0),
      insertModifiesCallCount(0),
      insertCallsCallCount(0),
      insertPatternCallCount(0),
      insertCFGCallCount(0){};

void PKBSPStub::insertEntity(EntityType type, std::string entity) {
  if (type == EntityType::CONSTANT) {
    insertConstantCallCount++;
  } else if (type == EntityType::VARIABLE) {
    insertVariableCallCount++;
  } else {
    insertEntityCallCount++;
  }
  entitiesSet.insert(entity);
  // std::cout << "(" + std::to_string(type) + ", " + entity + ")\n";
}

void PKBSPStub::insertRelationCommon(RelationType type, std::string a,
                                     std::string b) {
  // if (type == RelationType::USES_P || type == RelationType::USES_S) {
  //    std::cout << REL_TYPE_STRINGS[type] + "(" + a + ", " + b + ")\n";
  //  }

  ++insertRelationCallCount;
  switch (type) {
    case (RelationType::FOLLOWS):
      ++insertFollowsCallCount;
      break;
    case (RelationType::PARENT):
      ++insertParentCallCount;
      break;
    case (RelationType::USES_S):
      ++insertUsesCallCount;
      break;
    case (RelationType::USES_P):
      ++insertUsesCallCount;
      break;
    case (RelationType::MODIFIES_S):
      ++insertModifiesCallCount;
      break;
    case (RelationType::MODIFIES_P):
      ++insertModifiesCallCount;
      break;
    case (RelationType::CALLS):
      ++insertCallsCallCount;
      break;
    default:
      break;
  }
}

// Relation (integer, integer)
void PKBSPStub::insertRelation(RelationType rel_type, std::string s1_line_num,
                               std::string s2_line_num) {
  insertRelationCommon(rel_type, s1_line_num, s2_line_num);
}

// Pattern clause
void PKBSPStub::insertPattern(std::string statement_number, std::string lhs,
                              std::unordered_set<std::string> rhs) {
  insertPatternCallCount++;
};
void PKBSPStub::insertPattern(PatternType type, std::string statement_number,
                              std::string lhs, std::shared_ptr<TreeNode> rhs) {
  insertPatternCallCount++;
};

std::unordered_set<std::string> PKBSPStub::getProcedureUses(
    std::string procName) {
  std::unordered_set<std::string> result;
  if (procName == "proc_AST1_A") {
    result.insert("var1");
    result.insert("var3");
    result.insert("var2");
    result.insert("var4");
    result.insert("var5");
  } else if (procName == "proc_AST1_B") {
    result.insert("y");
  } else if (procName == "proc_AST2_A") {
    result.insert("var1");
    result.insert("var2");
    result.insert("var3");
  } else if (procName == "proc_AST3_A") {
    result.insert("var1");
  } else if (procName == "proc_AST3_B") {
    result.insert("var2");
  } else if (procName == "proc_AST4_A") {
    result.insert("var1");
    result.insert("var2");
    result.insert("var3");
  } else if (procName == "proc_AST4_B") {
    result.insert("y");
  } else if (procName == "proc_AST5_A") {
  } else if (procName == "proc_AST5_B") {
  } else if (procName == "proc_AST1_C") {
    result.insert("y");
  }
  return result;
}

std::unordered_set<std::string> PKBSPStub::getProcedureModifies(
    std::string procName) {
  std::unordered_set<std::string> result;
  if (procName == "proc_AST1_A") {
    result.insert("var1");
    result.insert("var3");
    result.insert("var2");
    result.insert("var4");
    result.insert("var5");
  } else if (procName == "proc_AST1_B") {
    result.insert("y");
  } else if (procName == "proc_AST2_A") {
    result.insert("var1");
    result.insert("var2");
    result.insert("var3");
  } else if (procName == "proc_AST3_A") {
    result.insert("var1");
  } else if (procName == "proc_AST3_B") {
    result.insert("var2");
  } else if (procName == "proc_AST4_A") {
    result.insert("var1");
    result.insert("var2");
    result.insert("var3");
  } else if (procName == "proc_AST4_B") {
    result.insert("y");
  } else if (procName == "proc_AST5_A") {
  } else if (procName == "proc_AST5_B") {
  } else if (procName == "proc_AST1_C") {
    result.insert("y");
  }
  return result;
}

void PKBSPStub::insertCFGNode(std::string statement_num,
                              std::shared_ptr<CFGNode> node) {
  insertCFGCallCount++;
  CFGNodeMap.insert({statement_num, node});

  //std::cout << " - INSERTED NODE " + statement_num + " contains: \n";

  //std::cout << " - Incoming: ";
  //for (std::shared_ptr<CFGNode> in : node->getIncomingNodes()) {
  //  std::cout << std::to_string(in->getNode()->getStmtIndex()) + ", ";
  //}
  //std::cout << "\n";

  //std::cout << " - Outgoing: ";
  //for (std::shared_ptr<CFGNode> in : node->getOutgoingNodes()) {
  //  std::cout << std::to_string(in->getNode()->getStmtIndex()) + ", ";
  //}
  //std::cout << "\n";
  //std::cout << "\n";
};

bool PKBSPStub::checkCFGNodeOutgoing(std::string statement_num,
                                     std::vector<std::string> outgoingStmtNos) {
  std::shared_ptr<CFGNode> node = CFGNodeMap.find(statement_num)->second;
  int length = node->getOutgoingNodes().size();
  int expLength = outgoingStmtNos.size();
  if (length != expLength) {
    std::cout << "OUTGOING EMPTY\n";
    return false;
  }

  for (std::shared_ptr<CFGNode> currNode : node->getOutgoingNodes()) {
    length--;
    std::string nodeStmtNo =
        std::to_string(currNode->getNode()->getStmtIndex());

    // std::cout << nodeStmtNo + "\n";

    bool removeResult =
        std::remove(outgoingStmtNos.begin(), outgoingStmtNos.end(),
                    nodeStmtNo) == outgoingStmtNos.end();

    if (removeResult) {
      // couldn't find the item
      std::cout << "INCORRECTLY CONTAINS " + nodeStmtNo + "\n";
      return false;
    }
  }
  return (length == 0);
  // if (length == 0) {
  //   return true;
  // } else {
  //   std::cout << "SHOULD CONTAIN BUT DOESN'T: ";
  //   for (std::string rem : outgoingStmtNos) {
  //     std::cout << rem + ", ";
  //   }
  //   return false;
  // }
}

bool PKBSPStub::checkCFGNodeIncoming(std::string statement_num,
                                     std::vector<std::string> incomingStmtNos) {
  bool result = true;
  std::shared_ptr<CFGNode> node = CFGNodeMap.find(statement_num)->second;
  int length = node->getIncomingNodes().size();
  int expLength = incomingStmtNos.size();

  //std::cout << "DISPLAYING STUB NODE MAP "
  //             "------------------------------------\n";
  //for (auto kv : CFGNodeMap) {
  //  std::cout << "NODE " + kv.first + " contains: \n";

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
  //}

  //std::cout
  //    << "DISPLAYING STUB NODE MAP ------------------------------------\n";

  //std::cout << std::to_string(node->getNode()->getStmtIndex()) +
  //                 " has incoming nodes: ";
  //for (std::shared_ptr<CFGNode> incomingNode : node->getIncomingNodes()) {
  //  std::cout << std::to_string(incomingNode->getNode()->getStmtIndex()) + ", ";
  //}
  //std::cout << "\n";
  //std::cout << "LENGTH: " + std::to_string(node->getIncomingNodes().size()) +
  //                 "\n";

  if (length == 0 && expLength != 0) {
    std::cout << "INCOMING EMPTY\n";
    return false;
  }

  for (std::shared_ptr<CFGNode> currNode : node->getIncomingNodes()) {
    length--;
    std::string nodeStmtNo =
        std::to_string(currNode->getNode()->getStmtIndex());

    // std::cout << nodeStmtNo + "\n";

    bool removeResult =
        std::remove(incomingStmtNos.begin(), incomingStmtNos.end(),
                    nodeStmtNo) == incomingStmtNos.end();

    if (removeResult) {
      // couldn't find the item
      std::cout << "INCORRECTLY CONTAINS " + nodeStmtNo + "\n";
      return false;
    }
  }
  return (length == 0);
}