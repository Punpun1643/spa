#include "../../../spa/src/program_knowledge_base/PKBQPSInterface.h"
#include "../../../spa/src/program_knowledge_base/PKBSPInterface.h"
#include <cassert>
#include "source_processor/node/stmt_node/StmtNode.h"

class PkbQpsInterfaceStub : public PKBQPSInterface {
 public:
  PkbQpsInterfaceStub();

  RelationType last_rel_passed = RelationType::FOLLOWS;

  // Select
  std::vector<std::string> const getAllOfTypeValues = {"x", "y", "z"};

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
  std::vector<std::string> const synonymWildValues = {"1", "2", "3"};
  std::vector<std::string> const wildSynonymValues = {"a", "b", "c"};
  std::vector<std::string> const synonymValueValues = {"4", "5", "6"};
  std::vector<std::string> const valueSynonymValues = {"d", "e", "f"};
  std::vector<std::string> const synonymSynonymValues1 = {"42"};
  std::vector<std::string> const synonymSynonymValues2 = {"43"};
  std::vector<std::pair<std::string, std::string>> const synonymSynonymValues =
      {std::make_pair(synonymSynonymValues1[0], synonymSynonymValues2[0])};

  // Pattern Clauses
  int patternWildCalls = 0;
  int patternValueCalls = 0;
  int patternDeclCalls = 0;
  MatchType last_match_type_passed = MatchType::EXACT_MATCH;
  std::shared_ptr<TreeNode> last_rhs_expr_passed;

  // Select Clause
  std::vector<std::string> const patternWildValues = {"10", "20", "30"};
  std::vector<std::string> const patternValueValues = {"ab", "bb", "cb"};
  std::vector<std::string> const patternDeclValues1 = {"123"};
  std::vector<std::string> const patternDeclValues2 = {"345"};
  std::vector<std::pair<std::string, std::string>> const patternDeclValues = {
      std::make_pair(patternDeclValues1[0], patternDeclValues2[0])};

  // Entities
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

  // 0 Declarations - SuchThatClauses
  bool isRelationTrueValueValue(std::string value_1, std::string value_2,
                                RelationType rel_type) override;
  bool isRelationTrueValueWild(std::string value,
                               RelationType rel_type) override;
  bool isRelationTrueWildValue(std::string value,
                               RelationType rel_type) override;
  bool isRelationTrueWildWild(RelationType relation_type) override;

  // 1 Declarations - SuchThatClauses
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

  // 2 Declarations - SuchThatClauses
  std::unique_ptr<std::vector<std::pair<std::string, std::string>>>
  getRelationSynonymSynonym(EntityType entity_type_1, EntityType entity_type_2,
                            RelationType rel_type) override;

  // Pattern clause
  std::unique_ptr<std::vector<std::string>> getPatternMatchesWildLhs(
      std::shared_ptr<TreeNode> rhs_expr, MatchType match_type) override;
  std::unique_ptr<std::vector<std::string>> getPatternMatchesValueLhs(
      std::string lhs_value, std::shared_ptr<TreeNode> rhs_expr,
      MatchType match_type) override;
  std::unique_ptr<std::vector<std::pair<std::string, std::string>>>
  // 2 paired values - one for the implicit assign declaration, paired with
  // the variable declaration on the LHS of the assign stmt
  getPatternMatchesSynonymLhs(std::shared_ptr<TreeNode> rhs_expr,
                              MatchType match_type) override;

  // TO DELETE
  std::unique_ptr<std::vector<std::string>> getPatternMatchesWildLhs(
      std::string rhs_expr, MatchType expr_match_type) override;
  std::unique_ptr<std::vector<std::string>> getPatternMatchesValueLhs(
      std::string lhs_value, std::string rhs_expr,
      MatchType expr_match_type) override;

  std::unique_ptr<std::vector<std::pair<std::string, std::string>>>
  getPatternMatchesSynonymLhs(std::string rhs_expr,
                              MatchType expr_match_type) override;
};
