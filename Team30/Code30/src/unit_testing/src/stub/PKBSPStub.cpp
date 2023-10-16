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
      insertPatternCallCount(0){};

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

std::unordered_set<std::string> PKBSPStub::getStatementModifies(
    std::string stmt) {
  std::unordered_set<std::string> result;
  return result;
}

std::unordered_set<std::string> PKBSPStub::getStatementUses(std::string stmt) {
  std::unordered_set<std::string> result;
  return result;
}

void PKBSPStub::insertCFGNode(std::string statement_num,
                              std::shared_ptr<CFGNode> node){};