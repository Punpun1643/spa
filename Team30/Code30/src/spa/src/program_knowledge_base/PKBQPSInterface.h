#pragma once
#include <source_processor/node/stmt_node/AssignNode.h>
#include <source_processor/node/stmt_node/StmtNode.h>

#include <memory>
#include <optional>
#include <string>
#include <unordered_set>
#include <utility>
#include <vector>

#include "../query_processing_system/common/EntityType.h"
#include "RelationType.h"
#include "query_processing_system/common/AttrType.h"
#include "query_processing_system/common/MatchType.h"

class PKBQPSInterface {
 public:
  // ---------- ENTITIES ----------
  virtual std::unique_ptr<std::vector<std::string>> getEntitiesWithType(
      EntityType type) = 0;  // Returns all Entities in the default AttrType

  virtual std::string ConvertEntityValueToAlias(
      std::string value, EntityType type, AttrType curr_attr_type,
      AttrType alias_attr_type) = 0;

  // ---------- WITH CLAUSES ----------
  virtual std::vector<std::string> GetEntitiesMatchingAttrValue(EntityType type, AttrType attr_type,
                                 std::string attr_value) = 0;

  virtual std::vector<std::pair<std::string, std::string>> GetEntitiesWhereAttributesMatch(
      EntityType type_1, AttrType attr_type_1, EntityType type_2,
      AttrType attr_type_2) = 0;

  // ---------- RELATIONS ----------
  // 0 Declarations
  // Relation(int, int)
  virtual bool isRelationTrueValueValue(std::string value_1,
                                        std::string value_2,
                                        RelationType rel_type) = 0;
  // Relation(int, _)
  virtual bool isRelationTrueValueWild(std::string value,
                                       RelationType rel_type) = 0;
  // Relation(_, int)
  virtual bool isRelationTrueWildValue(std::string value,
                                       RelationType rel_type) = 0;
  // Relation(_, _)
  virtual bool isRelationTrueWildWild(RelationType relation_type) = 0;

  // 1 Declarations
  // Relation(syn, _)
  virtual std::unique_ptr<std::vector<std::string>> getRelationSynonymWild(
      EntityType entity_type, RelationType rel_type) = 0;
  // Relation(_, syn)
  virtual std::unique_ptr<std::vector<std::string>> getRelationWildSynonym(
      EntityType entity_type, RelationType rel_type) = 0;
  // Relation(syn, int)
  virtual std::unique_ptr<std::vector<std::string>> getRelationSynonymValue(
      EntityType entity_type, std::string value, RelationType rel_type) = 0;
  // Relation(int, syn)
  virtual std::unique_ptr<std::vector<std::string>> getRelationValueSynonym(
      std::string value, EntityType entity_type, RelationType rel_type) = 0;

  // 2 Declarations
  // Relation(syn1, syn2)
  virtual std::unique_ptr<std::vector<std::pair<std::string, std::string>>>
  getRelationSynonymSynonym(EntityType entity_type_1, EntityType entity_type_2,
                            RelationType rel_type) = 0;

  // ---------- PATTERNS ----------
  virtual std::unique_ptr<std::vector<std::string>> getPatternMatchesWildLhs(
      std::shared_ptr<TreeNode> rhs_expr, MatchType match_type) = 0;
  virtual std::unique_ptr<std::vector<std::string>> getPatternMatchesValueLhs(
      std::string lhs_value, std::shared_ptr<TreeNode> rhs_expr,
      MatchType match_type) = 0;
  virtual std::unique_ptr<std::vector<std::pair<std::string, std::string>>>
  getPatternMatchesSynonymLhs(std::shared_ptr<TreeNode> rhs_expr,
                              MatchType match_type) = 0;

  // ----------------- TO DELETE ----------------------
  virtual std::unique_ptr<std::vector<std::string>> getPatternMatchesWildLhs(
      std::string rhs_expr, MatchType expr_match_type) = 0;
  virtual std::unique_ptr<std::vector<std::string>> getPatternMatchesValueLhs(
      std::string lhs_value, std::string rhs_expr,
      MatchType expr_match_type) = 0;
  // 2 paired values - one for the implicit assign declaration, paired with
  // the variable declaration on the LHS of the assign stmt
  virtual std::unique_ptr<std::vector<std::pair<std::string, std::string>>>
  getPatternMatchesSynonymLhs(std::string rhs_expr,
                              MatchType expr_match_type) = 0;
};
