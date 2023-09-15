#include "PKB.h"

#include <stdio.h>

#include <iostream>
#include <optional>
#include <string>
#include <utility>
#include <vector>

#include "../query_processing_system/common/EntityType.h"
#include "../source_processor/node/ANode.h"
#include "EntityDatabase.h"
#include "PkbApi.h"
#include "RelDatabase.h"

PKB::PKB() {
  //    entData = new EntityDatabase();
  //    relData = new RelDatabase();
}

bool PKB::insertFollows(std::shared_ptr<StmtNode> stmt1,
                        std::shared_ptr<StmtNode> stmt2) {
  // entData.insert(stmt1);
  // entData.insert(stmt2);
  // relData.insert(Follows, stmt1, stmt2)
  return true;
}

std::unique_ptr<std::vector<std::string>> PKB::getEntitiesWithType(
    EntityType type) {
  std::vector<std::string> matchingEnts = {"foo"};
  return std::make_unique<std::vector<std::string>>(matchingEnts);
}

bool PKB::isRelationTrue(std::string value_1, std::string value_2,
                         RelationType rel_type) {
  return true;
}
bool PKB::isRelationTrueGivenFirstValue(std::string value,
                                        RelationType rel_type) {
  return false;
}
bool PKB::isRelationTrueGivenSecondValue(std::string value,
                                         RelationType rel_type) {
  return true;
}
bool PKB::isRelationTrueForAny(RelationType relation_type) { return false; }

std::unique_ptr<std::vector<std::string>> PKB::getRelationValuesGivenFirstType(
    EntityType entity_type, RelationType rel_type) {
  return std::make_unique<std::vector<std::string>>();  // empty
}
std::unique_ptr<std::vector<std::string>> PKB::getRelationValuesGivenSecondType(
    EntityType entity_type, RelationType rel_type) {
  return std::make_unique<std::vector<std::string>>();  // empty
}

std::unique_ptr<std::vector<std::string>> PKB::getRelationValues(
    EntityType entity_type, std::string value, RelationType rel_type) {
  return std::make_unique<std::vector<std::string>>();
}
std::unique_ptr<std::vector<std::string>> PKB::getRelationValues(
    std::string value, EntityType entity_type, RelationType rel_type) {
  return std::make_unique<std::vector<std::string>>();  // empty
}

std::unique_ptr<std::vector<std::pair<std::string, std::string>>>
PKB::getRelationValues(EntityType entity_type_1, EntityType entity_type_2,
                       RelationType rel_type) {
  return std::make_unique<std::vector<std::pair<std::string, std::string>>>();
}

// int PKB::setProcToAST(PROC p, TNode* r) { return 0; }

// TNode* PKB::getRootAST(PROC p) { return nullptr; }
