#include "PkbStub.h"

#include <iostream>

PkbStub::PkbStub()
    : insertEntityCallCount(0),
      insertVariableCallCount(0),
      insertConstantCallCount(0),
      insertRelationCallCount(0),
      insertFollowsCallCount(0),
      insertParentCallCount(0),
      insertUsesCallCount(0),
      insertModifiesCallCount(0),
      insertPatternCallCount(0){};

void PkbStub::insertEntity(EntityType type, std::string entity) {
  if (type == EntityType::CONSTANT) {
    insertConstantCallCount++;
  } else if (type == EntityType::VARIABLE) {
    insertVariableCallCount++;
  } else {
    insertEntityCallCount++;
  }
  // std::cout << "(" + std::to_string(type) + ", " + entity + ")\n";
}

void PkbStub::insertRelationCommon(RelationType type) {
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
    default:
      break;
  }
}

// Relation (integer, integer)
void PkbStub::insertRelation(RelationType rel_type, std::string s1_line_num,
                             std::string s2_line_num) {
  insertRelationCommon(rel_type);
}

// Pattern clause
void PkbStub::insertPattern(std::string statement_number, std::string lhs,
                            std::unordered_set<std::string> rhs) {
  insertPatternCallCount++;
};

std::unique_ptr<std::vector<std::string>> PkbStub::getEntitiesWithType(
    EntityType type) {
  std::unique_ptr<std::vector<std::string>> output =
      std::make_unique<std::vector<std::string>>();

  if (type == EntityType::PROCEDURE) {
    *output = PROCEDURES;
  } else if (type == EntityType::CONSTANT) {
    *output = CONSTANTS;
  } else if (type == EntityType::VARIABLE) {
    *output = VARIABLES;
  } else {  // statement type
    *output = STATEMENTS;
  }
  return output;
}

bool PkbStub::isRelationTrueValueValue(std::string value_1, std::string value_2,
                                       RelationType rel_type) {
  return true;
}
bool PkbStub::isRelationTrueValueWild(std::string value,
                                      RelationType rel_type) {
  return false;
}
bool PkbStub::isRelationTrueWildValue(std::string value,
                                      RelationType rel_type) {
  return true;
}
bool PkbStub::isRelationTrueWildWild(RelationType relation_type) {
  return false;
}

std::unique_ptr<std::vector<std::string>> PkbStub::getRelationSynonymWild(
    EntityType entity_type, RelationType rel_type) {
  return std::make_unique<std::vector<std::string>>();  // empty
}
std::unique_ptr<std::vector<std::string>> PkbStub::getRelationWildSynonym(
    EntityType entity_type, RelationType rel_type) {
  std::vector<std::string> vec = {"1", "3", "5", "7", "9"};
  return std::make_unique<std::vector<std::string>>(vec);
}

std::unique_ptr<std::vector<std::string>> PkbStub::getRelationSynonymValue(
    EntityType entity_type, std::string value, RelationType rel_type) {
  std::vector<std::string> vec = {"2", "4", "6", "8", "10"};
  return std::make_unique<std::vector<std::string>>(vec);
}
std::unique_ptr<std::vector<std::string>> PkbStub::getRelationValueSynonym(
    std::string value, EntityType entity_type, RelationType rel_type) {
  return std::make_unique<std::vector<std::string>>();  // empty
}

std::unique_ptr<std::vector<std::pair<std::string, std::string>>>
PkbStub::getRelationSynonymSynonym(EntityType entity_type_1,
                                   EntityType entity_type_2,
                                   RelationType rel_type) {
  if (entity_type_1 == EntityType::STMT && entity_type_2 == EntityType::STMT) {
    auto result =
        std::make_unique<std::vector<std::pair<std::string, std::string>>>();
    result->push_back(std::make_pair("5", "10"));
    result->push_back(std::make_pair("9", "1"));
    result->push_back(std::make_pair("2", "2"));
    result->push_back(std::make_pair("2", "1"));
    return result;
  } else {
    return std::make_unique<std::vector<std::pair<std::string, std::string>>>();
  }
}

std::unique_ptr<std::vector<std::string>> PkbStub::getPatternMatchesWildLhs(
    std::string rhs_expr, MatchType expr_match_type) {
  if (expr_match_type == MatchType::WILD_MATCH) {
    auto return_vec = std::vector<std::string>({"1"});
    return std::make_unique<std::vector<std::string>>(return_vec);
  } else if (expr_match_type == MatchType::PARTIAL_MATCH) {
    auto return_vec = std::vector<std::string>({"2"});
    return std::make_unique<std::vector<std::string>>(return_vec);
  } else {  // exact
    return std::make_unique<std::vector<std::string>>();
  }
}

std::unique_ptr<std::vector<std::string>> PkbStub::getPatternMatchesValueLhs(
    std::string lhs_value, std::string rhs_expr, MatchType expr_match_type) {
  if (expr_match_type == MatchType::WILD_MATCH) {
    auto return_vec = std::vector<std::string>({"3"});
    return std::make_unique<std::vector<std::string>>(return_vec);
  } else if (expr_match_type == MatchType::PARTIAL_MATCH) {
    auto return_vec = std::vector<std::string>({"4"});
    return std::make_unique<std::vector<std::string>>(return_vec);
  } else {
    return std::make_unique<std::vector<std::string>>();
  }
};

// 2 paired values - for the implicit assign declaration, and the values for the
// given lhs_entity_type
std::unique_ptr<std::vector<std::pair<std::string, std::string>>>
PkbStub::getPatternMatchesSynonymLhs(std::string rhs_expr,
                                     MatchType expr_match_type) {
  auto output =
      std::make_unique<std::vector<std::pair<std::string, std::string>>>();

  if (expr_match_type == MatchType::WILD_MATCH) {
    output->push_back(std::make_pair("1", "varX"));

  } else if (expr_match_type == MatchType::PARTIAL_MATCH) {
    output->push_back(std::make_pair("2", "varY"));
  }
  return output;
};

std::unordered_set<std::string> PkbStub::getProcedureModifies(
    std::string procName) {
  return std::unordered_set<std::string>();
}

std::unordered_set<std::string> PkbStub::getProcedureUses(
    std::string procName) {
  return std::unordered_set<std::string>();
};

void PkbStub::insertPattern(PatternType type, std::string statement_number,
                            std::string lhs, std::shared_ptr<TreeNode> rhs) {
  insertPatternCallCount++;
};

std::unique_ptr<std::vector<std::string>> PkbStub::getPatternMatchesWildLhs(
    std::shared_ptr<TreeNode> rhs_expr, MatchType match_type) {
  return NULL;
};
std::unique_ptr<std::vector<std::string>> PkbStub::getPatternMatchesValueLhs(
    std::string lhs_value, std::shared_ptr<TreeNode> rhs_expr,
    MatchType match_type) {
  return NULL;
};
std::unique_ptr<std::vector<std::pair<std::string, std::string>>>
PkbStub::getPatternMatchesSynonymLhs(std::shared_ptr<TreeNode> rhs_expr,
                                     MatchType match_type) {
  return NULL;
};
