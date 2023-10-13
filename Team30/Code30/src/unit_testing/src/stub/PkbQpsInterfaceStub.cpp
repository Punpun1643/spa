#include "PkbQpsInterfaceStub.h"

PkbQpsInterfaceStub::PkbQpsInterfaceStub(){};

std::unique_ptr<std::vector<std::string>>
PkbQpsInterfaceStub::getEntitiesWithType(EntityType type) {
  last_entity_type_passed = type;
  return std::make_unique<std::vector<std::string>>(getAllOfTypeValues);
}

std::unordered_set<std::string> PkbQpsInterfaceStub::convertEntityAttribute(
    std::string value, EntityType type, AttrType curr_attr_type,
    AttrType wanted_attr_type) {
  last_value_passed = value;
  last_entity_type_passed = type;
  last_attr_type_passed = curr_attr_type;
  last_attr_type_2_passed = wanted_attr_type;
  return {converted_entity};
};

bool PkbQpsInterfaceStub::doesEntityExist(EntityType type, AttrType attr_type,
                                          std::string value) {
  return false;
};

std::vector<std::pair<std::string, std::string>> PkbQpsInterfaceStub::getMatchingValues(
    EntityType type_1, AttrType attr_type_1, EntityType type_2,
    AttrType attr_type_2) {
  return {};
};

bool PkbQpsInterfaceStub::isRelationTrueValueValue(std::string value_1,
                                                   std::string value_2,
                                                   RelationType rel_type) {
  valueValueCalls++;
  last_rel_passed = rel_type;
  last_value_passed = value_1;
  last_value_2_passed = value_2;
  return valueValueBool;
}
bool PkbQpsInterfaceStub::isRelationTrueValueWild(std::string value,
                                                  RelationType rel_type) {
  valueWildCalls++;
  last_rel_passed = rel_type;
  last_value_passed = value;
  return valueWildBool;
}
bool PkbQpsInterfaceStub::isRelationTrueWildValue(std::string value,
                                                  RelationType rel_type) {
  wildValueCalls++;
  last_rel_passed = rel_type;
  last_value_passed = value;
  return wildValueBool;
}
bool PkbQpsInterfaceStub::isRelationTrueWildWild(RelationType rel_type) {
  wildWildCalls++;
  last_rel_passed = rel_type;
  return wildWildBool;
}

std::unique_ptr<std::vector<std::string>>
PkbQpsInterfaceStub::getRelationSynonymWild(EntityType entity_type,
                                            RelationType rel_type) {
  synonymWildCalls++;
  last_entity_type_passed = entity_type;
  last_rel_passed = rel_type;
  return std::make_unique<std::vector<std::string>>(synonymWildValues);
}
std::unique_ptr<std::vector<std::string>>
PkbQpsInterfaceStub::getRelationWildSynonym(EntityType entity_type,
                                            RelationType rel_type) {
  wildSynonymCalls++;
  last_entity_type_passed = entity_type;
  last_rel_passed = rel_type;
  return std::make_unique<std::vector<std::string>>(wildSynonymValues);
}

std::unique_ptr<std::vector<std::string>>
PkbQpsInterfaceStub::getRelationSynonymValue(EntityType entity_type,
                                             std::string value,
                                             RelationType rel_type) {
  synonymValueCalls++;
  last_entity_type_passed = entity_type;
  last_value_passed = value;
  last_rel_passed = rel_type;
  return std::make_unique<std::vector<std::string>>(synonymValueValues);
}
std::unique_ptr<std::vector<std::string>>
PkbQpsInterfaceStub::getRelationValueSynonym(std::string value,
                                             EntityType entity_type,
                                             RelationType rel_type) {
  valueSynonymCalls++;
  last_entity_type_passed = entity_type;
  last_value_passed = value;
  last_rel_passed = rel_type;
  return std::make_unique<std::vector<std::string>>(valueSynonymValues);
}

std::unique_ptr<std::vector<std::pair<std::string, std::string>>>
PkbQpsInterfaceStub::getRelationSynonymSynonym(EntityType entity_type_1,
                                               EntityType entity_type_2,
                                               RelationType rel_type) {
  synonymSynonymCalls++;
  last_entity_type_passed = entity_type_1;
  last_entity_type_2_passed = entity_type_2;
  last_rel_passed = rel_type;
  return std::make_unique<std::vector<std::pair<std::string, std::string>>>(
      synonymSynonymValues);
}

std::unique_ptr<std::vector<std::string>>
PkbQpsInterfaceStub::getPatternMatchesWildLhs(std::string rhs_expr,
                                              MatchType expr_match_type) {
  assert(false);
  return nullptr;
}

std::unique_ptr<std::vector<std::string>>
PkbQpsInterfaceStub::getPatternMatchesValueLhs(std::string lhs_value,
                                               std::string rhs_expr,
                                               MatchType expr_match_type) {
  assert(false);
  return nullptr;
};

// 2 paired values - for the implicit assign declaration, and the values for the
// given lhs_entity_type
std::unique_ptr<std::vector<std::pair<std::string, std::string>>>
PkbQpsInterfaceStub::getPatternMatchesSynonymLhs(std::string rhs_expr,
                                                 MatchType expr_match_type) {
  assert(false);
  return nullptr;
};

std::unique_ptr<std::vector<std::string>>
PkbQpsInterfaceStub::getPatternMatchesWildLhs(
    std::shared_ptr<TreeNode> rhs_expr, MatchType match_type) {
  patternWildCalls++;
  last_match_type_passed = match_type;
  last_rhs_expr_passed = rhs_expr;
  return std::make_unique<std::vector<std::string>>(patternWildValues);
};

std::unique_ptr<std::vector<std::string>>
PkbQpsInterfaceStub::getPatternMatchesValueLhs(
    std::string lhs_value, std::shared_ptr<TreeNode> rhs_expr,
    MatchType match_type) {
  patternValueCalls++;
  last_match_type_passed = match_type;
  last_rhs_expr_passed = rhs_expr;
  last_value_passed = lhs_value;
  return std::make_unique<std::vector<std::string>>(patternValueValues);
};

std::unique_ptr<std::vector<std::pair<std::string, std::string>>>
PkbQpsInterfaceStub::getPatternMatchesSynonymLhs(
    std::shared_ptr<TreeNode> rhs_expr, MatchType match_type) {
  patternDeclCalls++;
  last_match_type_passed = match_type;
  last_rhs_expr_passed = rhs_expr;
  return std::make_unique<std::vector<std::pair<std::string, std::string>>>(
      patternDeclValues);
};