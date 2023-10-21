#include "PKB.h"

#include <algorithm>
#include <cassert>
#include <string>
#include <unordered_set>
#include <vector>

PKB::PKB() : PKBQPSInterface(), PKBSPInterface() {
  entData = std::make_unique<EntityDatabase>();
  relData = std::make_unique<RelDatabase>();
  patData = std::make_unique<PatternDatabase>();
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
  entData->InsertEntity(type, value);
}

void PKB::InsertEntity(EntityType type, AttrType attr_type,
                       std::string statement_number, std::string attribute) {
  entData->InsertEntity(type, attr_type, statement_number, attribute);
}

void PKB::InsertRelation(RelationType type, std::string input1,
                         std::string input2) {
  relData->Insert(type, input1, input2);
}

void PKB::InsertPattern(PatternType type, std::string statement_number,
                        std::string lhs, std::shared_ptr<TreeNode> rhs) {
  patData->Insert(type, statement_number, lhs, rhs);
}

void PKB::InsertCFGNode(std::string statement_number,
                        std::shared_ptr<CFGNode> node) {
  relData->InsertCFGNode(statement_number, node);
}

std::unordered_set<std::string> PKB::GetProcedureModifies(
    std::string procName) {
  return relData->GetAllRelatedToValue(RelationType::MODIFIES_P, procName);
}

std::unordered_set<std::string> PKB::GetProcedureUses(std::string procName) {
  return relData->GetAllRelatedToValue(RelationType::USES_P, procName);
}

std::unordered_set<std::string> PKB::GetStatementModifies(std::string stmt) {
  return relData->GetAllRelatedToValue(RelationType::MODIFIES_S, stmt);
}

std::unordered_set<std::string> PKB::GetStatementUses(std::string stmt) {
  return relData->GetAllRelatedToValue(RelationType::USES_S, stmt);
}

// ********** QPS **********
// ---------- ENTITIES ----------
std::unique_ptr<std::vector<std::string>> PKB::GetEntitiesWithType(
    EntityType type) {
  std::shared_ptr<std::unordered_set<std::string>> e = entData->get(type);
  return std::make_unique<std::vector<std::string>>(e->begin(), e->end());
}

std::string PKB::ConvertEntityValueToAlias(std::string value, EntityType type,
                                           AttrType curr_attr_type,
                                           AttrType wanted_attr_type) {
  return entData->ConvertEntityValueToAlias(value, type, curr_attr_type,
                                            wanted_attr_type);
}

std::vector<std::string> PKB::GetEntitiesMatchingAttrValue(EntityType type,
                                                           AttrType attr_type,
                                                           std::string value) {
  return entData->GetEntitiesMatchingAttrValue(type, attr_type, value);
}

std::vector<std::pair<std::string, std::string>>
PKB::GetEntitiesWhereAttributesMatch(EntityType type_1, AttrType attr_type_1,
                                     EntityType type_2, AttrType attr_type_2) {
  return entData->GetEntitiesWhereAttributesMatch(type_1, attr_type_1, type_2,
                                                  attr_type_2);
}

// ---------- RELATIONS ----------
// 0 Declarations
// example Follows(1, 3)
bool PKB::IsRelationTrueValueValue(std::string value_1, std::string value_2,
                                   RelationType rel_type) {
  return relData->IsRelated(rel_type, value_1, value_2);
}

// example Follows(1, _)
bool PKB::IsRelationTrueValueWild(std::string value, RelationType rel_type) {
  return relData->HasRelations(rel_type, value);
}

// example Follows(_, 1)
bool PKB::IsRelationTrueWildValue(std::string value, RelationType rel_type) {
  return relData->HasInverseRelations(rel_type, value);
}

// example Follows(_, _)
bool PKB::IsRelationTrueWildWild(RelationType relation_type) {
  return !relData->IsEmpty(relation_type);
}

// 1 Declarations
// example Parent(s, _), ParentsStar(s, _)
std::unique_ptr<std::vector<std::string>> PKB::GetRelationSynonymWild(
    EntityType entity_type, RelationType rel_type) {
  std::unordered_set<std::string> output =
      relData->GetAllWithRelations(rel_type, entData->get(entity_type));
  return std::make_unique<std::vector<std::string>>(output.begin(),
                                                    output.end());
}

// example Follows(_, s), FolowsStar(_, s)
std::unique_ptr<std::vector<std::string>> PKB::GetRelationWildSynonym(
    EntityType entity_type, RelationType rel_type) {
  std::unordered_set<std::string> output =
      relData->GetAllWithInverseRelations(rel_type, entData->get(entity_type));
  return std::make_unique<std::vector<std::string>>(output.begin(),
                                                    output.end());
}

// example Follows(s, 3), FolowsStar(s, 3)
std::unique_ptr<std::vector<std::string>> PKB::GetRelationSynonymValue(
    EntityType entity_type, std::string value, RelationType rel_type) {
  std::unordered_set<std::string> allInverseRelated =
      relData->GetAllInverseRelatedToValue(rel_type, value);
  std::unordered_set<std::string> entities = *entData->get(entity_type);

  std::unordered_set<std::string> output =
      GetIntersection(allInverseRelated, entities);

  return std::make_unique<std::vector<std::string>>(output.begin(),
                                                    output.end());
}

