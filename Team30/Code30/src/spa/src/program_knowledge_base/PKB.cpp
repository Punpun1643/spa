#include "PKB.h"

#include <algorithm>
#include <cassert>
#include <string>
#include <unordered_set>
#include <vector>

PKB::PKB() : PKBQPSInterface(), PKBSPInterface() {
  ent_data = std::make_unique<EntityDatabase>();
  rel_data = std::make_unique<RelDatabase>();
  pat_data = std::make_unique<PatternDatabase>();
}

// ********** Private methods **********
std::unordered_set<std::string> PKB::GetIntersectionHelper(
    std::unordered_set<std::string> const& larger_set,
    std::unordered_set<std::string> const& smaller_set) {
  std::unordered_set<std::string> output;
  for (std::string v : smaller_set) {
    if (larger_set.count(v)) {
      output.insert(v);
    }
  }
  return output;
}

std::unordered_set<std::string> PKB::GetIntersection(
    std::unordered_set<std::string> const& set1,
    std::unordered_set<std::string> const& set2) {
  std::unordered_set<std::string> output;
  if (set1.size() < set2.size()) {
    return GetIntersectionHelper(set2, set1);
  }
  return GetIntersectionHelper(set1, set2);
}

// ********** SP **********
void PKB::InsertEntity(EntityType type, std::string const& value) {
  ent_data->InsertEntity(type, value);
}

void PKB::InsertEntity(EntityType type, AttrType attr_type,
                       std::string const& statement_number,
                       std::string const& attribute) {
  ent_data->InsertEntity(type, attr_type, statement_number, attribute);
}

void PKB::InsertRelation(RelationType type, std::string const& input1,
                         std::string const& input2) {
  rel_data->Insert(type, input1, input2);
}

void PKB::InsertAssignPattern(std::string const& statement_number,
                              std::string const& lhs,
                              std::shared_ptr<TreeNode> const& rhs) {
  pat_data->InsertAssignment(statement_number, lhs, rhs);
}

void PKB::InsertCondVarPattern(EntityType type,
                               std::string const& statement_number,
                               std::string const& var) {
  pat_data->InsertCondVar(type, statement_number, var);
}

void PKB::InsertCFGNode(std::string const& statement_num,
                        std::shared_ptr<CFGNode> const& node) {
  rel_data->InsertCFGNode(statement_num, node);
}

std::unordered_set<std::string> PKB::GetProcedureModifies(
    std::string const& proc_name) {
  return rel_data->GetAllRelatedToValue(RelationType::MODIFIES_P, proc_name);
}

std::unordered_set<std::string> PKB::GetProcedureUses(
    std::string const& proc_name) {
  return rel_data->GetAllRelatedToValue(RelationType::USES_P, proc_name);
}

std::unordered_set<std::string> PKB::GetStatementModifies(
    std::string const& stmt) {
  return rel_data->GetAllRelatedToValue(RelationType::MODIFIES_S, stmt);
}

std::unordered_set<std::string> PKB::GetStatementUses(std::string const& stmt) {
  return rel_data->GetAllRelatedToValue(RelationType::USES_S, stmt);
}

// ********** QPS **********
// ---------- ENTITIES ----------
std::vector<std::string> PKB::GetEntitiesWithType(EntityType type) {
  std::unordered_set<std::string> e = ent_data->Get(type);
  return std::vector<std::string>(e.begin(), e.end());
}

std::string PKB::ConvertEntityValueToAlias(std::string const& value,
                                           EntityType type,
                                           AttrType wanted_attr_type) {
  return ent_data->ConvertEntityValueToAlias(value, type, wanted_attr_type);
}

std::vector<std::string> PKB::GetEntitiesMatchingAttrValue(
    EntityType type, AttrType attr_type, std::string const& value) {
  return ent_data->GetEntitiesMatchingAttrValue(type, attr_type, value);
}

std::vector<std::pair<std::string, std::string>>
PKB::GetEntitiesWhereAttributesMatch(EntityType type_1, AttrType attr_type_1,
                                     EntityType type_2, AttrType attr_type_2) {
  return ent_data->GetEntitiesWhereAttributesMatch(type_1, attr_type_1, type_2,
                                                   attr_type_2);
}

// ---------- RELATIONS ----------
// 0 Declarations
// example Follows(1, 3)
bool PKB::IsRelationTrueValueValue(std::string const& value_1,
                                   std::string const& value_2,
                                   RelationType rel_type) {
  return rel_data->IsRelated(rel_type, value_1, value_2);
}

// example Follows(1, _)
bool PKB::IsRelationTrueValueWild(std::string const& value,
                                  RelationType rel_type) {
  return rel_data->HasRelations(rel_type, value);
}

// example Follows(_, 1)
bool PKB::IsRelationTrueWildValue(std::string const& value,
                                  RelationType rel_type) {
  return rel_data->HasInverseRelations(rel_type, value);
}

// example Follows(_, _)
bool PKB::IsRelationTrueWildWild(RelationType relation_type) {
  return !rel_data->IsEmpty(relation_type);
}

