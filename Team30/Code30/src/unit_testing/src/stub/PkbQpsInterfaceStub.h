#include <cassert>

#include "../../../spa/src/program_knowledge_base/PKBQPSInterface.h"
#include "../../../spa/src/program_knowledge_base/PKBSPInterface.h"
#include "source_processor/node/stmt_node/StmtNode.h"

class PkbQpsInterfaceStub : public PKBQPSInterface {
 public:
  PkbQpsInterfaceStub();

  RelationType last_rel_passed = RelationType::FOLLOWS;

  // Select
  std::vector<std::string> const getAllOfTypeValues = {"x", "y", "z"};
  std::string converted_entity = "a";
  AttrType last_attr_type_passed = AttrType::VALUE;
  AttrType last_attr_type_2_passed = AttrType::VALUE;

  // SuchThat Clauses
  std::string last_value_passed;
  std::string last_value_2_passed;
  EntityType last_entity_type_passed = EntityType::IF;
  EntityType last_entity_type_2_passed = EntityType::IF;

  int valueValueCalls = 0;
  int valueWildCalls = 0;
  int wildValueCalls = 0;
  int wildWildCalls = 0;
  int synonymWildCalls = 0;
  int wildSynonymCalls = 0;
  int synonymValueCalls = 0;
  int valueSynonymCalls = 0;
  int synonymSynonymCalls = 0;

  bool const valueValueBool = false;
  bool const valueWildBool = false;
  bool const wildValueBool = true;
  bool const wildWildBool = true;
  std::vector<std::string> const synonymWildValues = {"1", "1", "3"};
  std::vector<std::string> const wildSynonymValues = {"a", "b", "c"};
  std::vector<std::string> const synonymValueValues = {"4", "5", "6"};
  std::vector<std::string> const valueSynonymValues = {"d", "e", "f"};
  std::vector<std::string> const synonymSynonymValues1 = {"42"};
  std::vector<std::string> const synonymSynonymValues2 = {"43"};
  std::vector<std::pair<std::string, std::string>> synonymSynonymValues =
      {std::make_pair(synonymSynonymValues1[0], synonymSynonymValues2[0])};

  // Pattern Clauses
  int patternWildCalls = 0;
  int patternValueCalls = 0;
  int patternDeclCalls = 0;
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
  std::vector<std::string> const patternWildValues = {"10", "20", "30"};
  std::vector<std::string> const patternValueValues = {"ab", "bb", "cb"};
  std::vector<std::string> const patternDeclValues1 = {"123"};
  std::vector<std::string> const patternDeclValues2 = {"345"};
  std::vector<std::pair<std::string, std::string>> const patternDeclValues = {
      std::make_pair(patternDeclValues1[0], patternDeclValues2[0])};

  // Entities
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

  // 0 Declarations - SuchThatClauses
  bool IsRelationTrueValueValue(std::string value_1, std::string value_2,
                                RelationType rel_type) override;
  bool IsRelationTrueValueWild(std::string value,
                               RelationType rel_type) override;
  bool IsRelationTrueWildValue(std::string value,
                               RelationType rel_type) override;
  bool IsRelationTrueWildWild(RelationType relation_type) override;

  // 1 Declarations - SuchThatClauses
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

  // 2 Declarations - SuchThatClauses
  std::vector<std::pair<std::string, std::string>> GetRelationSynonymSynonym(
      EntityType entity_type_1, EntityType entity_type_2,
      RelationType rel_type) override;

  // Pattern clause
  std::vector<std::string> GetPatternMatchesWildLhs(
      std::shared_ptr<TreeNode> rhs_expr, MatchType match_type) override;
  std::vector<std::string> GetPatternMatchesValueLhs(
      std::string lhs_value, std::shared_ptr<TreeNode> rhs_expr,
      MatchType match_type) override;
  std::vector<std::pair<std::string, std::string>>
  // 2 paired values - one for the implicit assign declaration, paired with
  // the variable declaration on the LHS of the assign stmt
  GetPatternMatchesSynonymLhs(std::shared_ptr<TreeNode> rhs_expr,
                              MatchType match_type) override;
};
