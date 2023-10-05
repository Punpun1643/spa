#include "PKB.h"

#include <algorithm>
#include <cassert>
#include <string>
#include <vector>

PKB::PKB() : PKBQPSInterface(), PkbApi() {
  wildCardMatcher = WildCardMatcher();
  entData = std::make_unique<EntityDatabase>();
  relData = std::make_unique<RelDatabase>();
  patData = std::make_unique<PatternDatabase>();

  relatedTables = {
      {RelationType::FOLLOWS, {RelationType::FOLLOWS_STAR}},
      {RelationType::PARENT, {RelationType::PARENT_STAR}},
  };
};

// ********** Private methods **********
std::unordered_set<std::string> PKB::getRelated(RelationType rel_type,
                                                EntityType ent_type1,
                                                EntityType ent_type2) {
  std::unordered_set<std::string> output;
  std::shared_ptr<BaseTable> table = relData->getTable(rel_type);
  for (std::string e1 : *entData->get(ent_type1)) {
    for (std::string e2 : *entData->get(ent_type2)) {
      if (table->isRelated(e1, e2)) {
        output.insert(e2);
      };
    };
  };
  return output;
};

std::unordered_set<std::string> PKB::getRelated(RelationType rel_type,
                                                std::string value,
                                                EntityType ent_type) {
  std::unordered_set<std::string> output;
  std::shared_ptr<BaseTable> table = relData->getTable(rel_type);
  for (std::string e : *entData->get(ent_type)) {
    if (table->isRelated(value, e)) {
      output.insert(e);
    };
  };
  return output;
};

// ********** SP **********
void PKB::insertEntity(EntityType type, std::string entity) {
  entData->insert(type, entity);
};

void PKB::insertRelation(RelationType type, std::string input1,
                         std::string input2) {
  relData->getTable(type)->insert(input1, input2);
  for (RelationType rt : relatedTables[type]) {
    std::shared_ptr<BaseTable> table = relData->getTable(rt);
    table->insert(input1, input2);
  }
};

void PKB::insertPattern(std::string statement_number, std::string lhs,
                        std::unordered_set<std::string> rhs) {
  patData->insert(statement_number, lhs, rhs);
};

std::unordered_set<std::string> PKB::getProcedureModifies(
    std::string procName) {
  return getRelated(RelationType::MODIFIES_P, procName, EntityType::VARIABLE);
}

std::unordered_set<std::string> PKB::getProcedureUses(std::string procName) {
  return getRelated(RelationType::USES_P, procName, EntityType::VARIABLE);
};

// ********** QPS **********
// ---------- ENTITIES ----------
std::unique_ptr<std::vector<std::string>> PKB::getEntitiesWithType(
    EntityType type) {
  std::shared_ptr<std::unordered_set<std::string>> e = entData->get(type);
  return std::make_unique<std::vector<std::string>>(e->begin(), e->end());
};

// ---------- RELATIONS ----------
// 0 Declarations
// example Follows(1, 3)
bool PKB::isRelationTrue(std::string value_1, std::string value_2,
                         RelationType rel_type) {
  return relData->getTable(rel_type)->isRelated(value_1, value_2);
};

// example Follows(1, _)
bool PKB::isRelationTrueGivenFirstValue(std::string value,
                                        RelationType rel_type) {
  std::shared_ptr<std::unordered_set<std::string>> ents =
      entData->get(wildCardMatcher.translateRHSWild(rel_type));
  std::shared_ptr<BaseTable> table = relData->getTable(rel_type);

  // TODO: Optimise
  for (std::string ent : *ents) {
    if (table->isRelated(value, ent)) {
      return true;
    }
  }
  return false;
}

// example Follows(_, 1)
bool PKB::isRelationTrueGivenSecondValue(std::string value,
                                         RelationType rel_type) {
  std::shared_ptr<std::unordered_set<std::string>> ents =
      entData->get(wildCardMatcher.translateLHSWild(rel_type));
  std::shared_ptr<BaseTable> table = relData->getTable(rel_type);

  // TODO: Optimise
  for (std::string ent : *ents) {
    if (table->isRelated(ent, value)) {
      return true;
    }
  }
  return false;
}

// example Follows(_, _)
bool PKB::isRelationTrueForAny(RelationType relation_type) {
  return !relData->getTable(relation_type)->isEmpty();
}

// 1 Declarations
// example Parent(s, _), ParentsStar(s, _)
std::unique_ptr<std::vector<std::string>> PKB::getRelationValuesGivenFirstType(
    EntityType entity_type, RelationType rel_type) {
  std::unordered_set<std::string> output;
  std::shared_ptr<BaseTable> table = relData->getTable(rel_type);
  std::shared_ptr<std::unordered_set<std::string>> ents1 =
      entData->get(entity_type);
  std::shared_ptr<std::unordered_set<std::string>> ents2 =
      entData->get(wildCardMatcher.translateRHSWild(rel_type));

  // TODO: Optimise
  for (std::string ent1 : *ents1) {
    for (std::string ent2 : *ents2) {
      if (table->isRelated(ent1, ent2)) {
        output.insert(ent1);
      }
    }
  }
  return std::make_unique<std::vector<std::string>>(output.begin(),
                                                    output.end());
}

