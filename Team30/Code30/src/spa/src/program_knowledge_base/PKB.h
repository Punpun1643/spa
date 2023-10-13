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
#include "program_knowledge_base/EntityDatabase.h"
#include "program_knowledge_base/PatternDatabase.h"
#include "program_knowledge_base/RelDatabase.h"

class PKB : public PKBQPSInterface, public PKBSPInterface {
 private:
  std::unique_ptr<EntityDatabase> entData;
  std::unique_ptr<RelDatabase> relData;
  std::unique_ptr<PatternDatabase> patData;

  std::unordered_map<RelationType, std::vector<RelationType>> relatedTables;

  // Helper functions
  std::unordered_set<std::string> getIntersection(
      std::unordered_set<std::string> set1,
      std::unordered_set<std::string> set2);

 public:
  PKB();

  // ********** SP **********
  void insertEntity(EntityType type, std::string entity) override;
  void insertRelation(RelationType rel_type, std::string s1_line_num,
                      std::string s2_line_num) override;
  void insertPattern(PatternType type, std::string statement_number,
                     std::string lhs,
                     std::shared_ptr<TreeNode> rhs) override;
  void insertCFGNode(std::string statement_num,
                     std::shared_ptr<CFGNode> node) override;

  std::unordered_set<std::string> getProcedureModifies(
      std::string procName) override;
  std::unordered_set<std::string> getProcedureUses(
      std::string procName) override;

  // ********** QPS **********
  // ---------- ENTITIES ----------
  std::unique_ptr<std::vector<std::string>> getEntitiesWithType(
      EntityType type) override;

  std::string convertEntityAttribute(std::string value, EntityType type,
                                     AttrType curr_attr_type,
                                     AttrType wanted_attr_type) override;

  bool doesEntityExist(EntityType type, AttrType attr_type,
                       std::string value) override;

  std::vector<std::string> getMatchingEntities(EntityType type_1,
                                               AttrType attr_type_1,
                                               EntityType type_2,
                                               AttrType attr_type_2) override;

  // ---------- RELATIONS ----------
  // 0 Declarations
  bool isRelationTrueValueValue(std::string value_1, std::string value_2,
                                RelationType rel_type) override;
  bool isRelationTrueValueWild(std::string value,
                               RelationType rel_type) override;
  bool isRelationTrueWildValue(std::string value,
                               RelationType rel_type) override;
  bool isRelationTrueWildWild(RelationType relation_type) override;

  // 1 Declarations
  std::unique_ptr<std::vector<std::string>> getRelationSynonymWild(
      EntityType entity_type, RelationType rel_type) override;
  std::unique_ptr<std::vector<std::string>> getRelationWildSynonym(
      EntityType entity_type, RelationType rel_type) override;
  std::unique_ptr<std::vector<std::string>> getRelationSynonymValue(
      EntityType entity_type, std::string value,
      RelationType rel_type) override;
  std::unique_ptr<std::vector<std::string>> getRelationValueSynonym(
      std::string value, EntityType entity_type,
      RelationType rel_type) override;

  // 2 Declarations
  std::unique_ptr<std::vector<std::pair<std::string, std::string>>>
  getRelationSynonymSynonym(EntityType entity_type_1, EntityType entity_type_2,
                            RelationType rel_type) override;

  // ---------- PATTERNS ----------
  std::unique_ptr<std::vector<std::string>> getPatternMatchesWildLhs(
      std::shared_ptr<TreeNode> rhs_expr, MatchType match_type) override;
  std::unique_ptr<std::vector<std::string>> getPatternMatchesValueLhs(
      std::string lhs_value, std::shared_ptr<TreeNode> rhs_expr,
      MatchType match_type) override;
  std::unique_ptr<std::vector<std::pair<std::string, std::string>>>
  getPatternMatchesSynonymLhs(std::shared_ptr<TreeNode> rhs_expr,
                              MatchType match_type) override;
};