// 1 Declarations
// example Parent(s, _), ParentsStar(s, _)
std::vector<std::string> PKB::GetRelationSynonymWild(EntityType entity_type,
                                                     RelationType rel_type) {
  std::unordered_set<std::string> output =
      rel_data->GetAllWithRelations(rel_type, ent_data->Get(entity_type));
  return std::vector<std::string>(output.begin(), output.end());
}

// example Follows(_, s), FolowsStar(_, s)
std::vector<std::string> PKB::GetRelationWildSynonym(EntityType entity_type,
                                                     RelationType rel_type) {
  std::unordered_set<std::string> output = rel_data->GetAllWithInverseRelations(
      rel_type, ent_data->Get(entity_type));
  return std::vector<std::string>(output.begin(), output.end());
}

// example Follows(s, 3), FolowsStar(s, 3)
std::vector<std::string> PKB::GetRelationSynonymValue(EntityType entity_type,
                                                      std::string const& value,
                                                      RelationType rel_type) {
  std::unordered_set<std::string> all_inverse_related =
      rel_data->GetAllInverseRelatedToValue(rel_type, value);
  std::unordered_set<std::string> entities = ent_data->Get(entity_type);

  std::unordered_set<std::string> output =
      GetIntersection(all_inverse_related, entities);

  return std::vector<std::string>(output.begin(), output.end());
}

// example Follows(3, s)
std::vector<std::string> PKB::GetRelationValueSynonym(std::string const& value,
                                                      EntityType entity_type,
                                                      RelationType rel_type) {
  std::unordered_set<std::string> all_related =
      rel_data->GetAllRelatedToValue(rel_type, value);
  std::unordered_set<std::string> entities = ent_data->Get(entity_type);

  std::unordered_set<std::string> output =
      GetIntersection(all_related, entities);

  return std::vector<std::string>(output.begin(), output.end());
}

//// 2 Declarations
// example Follows(s1, s2), FollowsStar(s1, s2)
std::vector<std::pair<std::string, std::string>> PKB::GetRelationSynonymSynonym(
    EntityType entity_type_1, EntityType entity_type_2, RelationType rel_type) {
  std::vector<std::pair<std::string, std::string>> output;
  std::unordered_set<std::string> ents1 = PKB::ent_data->Get(entity_type_1);

  for (std::string ent1 : ents1) {
    std::unordered_set<std::string> all_related_to_ent1 =
        rel_data->GetAllRelatedToValue(rel_type, ent1);
    std::unordered_set<std::string> ents2 =
        PKB::GetIntersection(ent_data->Get(entity_type_2), all_related_to_ent1);

    for (std::string ent2 : ents2) {
      output.push_back(make_pair(ent1, ent2));
    }
  }

  return output;
}

std::vector<std::pair<std::string, std::string>> PKB::GetRelationSynonymSynonym(
    EntityType entity_type_1, EntityType entity_type_2, RelationType rel_type,
    std::unordered_set<std::string> const& syn_1_possible_values,
    std::unordered_set<std::string> const& syn_2_possible_values) {
  return {};
}

// ---------- PATTERNS ----------
std::vector<std::string> PKB::GetMatchingAssignStmts(
    std::shared_ptr<TreeNode> const& rhs_expr, MatchType match_type) {
  std::unordered_set<std::string> assign_stmts =
      ent_data->Get(EntityType::ASSIGN);
  return pat_data->GetMatchingAssignStmts(assign_stmts, rhs_expr, match_type);
}

std::vector<std::string> PKB::GetMatchingAssignStmts(
    std::string const& lhs_value, std::shared_ptr<TreeNode> const& rhs_expr,
    MatchType match_type) {
  return pat_data->GetMatchingAssignStmts(lhs_value, rhs_expr, match_type);
}

std::vector<std::pair<std::string, std::string>>
PKB::GetMatchingAssignStmtLhsVarPairs(std::shared_ptr<TreeNode> const& rhs_expr,
                                      MatchType match_type) {
  return pat_data->GetMatchingAssignStmtLhsVarPairs(rhs_expr, match_type);
}

std::vector<std::pair<std::string, std::string>>
PKB::GetMatchingAssignStmtLhsVarPairs(
    std::shared_ptr<TreeNode> const& rhs_expr, MatchType match_type,
    std::unordered_set<std::string> const& assign_syn_possible_values,
    std::unordered_set<std::string> const& var_syn_possible_values) {
  return {};
}

std::vector<std::string> PKB::GetContainerStmtsWithControlVar(
    EntityType container_stmt_type) {
  return pat_data->GetContainerStmtsWithControlVar(container_stmt_type);
}

std::vector<std::string> PKB::GetContainerStmtsWithGivenControlVar(
    EntityType container_stmt_type, std::string const& var_name) {
  return pat_data->GetContainerStmtsWithGivenControlVar(container_stmt_type,
                                                        var_name);
}

std::vector<std::pair<std::string, std::string>>
PKB::GetContainerStmtControlVarPairs(EntityType container_stmt_type) {
  return pat_data->GetContainerStmtControlVarPairs(container_stmt_type);
}

std::vector<std::pair<std::string, std::string>>
PKB::GetContainerStmtControlVarPairs(
    EntityType container_stmt_type,
    std::unordered_set<std::string> const& container_syn_possible_values,
    std::unordered_set<std::string> const& control_var_possible_values) {
  return {};
}
