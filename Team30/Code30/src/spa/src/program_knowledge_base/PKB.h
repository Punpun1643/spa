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
#include "PKBQPSInterface.h"
#include "PKBSPInterface.h"
#include "WildCardMatcher.h"
#include "program_knowledge_base/EntityDatabase.h"
#include "program_knowledge_base/PatternDatabase.h"
#include "program_knowledge_base/RelDatabase.h"

class PKB : public PKBQPSInterface, public PkbApi {
 private:
  WildCardMatcher wildCardMatcher;
  std::unique_ptr<EntityDatabase> entData;
  std::unique_ptr<RelDatabase> relData;
  std::unique_ptr<PatternDatabase> patData;

  std::unordered_map<RelationType, std::vector<RelationType>> relatedTables;

  // Helper functions
  std::unordered_set<std::string> getAllRelated(
      RelationType rel_type,
      std::shared_ptr<std::unordered_set<std::string>> set, std::string value);

  std::unordered_set<std::string> getAllRelated(
      RelationType rel_type, std::string value,
      std::shared_ptr<std::unordered_set<std::string>> set);

 public:
  PKB();

  // ********** SP **********
  void insertEntity(EntityType type, std::string entity) override;
  void insertRelation(RelationType rel_type, std::string s1_line_num,
                      std::string s2_line_num) override;
  void insertPattern(std::string statement_number, std::string lhs,
                     std::unordered_set<std::string> rhs) override;
  std::unordered_set<std::string> getProcedureModifies(
      std::string procName) override;
  std::unordered_set<std::string> getProcedureUses(
      std::string procName) override;

  // ********** QPS **********
  // ---------- ENTITIES ----------
  std::unique_ptr<std::vector<std::string>> getEntitiesWithType(
      EntityType type) override;

  // ---------- RELATIONS ----------
  // 0 Declarations
  bool isRelationTrue(std::string value_1, std::string value_2,
                      RelationType rel_type) override;
  bool isRelationTrueGivenFirstValue(std::string value,
                                     RelationType rel_type) override;
  bool isRelationTrueGivenSecondValue(std::string value,
                                      RelationType rel_type) override;
  bool isRelationTrueForAny(RelationType relation_type) override;

  // 1 Declarations
  std::unique_ptr<std::vector<std::string>> getRelationValuesGivenFirstType(
      EntityType entity_type, RelationType rel_type) override;
  std::unique_ptr<std::vector<std::string>> getRelationValuesGivenSecondType(
      EntityType entity_type, RelationType rel_type) override;
  std::unique_ptr<std::vector<std::string>> getRelationValues(
      EntityType entity_type, std::string value,
      RelationType rel_type) override;
  std::unique_ptr<std::vector<std::string>> getRelationValues(
      std::string value, EntityType entity_type,
      RelationType rel_type) override;

  // 2 Declarations
  std::unique_ptr<std::vector<std::pair<std::string, std::string>>>
  getRelationValues(EntityType entity_type_1, EntityType entity_type_2,
                    RelationType rel_type) override;

  // ---------- PATTERNS ----------
  std::unique_ptr<std::vector<std::string>> getPatternMatchesWithWildLhs(
      std::string rhs_expr, MatchType expr_match_type) override;
  std::unique_ptr<std::vector<std::string>> getPatternMatchesWithLhsValue(
      std::string lhs_value, std::string rhs_expr,
      MatchType expr_match_type) override;
  std::unique_ptr<std::vector<std::pair<std::string, std::string>>>
  getPatternMatchesWithDeclarationLhs(std::string rhs_expr,
                                      MatchType expr_match_type) override;
};