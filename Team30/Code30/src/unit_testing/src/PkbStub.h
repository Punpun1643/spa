#include "../../spa/src/program_knowledge_base/PkbApi.h"

class PkbStub : public PkbApi {
 public:
  std::vector<std::string> PROCEDURES = {"procedure1", "procedure2",
                                         "procedure3"};
  std::vector<std::string> CONSTANTS = {"12", "13", "14", "15"};
  std::vector<std::string> VARIABLES = {"varX"};
  std::vector<std::string> STATEMENTS = {"1", "2", "3"};

  int insertFollowsCallCount;
  PkbStub();

  bool insertFollows(std::shared_ptr<StmtNode> stmt1,
                     std::shared_ptr<StmtNode> stmt2) override;

  // Select Clause
  std::unique_ptr<std::vector<std::string>> getEntitiesWithType(
      EntityType type) override;

  // 0 Declarations - SuchThatClauses
  bool isRelationTrue(std::string value_1, std::string value_2,
                      RelationType rel_type) override;
  bool isRelationTrueGivenFirstValue(std::string value, RelationType rel_type) override;
  bool isRelationTrueGivenSecondValue(std::string value, RelationType rel_type) override;
  bool isRelationTrueForAny(RelationType relation_type) override;

  // 1 Declarations - SuchThatClauses
  std::unique_ptr<std::vector<std::string>> getRelationValuesGivenFirstType(
      EntityType entity_type, RelationType rel_type) override;
  std::unique_ptr<std::vector<std::string>> getRelationValuesGivenSecondType(
      EntityType entity_type, RelationType rel_type) override;
  std::unique_ptr<std::vector<std::string>> getRelationValues(
      EntityType entity_type, std::string value, RelationType rel_type) override;
  std::unique_ptr<std::vector<std::string>> getRelationValues(
      std::string value, EntityType entity_type, RelationType rel_type) override;

  // 2 Declarations - SuchThatClauses
  std::unique_ptr<std::vector<std::pair<std::string, std::string>>>
  getRelationValues(EntityType entity_type_1, EntityType entity_type_2,
                    RelationType rel_type) override;
};
