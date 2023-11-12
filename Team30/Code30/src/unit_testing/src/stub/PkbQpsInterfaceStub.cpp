#include "PkbQpsInterfaceStub.h"

PkbQpsInterfaceStub::PkbQpsInterfaceStub(){};

std::vector<std::string> PkbQpsInterfaceStub::GetEntitiesWithType(
    EntityType type) {
  last_entity_type_passed = type;
  return std::vector<std::string>(get_all_of_type_values);
}

std::string PkbQpsInterfaceStub::ConvertEntityValueToAlias(
    std::string const& value, EntityType type, AttrType wanted_attr_type) {
  last_value_passed = value;
  last_entity_type_passed = type;
  last_attr_type_2_passed = wanted_attr_type;
  return converted_entity;
};

std::vector<std::string> PkbQpsInterfaceStub::GetEntitiesMatchingAttrValue(
    EntityType type, AttrType attr_type, std::string const& value) {
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

bool PkbQpsInterfaceStub::IsRelationTrueValueValue(std::string const& value_1,
                                                   std::string const& value_2,
                                                   RelationType rel_type) {
  value_value_calls++;
  last_rel_passed = rel_type;
  last_value_passed = value_1;
  last_value_2_passed = value_2;
  return value_value_bool;
}
bool PkbQpsInterfaceStub::IsRelationTrueValueWild(std::string const& value,
                                                  RelationType rel_type) {
  value_wild_calls++;
  last_rel_passed = rel_type;
  last_value_passed = value;
  return value_wild_bool;
}
bool PkbQpsInterfaceStub::IsRelationTrueWildValue(std::string const& value,
                                                  RelationType rel_type) {
  wild_value_calls++;
  last_rel_passed = rel_type;
  last_value_passed = value;
  return wild_value_bool;
}
bool PkbQpsInterfaceStub::IsRelationTrueWildWild(RelationType rel_type) {
  wild_wild_calls++;
  last_rel_passed = rel_type;
  return wild_wild_bool;
}

std::vector<std::string> PkbQpsInterfaceStub::GetRelationSynonymWild(
    EntityType entity_type, RelationType rel_type) {
  synonym_wild_calls++;
  last_entity_type_passed = entity_type;
  last_rel_passed = rel_type;
  return std::vector<std::string>(synonym_wild_values);
}
std::vector<std::string> PkbQpsInterfaceStub::GetRelationWildSynonym(
    EntityType entity_type, RelationType rel_type) {
  wild_synonym_calls++;
  last_entity_type_passed = entity_type;
  last_rel_passed = rel_type;
  return std::vector<std::string>(wild_synonym_values);
}

std::vector<std::string> PkbQpsInterfaceStub::GetRelationSynonymValue(
    EntityType entity_type, std::string const& value, RelationType rel_type) {
  synonym_value_calls++;
  last_entity_type_passed = entity_type;
  last_value_passed = value;
  last_rel_passed = rel_type;
  return std::vector<std::string>(synonym_value_values);
}
std::vector<std::string> PkbQpsInterfaceStub::GetRelationValueSynonym(
    std::string const& value, EntityType entity_type, RelationType rel_type) {
  value_synonym_calls++;
  last_entity_type_passed = entity_type;
  last_value_passed = value;
  last_rel_passed = rel_type;
  return std::vector<std::string>(value_synonym_values);
}

std::vector<std::pair<std::string, std::string>>
PkbQpsInterfaceStub::GetRelationSynonymSynonym(EntityType entity_type_1,
                                               EntityType entity_type_2,
                                               RelationType rel_type) {
  synonym_synonym_calls++;
  last_entity_type_passed = entity_type_1;
  last_entity_type_2_passed = entity_type_2;
  last_rel_passed = rel_type;
  return std::vector<std::pair<std::string, std::string>>(
      synonym_synonym_values);
}

std::vector<std::string> PkbQpsInterfaceStub::GetMatchingAssignStmts(
    std::shared_ptr<TreeNode> const& rhs_expr, MatchType match_type) {
  pattern_assign_wild_calls++;
  last_match_type_passed = match_type;
  last_rhs_expr_passed = rhs_expr;
  return std::vector<std::string>(pattern_assign_wild_values);
};

std::vector<std::string> PkbQpsInterfaceStub::GetMatchingAssignStmts(
    std::string const& lhs_value, std::shared_ptr<TreeNode> const& rhs_expr,
    MatchType match_type) {
  pattern_assign_value_calls++;
  last_match_type_passed = match_type;
  last_rhs_expr_passed = rhs_expr;
  last_value_passed = lhs_value;
  return std::vector<std::string>(pattern_assign_value_values);
};

std::vector<std::pair<std::string, std::string>>
PkbQpsInterfaceStub::GetMatchingAssignStmtLhsVarPairs(
    std::shared_ptr<TreeNode> const& rhs_expr, MatchType match_type) {
  pattern_assign_decl_calls++;
  last_match_type_passed = match_type;
  last_rhs_expr_passed = rhs_expr;
  return std::vector<std::pair<std::string, std::string>>(
      pattern_assign_decl_values);
};

std::vector<std::string> PkbQpsInterfaceStub::GetContainerStmtsWithControlVar(
    EntityType container_stmt_type) {
  last_entity_type_passed = container_stmt_type;
  return pattern_container_wild_values;
}

std::vector<std::string>
PkbQpsInterfaceStub::GetContainerStmtsWithGivenControlVar(
    EntityType container_stmt_type, std::string const& var_name) {
  last_entity_type_passed = container_stmt_type;
  last_value_passed = var_name;
  return pattern_container_value_values;
}

std::vector<std::pair<std::string, std::string>>
PkbQpsInterfaceStub::GetContainerStmtControlVarPairs(
    EntityType container_stmt_type) {
  last_entity_type_passed = container_stmt_type;
  return pattern_container_decl_values;
}
