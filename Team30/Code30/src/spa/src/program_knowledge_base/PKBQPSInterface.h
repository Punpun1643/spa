#pragma once
#include <source_processor/node/stmt_node/AssignNode.h>
#include <source_processor/node/stmt_node/StmtNode.h>

#include <memory>
#include <optional>
#include <string>
#include <unordered_set>
#include <utility>
#include <vector>

#include "shared/types/AttrType.h"
#include "shared/types/EntityType.h"
#include "shared/types/MatchType.h"
#include "shared/types/RelationType.h"

class PKBQPSInterface {
 public:
  // ---------- ENTITIES ----------
  virtual std::vector<std::string> GetEntitiesWithType(
      EntityType type) = 0;  // Returns all Entities in the default AttrType

  virtual std::string ConvertEntityValueToAlias(std::string const& value,
                                                EntityType type,
                                                AttrType alias_attr_type) = 0;

  // ---------- WITH CLAUSES ----------
  virtual std::vector<std::string> GetEntitiesMatchingAttrValue(
      EntityType type, AttrType attr_type, std::string const& value) = 0;

  virtual std::vector<std::pair<std::string, std::string>>
  GetEntitiesWhereAttributesMatch(EntityType type_1, AttrType attr_type_1,
                                  EntityType type_2, AttrType attr_type_2) = 0;

  // ---------- RELATIONS ----------
  // 0 Declarations
  // Relation(int, int)
  virtual bool IsRelationTrueValueValue(std::string const& value_1,
                                        std::string const& value_2,
                                        RelationType rel_type) = 0;
  // Relation(int, _)
  virtual bool IsRelationTrueValueWild(std::string const& value,
                                       RelationType rel_type) = 0;
  // Relation(_, int)
  virtual bool IsRelationTrueWildValue(std::string const& value,
                                       RelationType rel_type) = 0;
  // Relation(_, _)
  virtual bool IsRelationTrueWildWild(RelationType relation_type) = 0;

  // 1 Declarations
  // Relation(syn, _)
  virtual std::vector<std::string> GetRelationSynonymWild(
      EntityType entity_type, RelationType rel_type) = 0;
  // Relation(_, syn)
  virtual std::vector<std::string> GetRelationWildSynonym(
      EntityType entity_type, RelationType rel_type) = 0;
  // Relation(syn, int)
  virtual std::vector<std::string> GetRelationSynonymValue(
      EntityType entity_type, std::string const& value,
      RelationType rel_type) = 0;
  // Relation(int, syn)
  virtual std::vector<std::string> GetRelationValueSynonym(
      std::string const& value, EntityType entity_type,
      RelationType rel_type) = 0;

  // 2 Declarations
  // Relation(syn1, syn2)
  virtual std::vector<std::pair<std::string, std::string>>
  GetRelationSynonymSynonym(EntityType entity_type_1, EntityType entity_type_2,
                            RelationType rel_type) = 0;

  virtual std::vector<std::pair<std::string, std::string>>
  GetRelationSynonymSynonym(
      EntityType entity_type_1, EntityType entity_type_2, RelationType rel_type,
      std::unordered_set<std::string> const& syn_1_possible_values,
      std::unordered_set<std::string> const& syn_2_possible_values) = 0;

  // ---------- PATTERNS ----------
  // wild LHS
  virtual std::vector<std::string> GetMatchingAssignStmts(
      std::shared_ptr<TreeNode> const& rhs_expr, MatchType match_type) = 0;

  // LHS with a fixed value
  virtual std::vector<std::string> GetMatchingAssignStmts(
      std::string const& lhs_value, std::shared_ptr<TreeNode> const& rhs_expr,
      MatchType match_type) = 0;

  virtual std::vector<std::pair<std::string, std::string>>
  GetMatchingAssignStmtLhsVarPairs(std::shared_ptr<TreeNode> const& rhs_expr,
                                   MatchType match_type) = 0;

  virtual std::vector<std::pair<std::string, std::string>>
  GetMatchingAssignStmtLhsVarPairs(
      std::shared_ptr<TreeNode> const& rhs_expr, MatchType match_type,
      std::unordered_set<std::string> const& assign_syn_possible_values,
      std::unordered_set<std::string> const& var_syn_possible_values) = 0;

  virtual std::vector<std::string> GetContainerStmtsWithControlVar(
      EntityType container_stmt_type) = 0;

  virtual std::vector<std::string> GetContainerStmtsWithGivenControlVar(
      EntityType container_stmt_type, std::string const& var_name) = 0;

  virtual std::vector<std::pair<std::string, std::string>>
  GetContainerStmtControlVarPairs(EntityType container_stmt_type) = 0;

  virtual std::vector<std::pair<std::string, std::string>>
  GetContainerStmtControlVarPairs(
      EntityType container_stmt_type,
      std::unordered_set<std::string> const& container_syn_possible_values,
      std::unordered_set<std::string> const& control_var_possible_values) = 0;
};