// example Follows(3, s)
std::unique_ptr<std::vector<std::string>> PKB::GetRelationValueSynonym(
    std::string value, EntityType entity_type, RelationType rel_type) {
  std::unordered_set<std::string> allRelated =
      relData->GetAllRelatedToValue(rel_type, value);
  std::unordered_set<std::string> entities = *entData->get(entity_type);

  std::unordered_set<std::string> output =
      GetIntersection(allRelated, entities);

  return std::make_unique<std::vector<std::string>>(output.begin(),
                                                    output.end());
}

//// 2 Declarations
// example Follows(s1, s2), FollowsStar(s1, s2)
std::unique_ptr<std::vector<std::pair<std::string, std::string>>>
PKB::GetRelationSynonymSynonym(EntityType entity_type_1,
                               EntityType entity_type_2,
                               RelationType rel_type) {
  std::vector<std::pair<std::string, std::string>> output;
  std::shared_ptr<std::unordered_set<std::string>> ents1 =
      PKB::entData->get(entity_type_1);
  std::shared_ptr<std::unordered_set<std::string>> ents2 =
      entData->get(entity_type_2);

  for (std::string ent1 : *ents1) {
    for (std::string ent2 : *ents2) {
      if (relData->IsRelated(rel_type, ent1, ent2)) {
        output.push_back(make_pair(ent1, ent2));
      }
    }
  }

  return std::make_unique<std::vector<std::pair<std::string, std::string>>>(
      output);
}

// ---------- PATTERNS ----------
std::unique_ptr<std::vector<std::string>> PKB::GetPatternMatchesWildLhs(
    std::shared_ptr<TreeNode> rhs_expr, MatchType match_type) {
  std::unordered_set<std::string> output;

  // Wild Wild
  if (match_type == MatchType::WILD_MATCH) {
    output = *entData->get(EntityType::ASSIGN);
  } else if (match_type == MatchType::PARTIAL_MATCH) {  // Wild Partial
    for (auto& pair : patData->GetAssignmentPatterns()) {
      if (TreeNode::IsSubTree(pair.second.second, rhs_expr)) {
        output.insert(pair.first);
      }
    }
  } else {  // Wild Exact
    for (auto& pair : patData->GetAssignmentPatterns()) {
      if (TreeNode::IsSameTree(pair.second.second, rhs_expr)) {
        output.insert(pair.first);
      }
    }
  }

  return std::make_unique<std::vector<std::string>>(output.begin(),
                                                    output.end());
}

std::unique_ptr<std::vector<std::string>> PKB::GetPatternMatchesValueLhs(
    std::string lhs_value, std::shared_ptr<TreeNode> rhs_expr,
    MatchType match_type) {
  std::unordered_set<std::string> output;

  // String Wild
  if (match_type == MatchType::WILD_MATCH) {
    output = patData->GetStatementNumbersGivenLHS(lhs_value);
  } else if (match_type == MatchType::PARTIAL_MATCH) {  // String Partial
    for (auto& pair : patData->GetAssignmentPatterns()) {
      if (pair.second.first == lhs_value &&
          TreeNode::IsSubTree(pair.second.second, rhs_expr)) {
        output.insert(pair.first);
      }
    }
  } else {  // String Exact
    for (auto& pair : patData->GetAssignmentPatterns()) {
      if (pair.second.first == lhs_value &&
          TreeNode::IsSameTree(pair.second.second, rhs_expr)) {
        output.insert(pair.first);
      }
    }
  }

  return std::make_unique<std::vector<std::string>>(output.begin(),
                                                    output.end());
}

std::unique_ptr<std::vector<std::pair<std::string, std::string>>>
PKB::GetPatternMatchesSynonymLhs(std::shared_ptr<TreeNode> rhs_expr,
                                 MatchType match_type) {
  std::vector<std::pair<std::string, std::string>> output;

  // Synonym Wild
  if (match_type == MatchType::WILD_MATCH) {
    for (auto& pair : patData->GetAssignmentPatterns()) {
      output.push_back(make_pair(pair.first, pair.second.first));
    }
  } else if (match_type == MatchType::PARTIAL_MATCH) {  // Synonym Partial
    for (auto& pair : patData->GetAssignmentPatterns()) {
      if (TreeNode::IsSubTree(pair.second.second, rhs_expr)) {
        output.push_back(make_pair(pair.first, pair.second.first));
      }
    }
  } else {  // Synonym Exact
    for (auto& pair : patData->GetAssignmentPatterns()) {
      if (TreeNode::IsSameTree(pair.second.second, rhs_expr)) {
        output.push_back(make_pair(pair.first, pair.second.first));
      }
    }
  }

  return std::make_unique<std::vector<std::pair<std::string, std::string>>>(
      output.begin(), output.end());
}
