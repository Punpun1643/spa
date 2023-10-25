#include "PkbQpsInterfaceStub.h"

PkbQpsInterfaceStub::PkbQpsInterfaceStub(){};

std::vector<std::string> PkbQpsInterfaceStub::GetEntitiesWithType(
    EntityType type) {
  last_entity_type_passed = type;
  return std::vector<std::string>(getAllOfTypeValues);
}

std::string PkbQpsInterfaceStub::ConvertEntityValueToAlias(
    std::string value, EntityType type, AttrType curr_attr_type,
    AttrType wanted_attr_type) {
  last_value_passed = value;
  last_entity_type_passed = type;
  last_attr_type_passed = curr_attr_type;
  last_attr_type_2_passed = wanted_attr_type;
  return converted_entity;
};

std::vector<std::string> PkbQpsInterfaceStub::GetEntitiesMatchingAttrValue(
    EntityType type, AttrType attr_type, std::string value) {
  last_entity_type_passed = type;
  last_attr_type_passed = attr_type;
  last_value_passed = value;
  return attr_value_matches;
};

std::vector<std::pair<std::string, std::string>>
PkbQpsInterfaceStub::GetEntitiesWhereAttributesMatch(EntityType type_1,
                                                     AttrType attr_type_1,
                                                     EntityType type_2,
                                                     AttrType attr_type_2) {
  last_entity_type_passed = type_1;
  last_attr_type_passed = attr_type_1;
  last_entity_type_2_passed = type_2;
  last_attr_type_2_passed = attr_type_2;
  return attr_pair_matches;
};

bool PkbQpsInterfaceStub::IsRelationTrueValueValue(std::string value_1,
                                                   std::string value_2,
                                                   RelationType rel_type) {
  valueValueCalls++;
  last_rel_passed = rel_type;
  last_value_passed = value_1;
  last_value_2_passed = value_2;
  return valueValueBool;
}
bool PkbQpsInterfaceStub::IsRelationTrueValueWild(std::string value,
                                                  RelationType rel_type) {
  valueWildCalls++;
  last_rel_passed = rel_type;
  last_value_passed = value;
  return valueWildBool;
}
bool PkbQpsInterfaceStub::IsRelationTrueWildValue(std::string value,
                                                  RelationType rel_type) {
  wildValueCalls++;
  last_rel_passed = rel_type;
  last_value_passed = value;
  return wildValueBool;
}
bool PkbQpsInterfaceStub::IsRelationTrueWildWild(RelationType rel_type) {
  wildWildCalls++;
  last_rel_passed = rel_type;
  return wildWildBool;
}

std::vector<std::string> PkbQpsInterfaceStub::GetRelationSynonymWild(
    EntityType entity_type, RelationType rel_type) {
  synonymWildCalls++;
  last_entity_type_passed = entity_type;
  last_rel_passed = rel_type;
  return std::vector<std::string>(synonymWildValues);
}
std::vector<std::string> PkbQpsInterfaceStub::GetRelationWildSynonym(
    EntityType entity_type, RelationType rel_type) {
  wildSynonymCalls++;
  last_entity_type_passed = entity_type;
  last_rel_passed = rel_type;
  return std::vector<std::string>(wildSynonymValues);
}

std::vector<std::string> PkbQpsInterfaceStub::GetRelationSynonymValue(
    EntityType entity_type, std::string value, RelationType rel_type) {
  synonymValueCalls++;
  last_entity_type_passed = entity_type;
  last_value_passed = value;
  last_rel_passed = rel_type;
  return std::vector<std::string>(synonymValueValues);
}
std::vector<std::string> PkbQpsInterfaceStub::GetRelationValueSynonym(
    std::string value, EntityType entity_type, RelationType rel_type) {
  valueSynonymCalls++;
  last_entity_type_passed = entity_type;
  last_value_passed = value;
  last_rel_passed = rel_type;
  return std::vector<std::string>(valueSynonymValues);
}

std::vector<std::pair<std::string, std::string>>
PkbQpsInterfaceStub::GetRelationSynonymSynonym(EntityType entity_type_1,
                                               EntityType entity_type_2,
                                               RelationType rel_type) {
  synonymSynonymCalls++;
  last_entity_type_passed = entity_type_1;
  last_entity_type_2_passed = entity_type_2;
  last_rel_passed = rel_type;
  return std::vector<std::pair<std::string, std::string>>(synonymSynonymValues);
}

std::vector<std::string> PkbQpsInterfaceStub::GetMatchingAssignStmts(
    std::shared_ptr<TreeNode> rhs_expr, MatchType match_type) {
  patternWildCalls++;
  last_match_type_passed = match_type;
  last_rhs_expr_passed = rhs_expr;
  return std::vector<std::string>(patternWildValues);
};

std::vector<std::string> PkbQpsInterfaceStub::GetMatchingAssignStmts(
    std::string lhs_value, std::shared_ptr<TreeNode> rhs_expr,
    MatchType match_type) {
  patternValueCalls++;
  last_match_type_passed = match_type;
  last_rhs_expr_passed = rhs_expr;
  last_value_passed = lhs_value;
  return std::vector<std::string>(patternValueValues);
};

std::vector<std::pair<std::string, std::string>>
PkbQpsInterfaceStub::GetMatchingAssignStmtLhsVarPairs(
    std::shared_ptr<TreeNode> rhs_expr, MatchType match_type) {
  patternDeclCalls++;
  last_match_type_passed = match_type;
  last_rhs_expr_passed = rhs_expr;
  return std::vector<std::pair<std::string, std::string>>(patternDeclValues);
};

std::vector<std::string> PkbQpsInterfaceStub::GetContainerStmtsWithControlVar(
    EntityType container_stmt_type) {
  return {};
}

std::vector<std::string>
PkbQpsInterfaceStub::GetContainerStmtsWithGivenControlVar(
    EntityType container_stmt_type, std::string var_name) {
  return {};
}

std::vector<std::pair<std::string, std::string>>
PkbQpsInterfaceStub::GetContainerStmtControlVarPairs(
    EntityType container_stmt_type) {
  return {};
}