// example Follows(_, s), FolowsStar(_, s)
std::unique_ptr<std::vector<std::string>> PKB::getRelationValuesGivenSecondType(
    EntityType entity_type, RelationType rel_type) {
  std::unordered_set<std::string> output;
  std::shared_ptr<BaseTable> table = relData->getTable(rel_type);
  std::shared_ptr<std::unordered_set<std::string>> ents1 =
      entData->get(wildCardMatcher.translateLHSWild(rel_type));
  std::shared_ptr<std::unordered_set<std::string>> ents2 =
      entData->get(entity_type);

  // TODO: Optimise
  for (std::string ent1 : *ents1) {
    for (std::string ent2 : *ents2) {
      if (table->isRelated(ent1, ent2)) {
        output.insert(ent2);
      }
    }
  }
  return std::make_unique<std::vector<std::string>>(output.begin(),
                                                    output.end());
};

// example Follows(s, 3), FolowsStar(s, 3)
std::unique_ptr<std::vector<std::string>> PKB::getRelationValues(
    EntityType entity_type, std::string value, RelationType rel_type) {
  std::unordered_set<std::string> output;
  std::shared_ptr<std::unordered_set<std::string>> ents =
      entData->get(entity_type);
  std::shared_ptr<BaseTable> table = relData->getTable(rel_type);
  for (std::string ent : *ents) {
    if (table->isRelated(ent, value)) {
      output.insert(ent);
    }
  }
  return std::make_unique<std::vector<std::string>>(output.begin(),
                                                    output.end());
}

// example Follows(3, s)
std::unique_ptr<std::vector<std::string>> PKB::getRelationValues(
    std::string value, EntityType entity_type, RelationType rel_type) {
  std::unordered_set<std::string> output;
  std::shared_ptr<std::unordered_set<std::string>> ents =
      entData->get(entity_type);
  std::shared_ptr<BaseTable> table = relData->getTable(rel_type);
  for (std::string ent : *ents) {
    if (table->isRelated(value, ent)) {
      output.insert(ent);
    }
  }
  return std::make_unique<std::vector<std::string>>(output.begin(),
                                                    output.end());
}

//// 2 Declarations
// example Follows(s1, s2), FollowsStar(s1, s2)
std::unique_ptr<std::vector<std::pair<std::string, std::string>>>
PKB::getRelationValues(EntityType entity_type_1, EntityType entity_type_2,
                       RelationType rel_type) {
  std::vector<std::pair<std::string, std::string>> output;
  std::shared_ptr<std::unordered_set<std::string>> ents1 =
      PKB::entData->get(entity_type_1);
  std::shared_ptr<std::unordered_set<std::string>> ents2 =
      entData->get(entity_type_2);
  std::shared_ptr<BaseTable> table = relData->getTable(rel_type);
  for (std::string ent1 : *ents1) {
    for (std::string ent2 : *ents2) {
      if (table->isRelated(ent1, ent2)) {
        output.push_back(make_pair(ent1, ent2));
      }
    }
  }
  return std::make_unique<std::vector<std::pair<std::string, std::string>>>(
      output);
};

// ---------- PATTERNS ----------
std::unique_ptr<std::vector<std::string>> PKB::getPatternMatchesWithWildLhs(
    std::string rhs_expr, MatchType expr_match_type) {
  // Wild match: pattern a(_, _), i.e. all statement numbers
  if (expr_match_type == MatchType::WILD_MATCH) {
    std::unordered_set<std::string> assignment_statements =
        *entData->get(EntityType::ASSIGN);
    return std::make_unique<std::vector<std::string>>(
        assignment_statements.begin(), assignment_statements.end());
  }

  // Partial match: pattern a(_, "x")
  // only works for single variables or constants
  std::unordered_set<std::string> rhs_statements =
      patData->getStatementNumbersGivenRHS(rhs_expr);
  return std::make_unique<std::vector<std::string>>(rhs_statements.begin(),
                                                    rhs_statements.end());
};

std::unique_ptr<std::vector<std::string>> PKB::getPatternMatchesWithLhsValue(
    std::string lhs_value, std::string rhs_expr, MatchType expr_match_type) {
  std::unordered_set<std::string> lhs_statements =
      patData->getStatementNumbersGivenLHS(lhs_value);

  // Wild match: pattern a("x", _)
  if (expr_match_type == MatchType::WILD_MATCH) {
    return std::make_unique<std::vector<std::string>>(lhs_statements.begin(),
                                                      lhs_statements.end());
  };

  // Partial match: pattern a("x", "_y_")
  // only works for single variables or constants
  std::unordered_set<std::string> rhs_statements =
      patData->getStatementNumbersGivenRHS(rhs_expr);
  std::unordered_set<std::string> intersection;
  for (std::string val : rhs_statements) {
    if (lhs_statements.find(val) != lhs_statements.end()) {
      intersection.insert(val);
    }
  }

  return std::make_unique<std::vector<std::string>>(intersection.begin(),
                                                    intersection.end());
};

// return possible values of the LHS synonym
std::unique_ptr<std::vector<std::pair<std::string, std::string>>>
PKB::getPatternMatchesWithDeclarationLhs(std::string rhs_expr,
                                         MatchType expr_match_type) {
  std::unordered_set<std::string> statements;
  if (expr_match_type == MatchType::WILD_MATCH) {
    statements = *entData->get(EntityType::ASSIGN);
  } else {
    statements = patData->getStatementNumbersGivenRHS(rhs_expr);
  }

  std::vector<std::pair<std::string, std::string>> output;
  for (std::string st_num : statements) {
    output.push_back(
        make_pair(st_num, patData->getVarGivenStatementNum(st_num)));
  }

  return std::make_unique<std::vector<std::pair<std::string, std::string>>>(
      output.begin(), output.end());
};