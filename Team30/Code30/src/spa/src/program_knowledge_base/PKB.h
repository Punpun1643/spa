#pragma once

#include <stdio.h>

#include <iostream>
#include <memory>
#include <optional>
#include <string>
#include <unordered_set>
#include <utility>
#include <vector>

#include "../source_processor/node/stmt_node/StmtNode.h"
#include "PKBQPSInterface.h"
#include "PKBSPInterface.h"
#include "program_knowledge_base/EntityDatabase.h"
#include "program_knowledge_base/PatternDatabase.h"
#include "program_knowledge_base/RelDatabase.h"
#include "shared/types/EntityType.h"

class PKB : public PKBQPSInterface, public PKBSPInterface {
 private:
  std::unique_ptr<EntityDatabase> ent_data;
  std::unique_ptr<RelDatabase> rel_data;
  std::unique_ptr<PatternDatabase> pat_data;

  // Helper functions
  std::unordered_set<std::string> GetIntersection(
      std::unordered_set<std::string> set1,
      std::unordered_set<std::string> set2);

 public:
  PKB();

  // ********** SP **********
  void InsertEntity(EntityType type, std::string value) override;
  void InsertEntity(EntityType type, AttrType attr_type,
                    std::string statement_number,
                    std::string attribute) override;
  void InsertRelation(RelationType rel_type, std::string s1_line_num,
                      std::string s2_line_num) override;
  void InsertAssignPattern(std::string statement_number,
                     std::string lhs, std::shared_ptr<TreeNode> rhs) override;
  void InsertCFGNode(std::string statement_num,
                     std::shared_ptr<CFGNode> node) override;

  std::unordered_set<std::string> GetProcedureModifies(
      std::string proc_name) override;
  std::unordered_set<std::string> GetProcedureUses(
      std::string proc_name) override;
  std::unordered_set<std::string> GetStatementModifies(
      std::string stmt) override;
  std::unordered_set<std::string> GetStatementUses(std::string stmt) override;

  // ********** QPS **********
  // ---------- ENTITIES ----------
  std::vector<std::string> GetEntitiesWithType(EntityType type) override;

  std::string ConvertEntityValueToAlias(std::string value, EntityType type,
                                        AttrType curr_attr_type,
                                        AttrType wanted_attr_type) override;

  std::vector<std::string> GetEntitiesMatchingAttrValue(
      EntityType type, AttrType attr_type, std::string value) override;

  std::vector<std::pair<std::string, std::string>>
  GetEntitiesWhereAttributesMatch(EntityType type_1, AttrType attr_type_1,
                                  EntityType type_2,
                                  AttrType attr_type_2) override;

  // ---------- RELATIONS ----------
  // 0 Declarations
  bool IsRelationTrueValueValue(std::string value_1, std::string value_2,
                                RelationType rel_type) override;
  bool IsRelationTrueValueWild(std::string value,
                               RelationType rel_type) override;
  bool IsRelationTrueWildValue(std::string value,
                               RelationType rel_type) override;
  bool IsRelationTrueWildWild(RelationType relation_type) override;

  // 1 Declarations
  std::vector<std::string> GetRelationSynonymWild(
      EntityType entity_type, RelationType rel_type) override;
  std::vector<std::string> GetRelationWildSynonym(
      EntityType entity_type, RelationType rel_type) override;
  std::vector<std::string> GetRelationSynonymValue(
      EntityType entity_type, std::string value,
      RelationType rel_type) override;
  std::vector<std::string> GetRelationValueSynonym(
      std::string value, EntityType entity_type,
      RelationType rel_type) override;

  // 2 Declarations
  std::vector<std::pair<std::string, std::string>> GetRelationSynonymSynonym(
      EntityType entity_type_1, EntityType entity_type_2,
      RelationType rel_type) override;

  // ---------- PATTERNS ----------
  std::vector<std::string> GetMatchingAssignStmts(
      std::shared_ptr<TreeNode> rhs_expr, MatchType match_type) override;
  std::vector<std::string> GetMatchingAssignStmts(
      std::string lhs_value, std::shared_ptr<TreeNode> rhs_expr,
      MatchType match_type) override;
  std::vector<std::pair<std::string, std::string>>
  GetMatchingAssignStmtLhsVarPairs(std::shared_ptr<TreeNode> rhs_expr,
                                   MatchType match_type) override;

  std::vector<std::string> GetContainerStmtsWithControlVar(
      EntityType container_stmt_type) override;

  std::vector<std::string> GetContainerStmtsWithGivenControlVar(
      EntityType container_stmt_type, std::string var_name) override;

  std::vector<std::pair<std::string, std::string>>
  GetContainerStmtControlVarPairs(EntityType container_stmt_type) override;
};
