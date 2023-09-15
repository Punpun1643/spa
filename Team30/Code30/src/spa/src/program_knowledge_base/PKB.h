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
  unique_ptr<EntityDatabase> entData;
  unique_ptr<RelDatabase> relData;
  unordered_map<RelationType, vector<RelationType>> relatedTables;

 public:
  PKB();
  ~PKB() = default;

  void insertEntity(EntityType type, std::string entity);
  void insertRelation(RelationType rel_type, std::string s1_line_num,
                      std::string s2_line_num);
  void insertRelation(RelationType rel_type, std::string s_line_num,
                              EntityType ent_type, std::string enity);
  void insertRelation(RelationType rel_type, EntityType ent_type,
                              std::string entity, std::string s_line_num);
  void insertRelation(RelationType rel_type, EntityType ent_type1,
                              std::string entity1, EntityType ent_type2,
                              std::string entity2);

  unique_ptr<vector<string>> getEntitiesWithType(EntityType type);

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
      RelationType rel_type);

  // ---------- Delete after migration ------------------
  std::optional<std::pair<int, int>> getFollows(int s1_line_num,
                                                EntityType s2_type);
  std::optional<std::pair<int, int>> getFollows(EntityType s1_type,
                                                int s2_line_num);
  std::unique_ptr<std::vector<std::pair<int, int>>> getFollows(
      EntityType s1_type, EntityType s2_type);
};