#include "../../spa/src/program_knowledge_base/PKBQPSInterface.h"
#include "../../spa/src/program_knowledge_base/PKBSPInterface.h"
#include "source_processor/node/stmt_node/StmtNode.h"

class PkbStub : public PKBQPSInterface, public PkbApi {
 public:
  PkbStub();

  std::vector<std::string> PROCEDURES = {"procedure1", "procedure2",
                                         "procedure3"};
  std::vector<std::string> CONSTANTS = {"12", "13", "14", "15"};
  std::vector<std::string> VARIABLES = {"varX", "varY"};
  std::vector<std::string> STATEMENTS = {"1", "2", "3"};

  std::vector<std::string> REL_TYPE_STRINGS = {
      "FOLLOWS", "FOLLOWS_STAR", "PARENT",     "PARENT_STAR",
      "USES_P",  "USES_S",       "MODIFIES_P", "MODIFIES_S"};

  int insertEntityCallCount;
  int insertVariableCallCount;
  int insertConstantCallCount;
  int insertRelationCallCount;
  int insertFollowsCallCount;
  int insertParentCallCount;
  int insertUsesCallCount;
  int insertModifiesCallCount;
  int insertPatternCallCount;

  std::unordered_set<std::string> entitiesSet;

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
  void insertRelationCommon(RelationType type, std::string a, std::string b);

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
  void insertPattern(std::string statement_number, std::string lhs,
                     std::unordered_set<std::string> rhs) override;

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

  std::unordered_set<std::string> getProcedureUses(
      std::string procName) override;

  std::unordered_set<std::string> getProcedureModifies(
      std::string procName) override;
};
