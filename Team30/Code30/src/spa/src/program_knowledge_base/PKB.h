#pragma once

#include <stdio.h>

#include <iostream>
#include <memory>
#include <optional>
#include <string>
#include <utility>
#include <vector>

#include "../query_processing_system/common/EntityType.h"
#include "../source_processor/node/stmt_node/StmtNode.h"
#include "PkbApi.h"
#include "program_knowledge_base/EntityDatabase.h"
#include "program_knowledge_base/RelDatabase.h"

class PKB : public PkbApi {
  std::unique_ptr<EntityDatabase> entData;
  std::unique_ptr<RelDatabase> relData;
  std::unordered_map<RelationType, std::vector<RelationType>> relatedTables;

 public:
  PKB();
  ~PKB() = default;

  void insertEntity(EntityType type, std::string entity) override;
  void insertRelation(RelationType rel_type, std::string s1_line_num,
                      std::string s2_line_num) override;
  void insertRelation(RelationType rel_type, std::string s_line_num,
                      EntityType ent_type, std::string entity) override;
  void insertRelation(RelationType rel_type, EntityType ent_type,
                      std::string entity, std::string s_line_num) override;
  void insertRelation(RelationType rel_type, EntityType ent_type1,
                      std::string entity1, EntityType ent_type2,
                      std::string entity2) override;

  // Select Clause
  std::unique_ptr<std::vector<std::string>> getEntitiesWithType(
      EntityType type) override;

  // 0 Declarations - SuchThatClauses
  bool isRelationTrue(std::string value_1, std::string value_2,
                      RelationType rel_type) override;
  bool isRelationTrueGivenFirstValue(std::string value,
                                     RelationType rel_type) override;
  bool isRelationTrueGivenSecondValue(std::string value,
                                      RelationType rel_type) override;
  bool isRelationTrueForAny(RelationType relation_type) override;

  // 1 Declarations - SuchThatClauses
  std::unique_ptr<std::vector<std::string>> getRelationValuesGivenFirstType(
      EntityType entity_type, RelationType rel_type) override;
  std::unique_ptr<std::vector<std::string>> getRelationValuesGivenSecondType(
      EntityType entity_type, RelationType rel_type) override;
  std::unique_ptr<std::vector<std::string>> getRelationValues(
      EntityType entity_type, std::string value,
                                               RelationType rel_type) override;
  std::unique_ptr<std::vector<std::string>> getRelationValues(
      std::string value,
                                               EntityType entity_type,
                                               RelationType rel_type) override;

  // 2 Declarations - SuchThatClauses
  std::unique_ptr<std::vector<std::pair<std::string, std::string>>>
  getRelationValues(
      EntityType entity_type_1, EntityType entity_type_2,
      RelationType rel_type) override;

  // Pattern clause
  std::unique_ptr<std::vector<std::string>> getPatternMatchesWithWildLhs(std::string rhs_expr, MatchType expr_match_type) override;


  std::unique_ptr<std::vector<std::string>> getPatternMatchesWithLhsValue(std::string lhs_value, std::string rhs_expr,
                                                                                  MatchType expr_match_type) override;

  // 2 paired values - for the implicit assign declaration, and the values for the given lhs_entity_type
  std::unique_ptr<std::vector<std::pair<std::string, std::string>>> getPatternMatchesWithLhsType(EntityType lhs_entity_type,
                                                                                                         std::string rhs_expr,
                                                                                                         MatchType expr_match_type) override;

};