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
std::unordered_set<std::string> PKB::GetIntersection(
    std::unordered_set<std::string> set1,
    std::unordered_set<std::string> set2) {
  std::unordered_set<std::string> output;
  for (std::string v : set1) {
    if (set2.find(v) != set2.end()) {
      output.insert(v);
    }
  }
  return output;
}

// ********** SP **********
void PKB::InsertEntity(EntityType type, std::string value) {
  ent_data->InsertEntity(type, value);
}

void PKB::InsertEntity(EntityType type, AttrType attr_type,
                       std::string statement_number, std::string attribute) {
  ent_data->InsertEntity(type, attr_type, statement_number, attribute);
}

void PKB::InsertRelation(RelationType type, std::string input1,
                         std::string input2) {
  rel_data->Insert(type, input1, input2);
}

void PKB::InsertPattern(PatternType type, std::string statement_number,
                        std::string lhs, std::shared_ptr<TreeNode> rhs) {
  pat_data->Insert(type, statement_number, lhs, rhs);
}

void PKB::InsertCFGNode(std::string statement_number,
                        std::shared_ptr<CFGNode> node) {
  rel_data->InsertCFGNode(statement_number, node);
}

std::unordered_set<std::string> PKB::GetProcedureModifies(
    std::string proc_name) {
  return rel_data->GetAllRelatedToValue(RelationType::MODIFIES_P, proc_name);
}

std::unordered_set<std::string> PKB::GetProcedureUses(std::string proc_name) {
  return rel_data->GetAllRelatedToValue(RelationType::USES_P, proc_name);
}

std::unordered_set<std::string> PKB::GetStatementModifies(std::string stmt) {
  return rel_data->GetAllRelatedToValue(RelationType::MODIFIES_S, stmt);
}

std::unordered_set<std::string> PKB::GetStatementUses(std::string stmt) {
  return rel_data->GetAllRelatedToValue(RelationType::USES_S, stmt);
}

// ********** QPS **********
// ---------- ENTITIES ----------
std::vector<std::string> PKB::GetEntitiesWithType(EntityType type) {
  std::shared_ptr<std::unordered_set<std::string>> e = ent_data->Get(type);
  return std::vector<std::string>(e->begin(), e->end());
}

std::string PKB::ConvertEntityValueToAlias(std::string value, EntityType type,
                                           AttrType curr_attr_type,
                                           AttrType wanted_attr_type) {
  return ent_data->ConvertEntityValueToAlias(value, type, curr_attr_type,
                                            wanted_attr_type);
}

