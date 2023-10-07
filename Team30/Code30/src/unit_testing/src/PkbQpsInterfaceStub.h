#include "../../spa/src/program_knowledge_base/PKBQPSInterface.h"
#include "../../spa/src/program_knowledge_base/PKBSPInterface.h"
#include "source_processor/node/stmt_node/StmtNode.h"

class PkbQpsInterfaceStub : public PKBQPSInterface {
 public:
  PkbQpsInterfaceStub();

  RelationType last_rel_passed = RelationType::FOLLOWS;
  int valueValueCalls = 0;
  int valueWildCalls = 0;
  int wildValueCalls = 0;
  int wildWildCalls = 0;
  int synonymWildCalls = 0;
  int wildSynonymCalls = 0;
  int synonymValueCalls = 0;
  int valueSynonymCalls = 0;
  int synonymSynonymCalls = 0;

  const bool valueValueBool = false;
  const bool valueWildBool = false;
  const bool wildValueBool = true;
  const bool wildWildBool = true;

  const std::vector<std::string> synonymWildValues = {"1","2","3"};
  const std::vector<std::string> wildSynonymValues = {"a","b","c"};
  const std::vector<std::string> synonymValueValues = {"4","5","6"};
  const std::vector<std::string> valueSynonymValues = {"d","e","f"};
  const std::vector<std::string> synonymSynonymValues1 = {"42"};
  const std::vector<std::string> synonymSynonymValues2 = {"43"};
  const std::vector<std::pair<std::string, std::string>> synonymSynonymValues = {std::make_pair(synonymSynonymValues1[0], synonymSynonymValues2[0])};


  // Select Clause
  std::unique_ptr<std::vector<std::string>> getEntitiesWithType(
      EntityType type) override;

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
  std::unique_ptr<std::vector<std::string>> getPatternMatchesWithWildLhs(
      std::string rhs_expr, MatchType expr_match_type) override;

  std::unique_ptr<std::vector<std::string>> getPatternMatchesWithLhsValue(
      std::string lhs_value, std::string rhs_expr,
      MatchType expr_match_type) override;

  // 2 paired values - for the implicit assign declaration, and the values for
  // the given lhs_entity_type
  std::unique_ptr<std::vector<std::pair<std::string, std::string>>>
  getPatternMatchesWithDeclarationLhs(std::string rhs_expr,
                                      MatchType expr_match_type) override;
};
