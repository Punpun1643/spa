#include <cassert>

#include "../../../spa/src/program_knowledge_base/PKBQPSInterface.h"
#include "../../../spa/src/program_knowledge_base/PKBSPInterface.h"
#include "source_processor/node/stmt_node/StmtNode.h"

class PkbQpsInterfaceStub : public PKBQPSInterface {
 public:
  PkbQpsInterfaceStub();

  RelationType last_rel_passed = RelationType::FOLLOWS;

  // Select
  std::vector<std::string> get_all_of_type_values = {"x", "y", "z"};
  std::string converted_entity = "a";
  AttrType last_attr_type_passed = AttrType::VALUE;
  AttrType last_attr_type_2_passed = AttrType::VALUE;

  // SuchThat Clauses
  std::string last_value_passed;
  std::string last_value_2_passed;
  EntityType last_entity_type_passed = EntityType::IF;
  EntityType last_entity_type_2_passed = EntityType::IF;

  int value_value_calls = 0;
  int value_wild_calls = 0;
  int wild_value_calls = 0;
  int wild_wild_calls = 0;
  int synonym_wild_calls = 0;
  int wild_synonym_calls = 0;
  int synonym_value_calls = 0;
  int value_synonym_calls = 0;
  int synonym_synonym_calls = 0;

  bool const value_value_bool = false;
  bool const value_wild_bool = false;
  bool const wild_value_bool = true;
  bool const wild_wild_bool = true;
  std::vector<std::string> synonym_wild_values = {"1", "1", "3"};
  std::vector<std::string> const wild_synonym_values = {"a", "b", "c"};
  std::vector<std::string> const synonym_value_values = {"4", "5", "6"};
  std::vector<std::string> const value_synonym_values = {"d", "e", "f"};
  std::vector<std::string> const synonym_synonym_values_1 = {"42"};
  std::vector<std::string> const synonym_synonym_values_2 = {"43"};
  std::vector<std::pair<std::string, std::string>> synonym_synonym_values = {
      std::make_pair(synonym_synonym_values_1[0], synonym_synonym_values_2[0])};

  // Pattern Clauses
  int pattern_assign_wild_calls = 0;
  int pattern_assign_value_calls = 0;
  int pattern_assign_decl_calls = 0;
  MatchType last_match_type_passed = MatchType::EXACT_MATCH;
  std::shared_ptr<TreeNode> last_rhs_expr_passed;

  // With Clause
  std::vector<std::string> const attr_value_matches = {"alpha", "beta",
                                                       "delta"};
  std::vector<std::string> const attr_pair_matches_1 = {"72"};
  std::vector<std::string> const attr_pair_matches_2 = {"74"};
  std::vector<std::pair<std::string, std::string>> const attr_pair_matches = {
      std::make_pair(attr_pair_matches_1[0], attr_pair_matches_2[0])};

  // Select Clause
  std::vector<std::string> const pattern_assign_wild_values = {"10", "20",
                                                               "30"};
  std::vector<std::string> const pattern_assign_value_values = {"ab", "bb",
                                                                "cb"};
  std::vector<std::string> const pattern_assign_decl_values_1 = {"123"};
  std::vector<std::string> const pattern_assign_decl_values_2 = {"345"};
  std::vector<std::pair<std::string, std::string>> const
      pattern_assign_decl_values = {std::make_pair(
          pattern_assign_decl_values_1[0], pattern_assign_decl_values_2[0])};

  std::vector<std::string> const pattern_container_wild_values = {"18", "19",
                                                                  "20"};
  std::vector<std::string> const pattern_container_value_values = {"21", "24",
                                                                   "25"};
  std::vector<std::string> const pattern_container_decl_values_1 = {"22"};
  std::vector<std::string> const pattern_container_decl_values_2 = {"varX"};
  std::vector<std::pair<std::string, std::string>> const
      pattern_container_decl_values = {
          std::make_pair(pattern_container_decl_values_1[0],
                         pattern_container_decl_values_2[0])};

  // Entities
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

  // 0 Declarations - SuchThatClauses
  bool IsRelationTrueValueValue(std::string const& value_1,
                                std::string const& value_2,
                                RelationType rel_type) override;
  bool IsRelationTrueValueWild(std::string const& value,
                               RelationType rel_type) override;
  bool IsRelationTrueWildValue(std::string const& value,
                               RelationType rel_type) override;
  bool IsRelationTrueWildWild(RelationType relation_type) override;

  // 1 Declarations - SuchThatClauses
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

  // 2 Declarations - SuchThatClauses
  std::vector<std::pair<std::string, std::string>> GetRelationSynonymSynonym(
      EntityType entity_type_1, EntityType entity_type_2,
      RelationType rel_type) override;
  std::vector<std::pair<std::string, std::string>> GetRelationSynonymSynonym(
      EntityType entity_type_1, EntityType entity_type_2, RelationType rel_type,
      std::unordered_set<std::string> syn_1_possible_values,
      std::unordered_set<std::string> syn_2_possible_values) override;

  // Pattern clause
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
