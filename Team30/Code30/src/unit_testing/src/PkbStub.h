#include "../../spa/src/program_knowledge_base/PkbApi.h"

class PkbStub : public PkbApi {
 public:
  PkbStub();

  std::vector<std::string> PROCEDURES = {"procedure1", "procedure2",
                                         "procedure3"};
  std::vector<std::string> CONSTANTS = {"12", "13", "14", "15"};
  std::vector<std::string> VARIABLES = {"varX"};
  std::vector<std::string> STATEMENTS = {"1", "2", "3"};

  int insertEntityCallCount;
  int insertRelationCallCount;
  int insertFollowsCallCount;
  int insertParentCallCount;
  int insertUsesCallCount;
  int insertModifiesCallCount;

  void insertEntity(EntityType type, std::string entity) override;
  void insertRelation(RelationType rel_type, std::string s1_line_num,
                      std::string s2_line_num) override;
  void insertRelation(RelationType rel_type, std::string s_line_num,
                      EntityType ent_type, std::string enity) override;
  void insertRelation(RelationType rel_type, EntityType ent_type,
                      std::string entity, std::string s_line_num) override;
  void insertRelation(RelationType rel_type, EntityType ent_type1,
                      std::string entity1, EntityType ent_type2,
                      std::string entity2) override;
  void insertRelationCommon(RelationType type);

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
      std::string value, EntityType entity_type,
      RelationType rel_type) override;

  // 2 Declarations - SuchThatClauses
  std::unique_ptr<std::vector<std::pair<std::string, std::string>>>
  getRelationValues(EntityType entity_type_1, EntityType entity_type_2,
                    RelationType rel_type) override;
};
