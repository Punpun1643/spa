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

void PKBSPStub::InsertEntity(EntityType type, std::string entity) {
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

void PKBSPStub::InsertEntity(EntityType type, AttrType attr_type,
                             std::string statement_number,
                             std::string attribute) {
  InsertEntity(type, statement_number);
};

void PKBSPStub::InsertRelationCommon(RelationType type, std::string a,
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
void PKBSPStub::InsertRelation(RelationType rel_type, std::string s1_line_num,
                               std::string s2_line_num) {
  InsertRelationCommon(rel_type, s1_line_num, s2_line_num);
}

// Assign Pattern clause
void PKBSPStub::InsertAssignPattern(std::string statement_number,
                                    std::string lhs,
                                    std::shared_ptr<TreeNode> rhs) {
  insertPatternCallCount++;
};

// If/While Pattern clause
void PKBSPStub::InsertCondVarPattern(EntityType type,
                                     std::string statement_number,
                                     std::string var){};

std::unordered_set<std::string> PKBSPStub::GetProcedureUses(
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

std::unordered_set<std::string> PKBSPStub::GetProcedureModifies(
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

std::unordered_set<std::string> PKBSPStub::GetStatementModifies(
    std::string stmt) {
  std::unordered_set<std::string> result;
  return result;
}

std::unordered_set<std::string> PKBSPStub::GetStatementUses(std::string stmt) {
  std::unordered_set<std::string> result;
  return result;
}

void PKBSPStub::InsertCFGNode(std::string statement_num,
                              std::shared_ptr<CFGNode> node) {
  insertCFGCallCount++;
  CFGNodeMap.insert({statement_num, node});
};

bool PKBSPStub::checkCFGNodeOutgoing(std::string statement_num,
                                     std::vector<std::string> outgoingStmtNos) {
  std::shared_ptr<CFGNode> node = CFGNodeMap.find(statement_num)->second;
  int length = node->GetOutgoingNodes().size();
  int expLength = outgoingStmtNos.size();
  if (length == 0 && expLength != 0) {
    std::cout << "OUTGOING EMPTY\n";
    return false;
  }

  for (std::shared_ptr<CFGNode> currNode : node->GetOutgoingNodes()) {
    length--;
    std::string nodeStmtNo =
        std::to_string(currNode->GetNode()->GetStmtIndex());

    // std::cout << nodeStmtNo + "\n";

    bool removeResult =
        std::remove(outgoingStmtNos.begin(), outgoingStmtNos.end(),
                    nodeStmtNo) == outgoingStmtNos.end();

    if (removeResult) {
      // couldn't find the item
      std::cout << " - INCORRECTLY CONTAINS " + nodeStmtNo + "\n";
      return false;
    }
  }
  return (length == 0);
}

bool PKBSPStub::checkCFGNodeIncoming(std::string statement_num,
                                     std::vector<std::string> incomingStmtNos) {
  bool result = true;
  std::shared_ptr<CFGNode> node = CFGNodeMap.find(statement_num)->second;
  int length = node->GetIncomingNodes().size();
  int expLength = incomingStmtNos.size();

  if (length == 0 && expLength != 0) {
    std::cout << "INCOMING EMPTY\n";
    return false;
  }

  for (std::shared_ptr<CFGNode> currNode : node->GetIncomingNodes()) {
    length--;
    std::string nodeStmtNo =
        std::to_string(currNode->GetNode()->GetStmtIndex());

    // std::cout << nodeStmtNo + "\n";

    bool removeResult =
        std::remove(incomingStmtNos.begin(), incomingStmtNos.end(),
                    nodeStmtNo) == incomingStmtNos.end();

    if (removeResult) {
      // couldn't find the item
      std::cout << " -- INCORRECTLY CONTAINS " + nodeStmtNo + "\n";
      return false;
    }
  }
  return (length == 0);
}
