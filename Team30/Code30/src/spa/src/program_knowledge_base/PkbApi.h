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
#include "query_processing_system/common/MatchType.h"

class PkbApi {
 public:
  // Cast integers to strings
  virtual void insertEntity(EntityType type, std::string entity) = 0;

  // Relation (integer, integer)
  virtual void insertRelation(RelationType rel_type, std::string s1_line_num,
                              std::string s2_line_num) = 0;

  // Relation (integer, EntityType)
  virtual void insertRelation(RelationType rel_type, std::string s_line_num,
                              EntityType ent_type, std::string entity) = 0;

  // Relation (EntityType, integer)
  virtual void insertRelation(RelationType rel_type, EntityType ent_type,
                              std::string entity, std::string s_line_num) = 0;

  // Relation (EntityType, EntityType)
  virtual void insertRelation(RelationType rel_type, EntityType ent_type1,
                              std::string entity1, EntityType ent_type2,
                              std::string entity2) = 0;

  // Select clauses
  virtual std::unique_ptr<std::vector<std::string>> getEntitiesWithType(
      EntityType type) = 0;

  // 0 Declarations - SuchThatClauses
  // Relation(value, value) e.g. Follows (int, int)
  virtual bool isRelationTrue(std::string value_1, std::string value_2,
                              RelationType rel_type) = 0;
  // Relation(value, wild) e.g. Follows (int, _)
  virtual bool isRelationTrueGivenFirstValue(std::string value,
                                             RelationType rel_type) = 0;
  // Relation(wild, value) e.g. Follows(_, int)
  virtual bool isRelationTrueGivenSecondValue(std::string value,
                                              RelationType rel_type) = 0;
  // Relation(wild, wild) e.g. Follows(_, _)
  virtual bool isRelationTrueForAny(RelationType relation_type) = 0;

  // 1 Declarations - SuchThatClauses
  // Relation(declaration, wild) e.g. Follows(s, _)
  virtual std::unique_ptr<std::vector<std::string>>
  getRelationValuesGivenFirstType(EntityType entity_type,
                                  RelationType rel_type) = 0;
  // Relation(wild, declaration) e.g. Follows(_, s)
  virtual std::unique_ptr<std::vector<std::string>>
  getRelationValuesGivenSecondType(EntityType entity_type,
                                   RelationType rel_type) = 0;
  // Relation(declaration, value) e.g. Follows(s, 5)
  virtual std::unique_ptr<std::vector<std::string>> getRelationValues(
      EntityType entity_type, std::string value, RelationType rel_type) = 0;
  // Relation(value, declaration) e.g. Follows(5, s)
  virtual std::unique_ptr<std::vector<std::string>> getRelationValues(
      std::string value, EntityType entity_type, RelationType rel_type) = 0;

  // 2 Declarations - SuchThatClauses
  // Relation (declaration, declaration) e.g. Follows(s1, s2)
  virtual std::unique_ptr<std::vector<std::pair<std::string, std::string>>>
  getRelationValues(EntityType entity_type_1, EntityType entity_type_2,
                    RelationType rel_type) = 0;

  // Pattern Clauses
  virtual void insertPattern(std::string statement_number, std::string lhs,
                             std::unordered_set<std::string> rhs) = 0;
  virtual std::unique_ptr<std::vector<std::string>>
  getPatternMatchesWithWildLhs(std::string rhs_expr,
                               MatchType expr_match_type) = 0;
  virtual std::unique_ptr<std::vector<std::string>>
  getPatternMatchesWithLhsValue(std::string lhs_value, std::string rhs_expr,
                                MatchType expr_match_type) = 0;
  // 2 paired values - one for the implicit assign declaration, paired with
  // the variable declaration on the LHS of the assign stmt
  virtual std::unique_ptr<std::vector<std::pair<std::string, std::string>>>
  getPatternMatchesWithLhsType(std::string rhs_expr,
                               MatchType expr_match_type) = 0;

  virtual ~PkbApi();  // so that the subclass destructors will be called
};
