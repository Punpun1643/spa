#include "PkbStub.h"

PkbStub::PkbStub() : insertFollowsCallCount(0){};

// ----- REMOVE AFTER SWITCHING -----
bool PkbStub::insertFollows(std::shared_ptr<StmtNode> stmt1,
                            std::shared_ptr<StmtNode> stmt2) {
  ++insertFollowsCallCount;
  return true;
}
// ----------------------------------

void PkbStub::insertEntity(EntityType type, std::string entity){};
void PkbStub::insertRelation(RelationType type, std::string stmt1,
                             std::string stmt2){};
void PkbStub::insertRelation(RelationType rel_type, std::string s_line_num,
                             EntityType ent_type, std::string enity){};
void PkbStub::insertRelation(RelationType rel_type, EntityType ent_type,
                             std::string entity, std::string s_line_num){};
void PkbStub::insertRelation(RelationType rel_type, EntityType ent_type1,
                             std::string entity1, EntityType ent_type2,
                             std::string entity2){};

std::unique_ptr<std::vector<std::string>> PkbStub::getEntitiesWithType(
    EntityType type) {
  std::unique_ptr<std::vector<std::string>> output =
      std::make_unique<std::vector<std::string>>();

  if (type == PROCEDURE) {
    *output = PROCEDURES;
  } else if (type == CONSTANT) {
    *output = CONSTANTS;
  } else if (type == VARIABLE) {
    *output = VARIABLES;
  } else {  // statement type
    *output = STATEMENTS;
  }
  return output;
}

bool PkbStub::isRelationTrue(std::string value_1, std::string value_2,
                             RelationType rel_type) {
  return true;
}
bool PkbStub::isRelationTrueGivenFirstValue(std::string value,
                                            RelationType rel_type) {
  return false;
}
bool PkbStub::isRelationTrueGivenSecondValue(std::string value,
                                             RelationType rel_type) {
  return true;
}
bool PkbStub::isRelationTrueForAny(RelationType relation_type) { return false; }

std::unique_ptr<std::vector<std::string>>
PkbStub::getRelationValuesGivenFirstType(EntityType entity_type,
                                         RelationType rel_type) {
  return std::make_unique<std::vector<std::string>>();  // empty
}
std::unique_ptr<std::vector<std::string>>
PkbStub::getRelationValuesGivenSecondType(EntityType entity_type,
                                          RelationType rel_type) {
  std::vector<std::string> vec = {"1", "3", "5", "7", "9"};
  return std::make_unique<std::vector<std::string>>(vec);
}

std::unique_ptr<std::vector<std::string>> PkbStub::getRelationValues(
    EntityType entity_type, std::string value, RelationType rel_type) {
  std::vector<std::string> vec = {"2", "4", "6", "8", "10"};
  return std::make_unique<std::vector<std::string>>(vec);
}
std::unique_ptr<std::vector<std::string>> PkbStub::getRelationValues(
    std::string value, EntityType entity_type, RelationType rel_type) {
  return std::make_unique<std::vector<std::string>>();  // empty
}

std::unique_ptr<std::vector<std::pair<std::string, std::string>>>
PkbStub::getRelationValues(EntityType entity_type_1, EntityType entity_type_2,
                           RelationType rel_type) {
  if (entity_type_1 == STMT && entity_type_2 == STMT) {
    auto result =
        std::make_unique<std::vector<std::pair<std::string, std::string>>>();
    result->push_back(std::make_pair("5", "10"));
    result->push_back(std::make_pair("9", "1"));
    result->push_back(std::make_pair("2", "2"));
    result->push_back(std::make_pair("2", "1"));
    return result;
  } else {
    return std::make_unique<std::vector<std::pair<std::string, std::string>>>();
  }
}
