#pragma once
#include <source_processor/node/stmt_node/StmtNode.h>

#include <memory>
#include <optional>
#include <string>
#include <unordered_set>
#include <utility>
#include <vector>

#include "../query_processing_system/common/EntityType.h"
#include "RelationType.h"

using namespace std;

class PkbApi {
 public:
  virtual void insertEntity(EntityType type, string entity) = 0;
  virtual void insertRelation(RelationType type, string stmt1,
                              string stmt2) = 0;

  virtual unique_ptr<vector<string>> getEntitiesWithType(EntityType type) = 0;

  // 0 Declarations
  virtual bool isRelationTrue(std::string value_1, std::string value_2,
                              RelationType rel_type) = 0;
  virtual bool isRelationTrueGivenFirstValue(std::string value,
                                             RelationType rel_type) = 0;
  virtual bool isRelationTrueGivenSecondValue(std::string value,
                                              RelationType rel_type) = 0;
  virtual bool isRelationTrueForAny(RelationType relation_type) = 0;

  // 1 Declarations
  virtual std::unique_ptr<std::vector<std::string>>
  getRelationValuesGivenFirstType(EntityType entity_type,
                                  RelationType rel_type) = 0;
  virtual std::unique_ptr<std::vector<std::string>>
  getRelationValuesGivenSecondType(EntityType entity_type,
                                   RelationType rel_type) = 0;
  virtual std::unique_ptr<std::vector<std::string>> getRelationValues(
      EntityType entity_type, std::string value, RelationType rel_type) = 0;
  virtual std::unique_ptr<std::vector<std::string>> getRelationValues(
      std::string value, EntityType entity_type, RelationType rel_type) = 0;

  // 2 Declarations
  virtual std::unique_ptr<std::vector<std::pair<std::string, std::string>>>
  getRelationValues(EntityType entity_type_1, EntityType entity_type_2,
                    RelationType rel_type) = 0;

  // OLD FUNCTIONS - DELETE WHEN QPS EVALUATOR UPDATED.

  virtual std::optional<std::pair<int, int>> getFollows(int s1_line_num,
                                                        EntityType s2_type) = 0;
  virtual std::optional<std::pair<int, int>> getFollows(EntityType s1_type,
                                                        int s2_line_num) = 0;
  virtual std::unique_ptr<std::vector<std::pair<int, int>>> getFollows(
      EntityType s1_type, EntityType s2_type) = 0;

  virtual ~PkbApi();  // so that the subclass destructors will be called
};