std::vector<std::string> PKB::GetEntitiesMatchingAttrValue(EntityType type,
                                                           AttrType attr_type,
                                                           std::string value) {
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
bool PKB::IsRelationTrueValueValue(std::string value_1, std::string value_2,
                                   RelationType rel_type) {
  return rel_data->IsRelated(rel_type, value_1, value_2);
}

// example Follows(1, _)
bool PKB::IsRelationTrueValueWild(std::string value, RelationType rel_type) {
  return rel_data->HasRelations(rel_type, value);
}

// example Follows(_, 1)
bool PKB::IsRelationTrueWildValue(std::string value, RelationType rel_type) {
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
  std::unordered_set<std::string> output =
      rel_data->GetAllWithInverseRelations(rel_type, ent_data->Get(entity_type));
  return std::vector<std::string>(output.begin(), output.end());
}

// example Follows(s, 3), FolowsStar(s, 3)
std::vector<std::string> PKB::GetRelationSynonymValue(EntityType entity_type,
                                                      std::string value,
                                                      RelationType rel_type) {
  std::unordered_set<std::string> allInverseRelated =
      rel_data->GetAllInverseRelatedToValue(rel_type, value);
  std::unordered_set<std::string> entities = *ent_data->Get(entity_type);

  std::unordered_set<std::string> output =
      GetIntersection(allInverseRelated, entities);

  return std::vector<std::string>(output.begin(), output.end());
}

// example Follows(3, s)
std::vector<std::string> PKB::GetRelationValueSynonym(std::string value,
                                                      EntityType entity_type,
                                                      RelationType rel_type) {
  std::unordered_set<std::string> allRelated =
      rel_data->GetAllRelatedToValue(rel_type, value);
  std::unordered_set<std::string> entities = *ent_data->Get(entity_type);

  std::unordered_set<std::string> output =
      GetIntersection(allRelated, entities);

  return std::vector<std::string>(output.begin(), output.end());
}

//// 2 Declarations
// example Follows(s1, s2), FollowsStar(s1, s2)
std::vector<std::pair<std::string, std::string>> PKB::GetRelationSynonymSynonym(
    EntityType entity_type_1, EntityType entity_type_2, RelationType rel_type) {
  std::vector<std::pair<std::string, std::string>> output;
  std::shared_ptr<std::unordered_set<std::string>> ents1 =
      PKB::ent_data->Get(entity_type_1);
  std::shared_ptr<std::unordered_set<std::string>> ents2 =
      ent_data->Get(entity_type_2);

  for (std::string ent1 : *ents1) {
    for (std::string ent2 : *ents2) {
      if (rel_data->IsRelated(rel_type, ent1, ent2)) {
        output.push_back(make_pair(ent1, ent2));
      }
    }
  }

  return std::vector<std::pair<std::string, std::string>>(output);
}

// ---------- PATTERNS ----------
std::vector<std::string> PKB::GetPatternMatchesWildLhs(
    std::shared_ptr<TreeNode> rhs_expr, MatchType match_type) {
  std::unordered_set<std::string> output;

  // Wild Wild
  if (match_type == MatchType::WILD_MATCH) {
    output = *ent_data->Get(EntityType::ASSIGN);
  } else if (match_type == MatchType::PARTIAL_MATCH) {  // Wild Partial
    for (auto& pair : pat_data->GetAssignmentPatterns()) {
      if (TreeNode::IsSubTree(pair.second.second, rhs_expr)) {
        output.insert(pair.first);
      }
    }
  } else {  // Wild Exact
    for (auto& pair : pat_data->GetAssignmentPatterns()) {
      if (TreeNode::IsSameTree(pair.second.second, rhs_expr)) {
        output.insert(pair.first);
      }
    }
  }

  return std::vector<std::string>(output.begin(), output.end());
}

std::vector<std::string> PKB::GetPatternMatchesValueLhs(
    std::string lhs_value, std::shared_ptr<TreeNode> rhs_expr,
    MatchType match_type) {
  std::unordered_set<std::string> output;

  // String Wild
  if (match_type == MatchType::WILD_MATCH) {
    output = pat_data->GetStatementNumbersGivenLHS(lhs_value);
  } else if (match_type == MatchType::PARTIAL_MATCH) {  // String Partial
    for (auto& pair : pat_data->GetAssignmentPatterns()) {
      if (pair.second.first == lhs_value &&
          TreeNode::IsSubTree(pair.second.second, rhs_expr)) {
        output.insert(pair.first);
      }
    }
  } else {  // String Exact
    for (auto& pair : pat_data->GetAssignmentPatterns()) {
      if (pair.second.first == lhs_value &&
          TreeNode::IsSameTree(pair.second.second, rhs_expr)) {
        output.insert(pair.first);
      }
    }
  }

  return std::vector<std::string>(output.begin(), output.end());
}

std::vector<std::pair<std::string, std::string>>
PKB::GetPatternMatchesSynonymLhs(std::shared_ptr<TreeNode> rhs_expr,
                                 MatchType match_type) {
  std::vector<std::pair<std::string, std::string>> output;

  // Synonym Wild
  if (match_type == MatchType::WILD_MATCH) {
    for (auto& pair : pat_data->GetAssignmentPatterns()) {
      output.push_back(make_pair(pair.first, pair.second.first));
    }
  } else if (match_type == MatchType::PARTIAL_MATCH) {  // Synonym Partial
    for (auto& pair : pat_data->GetAssignmentPatterns()) {
      if (TreeNode::IsSubTree(pair.second.second, rhs_expr)) {
        output.push_back(make_pair(pair.first, pair.second.first));
      }
    }
  } else {  // Synonym Exact
    for (auto& pair : pat_data->GetAssignmentPatterns()) {
      if (TreeNode::IsSameTree(pair.second.second, rhs_expr)) {
        output.push_back(make_pair(pair.first, pair.second.first));
      }
    }
  }

  return std::vector<std::pair<std::string, std::string>>(output.begin(),
                                                          output.end());
}
