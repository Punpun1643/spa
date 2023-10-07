#include "PKB.h"

#include <algorithm>
#include <cassert>
#include <string>
#include <vector>

PKB::PKB() : PKBQPSInterface(), PKBSPInterface() {
  entData = std::make_unique<EntityDatabase>();
  relData = std::make_unique<RelDatabase>();
  patData = std::make_unique<PatternDatabase>();

  relatedTables = {
      {RelationType::FOLLOWS, {RelationType::FOLLOWS_STAR}},
      {RelationType::PARENT, {RelationType::PARENT_STAR}},
  };
};

// ********** Private methods **********
std::unordered_set<std::string> PKB::getAllRelated(
    RelationType rel_type, std::shared_ptr<std::unordered_set<std::string>> set,
    std::string value) {
  std::unordered_set<std::string> output;
  for (std::string e : *set) {
    if (relData->isRelated(rel_type, e, value)) {
      output.insert(e);
    };
  };
  return output;
};

std::unordered_set<std::string> PKB::getAllRelated(
    RelationType rel_type, std::string value,
    std::shared_ptr<std::unordered_set<std::string>> set) {
  std::unordered_set<std::string> output;
  for (std::string e : *set) {
    if (relData->isRelated(rel_type, value, e)) {
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
  relData->insert(type, input1, input2);
  for (RelationType rt : relatedTables[type]) {
    relData->insert(rt, input1, input2);
  }
};

void PKB::insertPattern(std::string statement_number, std::string lhs,
                        std::unordered_set<std::string> rhs) {
  patData->insert(statement_number, lhs, rhs);
};

std::unordered_set<std::string> PKB::getProcedureModifies(
    std::string procName) {
  std::shared_ptr<std::unordered_set<std::string>> input;
  input->insert(procName);
  return relData->getAllRelated(RelationType::MODIFIES_P, input);
};

std::unordered_set<std::string> PKB::getProcedureUses(std::string procName) {
  std::shared_ptr<std::unordered_set<std::string>> input;
  input->insert(procName);
  return relData->getAllRelated(RelationType::USES_P, input);
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
bool PKB::isRelationTrueValueValue(std::string value_1, std::string value_2,
                                   RelationType rel_type) {
  return relData->isRelated(rel_type, value_1, value_2);
};

// example Follows(1, _)
bool PKB::isRelationTrueValueWild(std::string value, RelationType rel_type) {
  return relData->hasRelations(rel_type, value);
}

// example Follows(_, 1)
bool PKB::isRelationTrueWildValue(std::string value, RelationType rel_type) {
  return relData->hasInverseRelations(rel_type, value);
}

// example Follows(_, _)
bool PKB::isRelationTrueWildWild(RelationType relation_type) {
  return !relData->isEmpty(relation_type);
}

// 1 Declarations
// example Parent(s, _), ParentsStar(s, _)
std::unique_ptr<std::vector<std::string>> PKB::getRelationSynonymWild(
    EntityType entity_type, RelationType rel_type) {
  std::unordered_set<std::string> output =
      relData->getAllRelated(rel_type, entData->get(entity_type));
  return std::make_unique<std::vector<std::string>>(output.begin(),
                                                    output.end());
}

// example Follows(_, s), FolowsStar(_, s)
std::unique_ptr<std::vector<std::string>> PKB::getRelationWildSynonym(
    EntityType entity_type, RelationType rel_type) {
  std::unordered_set<std::string> output =
      relData->getAllInverseRelated(rel_type, entData->get(entity_type));
  return std::make_unique<std::vector<std::string>>(output.begin(),
                                                    output.end());
};

// example Follows(s, 3), FolowsStar(s, 3)
std::unique_ptr<std::vector<std::string>> PKB::getRelationSynonymValue(
    EntityType entity_type, std::string value, RelationType rel_type) {
  std::unordered_set<std::string> output =
      getAllRelated(rel_type, entData->get(entity_type), value);
  return std::make_unique<std::vector<std::string>>(output.begin(),
                                                    output.end());
}

// example Follows(3, s)
std::unique_ptr<std::vector<std::string>> PKB::getRelationValueSynonym(
    std::string value, EntityType entity_type, RelationType rel_type) {
  std::unordered_set<std::string> output =
      getAllRelated(rel_type, value, entData->get(entity_type));
  return std::make_unique<std::vector<std::string>>(output.begin(),
                                                    output.end());
}

//// 2 Declarations
// example Follows(s1, s2), FollowsStar(s1, s2)
std::unique_ptr<std::vector<std::pair<std::string, std::string>>>
PKB::getRelationSynonymSynonym(EntityType entity_type_1,
                               EntityType entity_type_2,
                               RelationType rel_type) {
  std::vector<std::pair<std::string, std::string>> output;
  std::shared_ptr<std::unordered_set<std::string>> ents1 =
      PKB::entData->get(entity_type_1);
  std::shared_ptr<std::unordered_set<std::string>> ents2 =
      entData->get(entity_type_2);

  for (std::string ent1 : *ents1) {
    for (std::string ent2 : *ents2) {
      if (relData->isRelated(rel_type, ent1, ent2)) {
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