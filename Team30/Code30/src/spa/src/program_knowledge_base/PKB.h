#pragma once

#include <stdio.h>

#include <iostream>
#include <memory>
#include <optional>
#include <string>
#include <utility>
#include <vector>

#include "../query_processing_system/common/EntityType.h"
#include "../source_processor/node/stmt_node/StmtNode.h"
#include "PkbApi.h"
#include "program_knowledge_base/EntityDatabase.h"
#include "program_knowledge_base/RelDatabase.h"

using namespace std;

class PKB : public PkbApi {
  EntityDatabase entData;
  RelDatabase relData;

 public:
  PKB();
  bool insertFollows(std::shared_ptr<StmtNode> stmt1,
                     std::shared_ptr<StmtNode> stmt2);
  unique_ptr<vector<string>> getEntitiesWithType(EntityType type);
  std::optional<std::pair<int, int>> getFollows(int s1_line_num,
                                                EntityType s2_type);
  std::optional<std::pair<int, int>> getFollows(EntityType s1_type,
                                                int s2_line_num);
  std::unique_ptr<std::vector<std::pair<int, int>>> getFollows(
      EntityType s1_type, EntityType s2_type);

  // 0 Declarations
  bool isRelationTrue(string value_1, string value_2, RelationType rel_type);
  bool isRelationTrueGivenFirstValue(string value, RelationType rel_type);
  bool isRelationTrueGivenSecondValue(string value, RelationType rel_type);
  bool isRelationTrueForAny(RelationType relation_type);

  // 1 Declarations
  unique_ptr<vector<string>> getRelationValuesGivenFirstType(
      EntityType entity_type, RelationType rel_type);
  unique_ptr<vector<string>> getRelationValuesGivenSecondType(
      EntityType entity_type, RelationType rel_type);
  unique_ptr<vector<string>> getRelationValues(EntityType entity_type,
                                               string value,
                                               RelationType rel_type);
  unique_ptr<vector<string>> getRelationValues(string value,
                                               EntityType entity_type,
                                               RelationType rel_type);

  // 2 Declarations
  unique_ptr<vector<pair<string, string>>> getRelationValues(
      EntityType entity_type_1, EntityType entity_type_2,
      RelationType rel_type) = 0;
};