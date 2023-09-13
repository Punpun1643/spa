#include "PKB.h"

#include <stdio.h>

#include <iostream>
#include <optional>
#include <string>
#include <utility>
#include <vector>

#include "../query_processing_system/common/EntityType.h"
#include "../source_processor/node/ANode.h"
#include "PkbApi.h"
#include "program_knowledge_base/EntityDatabase.h"
#include "program_knowledge_base/RelDatabase.h"

using namespace std;

PKB::PKB() {
  entData = EntityDatabase();
  relData = RelDatabase();
}

unique_ptr<vector<string>> PKB::getEntitiesWithType(EntityType type) {
  unordered_set<string> e = entData.get(type);
  vector<string> v(e.begin(), e.end());
  return make_unique<vector<string>>(v);
};

//// 0 Declarations
bool PKB::isRelationTrue(string value_1, string value_2,
                         RelationType rel_type) {
  shared_ptr<BaseTable> t = relData.getTable(rel_type);
  return t->isRelated(stoi(value_1), stoi(value_2));
};

bool PKB::isRelationTrueGivenFirstValue(std::string value,
                                        RelationType rel_type) {
  shared_ptr<BaseTable> t = relData.getTable(rel_type);
}
  // bool PKB::isRelationTrueGivenSecondValue(std::string value,
//                                          RelationType rel_type) = 0;
// bool PKB::isRelationTrueForAny(RelationType relation_type) = 0;
//
//// 1 Declarations
// unique_ptr<vector<string>> PKB::getRelationValuesGivenFirstType(
//     EntityType entity_type, RelationType rel_type) = 0;
//
//
// unique_ptr<vector<string>> PKB::getRelationValuesGivenSecondType(EntityType
// entity_type,
//                                  RelationType rel_type) = 0;
//
//
// unique_ptr<vector<string>> PKB::getRelationValues(
//     EntityType entity_type, std::string value, RelationType rel_type) = 0;
//
// unique_ptr<vector<string>> getRelationValues(
//     string value, EntityType entity_type, RelationType rel_type) = 0;
//
//// 2 Declarations
// virtual std::unique_ptr<std::vector<std::pair<std::string, std::string>>>
// getRelationValues(EntityType entity_type_1, EntityType entity_type_2,
//                   RelationType rel_type) = 0;

bool PKB::insertFollows(std::shared_ptr<StmtNode> stmt1,
                        std::shared_ptr<StmtNode> stmt2) {
  // entData.insert(stmt1);
  // entData.insert(stmt2);
  // relData.insert(Follows, stmt1, stmt2)
  return true;
}

std::optional<std::pair<int, int>> PKB::getFollows(int s1_line_num,
                                                   EntityType s2_type) {
  // Code logic to be implemented here
  std::pair<int, int> matchingPair = std::make_pair(1, 2);
  std::optional<std::pair<int, int>> returnPair = matchingPair;
  return returnPair;
}

std::optional<std::pair<int, int>> PKB::getFollows(EntityType s1_type,
                                                   int s2_line_num) {
  // Database logic to be added here
  std::pair<int, int> matchPair = std::make_pair(3, 4);
  std::optional<std::pair<int, int>> returnPair = matchPair;
  return returnPair;
}

std::unique_ptr<std::vector<std::pair<int, int>>> PKB::getFollows(
    EntityType s1_type, EntityType s2_type) {
  // Database logic to be added here
  std::pair<int, int> matchPair = std::make_pair(5, 6);
  std::vector<std::pair<int, int>> pairs = {matchPair};
  return std::make_unique<std::vector<std::pair<int, int>>>(pairs);
}

// int PKB::setProcToAST(PROC p, TNode* r) { return 0; }

// TNode* PKB::getRootAST(PROC p) { return nullptr; }
