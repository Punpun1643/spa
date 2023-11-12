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
#include "program_knowledge_base/entities/EntityDatabase.h"
#include "program_knowledge_base/patterns/PatternDatabase.h"
#include "program_knowledge_base/relations/RelDatabase.h"
#include "shared/types/EntityType.h"

class PKB : public PKBQPSInterface, public PKBSPInterface {
 private:
  std::unique_ptr<EntityDatabase> ent_data;
  std::unique_ptr<RelDatabase> rel_data;
  std::unique_ptr<PatternDatabase> pat_data;

  // Helper functions
  std::unordered_set<std::string> GetIntersectionHelper(
      std::unordered_set<std::string> const& larger_set,
      std::unordered_set<std::string> const& smaller_set);

  std::unordered_set<std::string> GetIntersection(
      std::unordered_set<std::string> const& set1,
      std::unordered_set<std::string> const& set2);

 public:
  PKB();

  // ********** SP **********
  void InsertEntity(EntityType type, std::string const& value) override;
  void InsertEntity(EntityType type, AttrType attr_type,
                    std::string const& statement_number,
                    std::string const& attribute) override;
  void InsertRelation(RelationType rel_type, std::string const& input1,
                      std::string const& input2) override;
  void InsertAssignPattern(std::string const& statement_number,
                           std::string const& lhs,
                           std::shared_ptr<TreeNode> const& rhs) override;
  void InsertCondVarPattern(EntityType type,
                            std::string const& statement_number,
                            std::string const& var) override;
  void InsertCFGNode(std::string const& statement_num,
                     std::shared_ptr<CFGNode> const& node) override;

  std::unordered_set<std::string> GetProcedureModifies(
      std::string const& proc_name) override;
  std::unordered_set<std::string> GetProcedureUses(
      std::string const& proc_name) override;
  std::unordered_set<std::string> GetStatementModifies(
      std::string const& stmt) override;
  std::unordered_set<std::string> GetStatementUses(
      std::string const& stmt) override;

  // ********** QPS **********
  // ---------- ENTITIES ----------
  std::vector<std::string> GetEntitiesWithType(EntityType type) override;

  std::string ConvertEntityValueToAlias(std::string const& value,
                                        EntityType type,
                                        AttrType wanted_attr_type) override;

  std::vector<std::string> GetEntitiesMatchingAttrValue(
      EntityType type, AttrType attr_type, std::string const& value) override;

  std::vector<std::pair<std::string, std::string>>
  GetEntitiesWhereAttributesMatch(EntityType type_1, AttrType attr_type_1,
                                  EntityType type_2,
                                  AttrType attr_type_2) override;

  // ---------- RELATIONS ----------
  // 0 Declarations
  bool IsRelationTrueValueValue(std::string const& value_1,
                                std::string const& value_2,
                                RelationType rel_type) override;
  bool IsRelationTrueValueWild(std::string const& value,
                               RelationType rel_type) override;
  bool IsRelationTrueWildValue(std::string const& value,
                               RelationType rel_type) override;
  bool IsRelationTrueWildWild(RelationType relation_type) override;

  // 1 Declarations
  std::vector<std::string> GetRelationSynonymWild(
      EntityType entity_type, RelationType rel_type) override;
  std::vector<std::string> GetRelationWildSynonym(
      EntityType entity_type, RelationType rel_type) override;
  std::vector<std::string> GetRelationSynonymValue(
      EntityType entity_type, std::string const& value,
      RelationType rel_type) override;
  std::vector<std::string> GetRelationValueSynonym(
      std::string const& value, EntityType entity_type,
      RelationType rel_type) override;

  // 2 Declarations
  std::vector<std::pair<std::string, std::string>> GetRelationSynonymSynonym(
      EntityType entity_type_1, EntityType entity_type_2,
      RelationType rel_type) override;

  std::vector<std::pair<std::string, std::string>> GetRelationSynonymSynonym(
      EntityType entity_type_1, EntityType entity_type_2, RelationType rel_type,
      std::unordered_set<std::string> const& syn_1_possible_values,
      std::unordered_set<std::string> const& syn_2_possible_values) override;

  // ---------- PATTERNS ----------
  std::vector<std::string> GetMatchingAssignStmts(
      std::shared_ptr<TreeNode> const& rhs_expr, MatchType match_type) override;
  std::vector<std::string> GetMatchingAssignStmts(
      std::string const& lhs_value, std::shared_ptr<TreeNode> const& rhs_expr,
      MatchType match_type) override;
  std::vector<std::pair<std::string, std::string>>
  GetMatchingAssignStmtLhsVarPairs(std::shared_ptr<TreeNode> const& rhs_expr,
                                   MatchType match_type) override;
  std::vector<std::pair<std::string, std::string>>
  GetMatchingAssignStmtLhsVarPairs(
      std::shared_ptr<TreeNode> const& rhs_expr, MatchType match_type,
      std::unordered_set<std::string> const& assign_syn_possible_values,
      std::unordered_set<std::string> const& var_syn_possible_values) override;

  std::vector<std::string> GetContainerStmtsWithControlVar(
      EntityType container_stmt_type) override;

  std::vector<std::string> GetContainerStmtsWithGivenControlVar(
      EntityType container_stmt_type, std::string const& var_name) override;

  std::vector<std::pair<std::string, std::string>>
  GetContainerStmtControlVarPairs(EntityType container_stmt_type) override;

  std::vector<std::pair<std::string, std::string>>
  GetContainerStmtControlVarPairs(
      EntityType container_stmt_type,
      std::unordered_set<std::string> const& container_syn_possible_values,
      std::unordered_set<std::string> const& control_var_possible_values)
      override;
};
