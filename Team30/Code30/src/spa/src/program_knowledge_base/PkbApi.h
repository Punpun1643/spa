#pragma once
#include <source_processor/node/stmt_node/StmtNode.h>

#include <memory>
#include <optional>
#include <string>
#include <utility>
#include <vector>

#include "../query_processing_system/common/EntityType.h"
#include "RelationType.h"

class PkbApi {
 public:
  bool insertFollows(std::shared_ptr<StmtNode> stmt1,
                     std::shared_ptr<StmtNode> stmt2);

  virtual std::unique_ptr<std::vector<std::string>> getEntitiesWithType(
      EntityType type) = 0;

  // 0 Declarations
  // Relation(value, value) e.g. Follows (int, int)
  virtual bool isRelationTrue(std::string value_1, std::string value_2,
                              RelationType rel_type) = 0;
  // Relation(value, wild) e.g. Follows (int, _)
  virtual bool isRelationTrueGivenFirstValue(std::string value,
                                             RelationType rel_type) = 0;
  // Relation(wild, value) e.g. Follows(_, int)
  virtual bool isRelationTrueGivenSecondValue(std::string value,
                                              RelationType rel_type) = 0;
  // Relation(wild, wild) e.g. Follows(_, _)
  virtual bool isRelationTrueForAny(RelationType relation_type) = 0;

  // 1 Declarations
  // Relation(declaration, wild) e.g. Follows(s, _)
  virtual std::unique_ptr<std::vector<std::string>>
  getRelationValuesGivenFirstType(EntityType entity_type,
                                  RelationType rel_type) = 0;
  // Relation(wild, declaration) e.g. Follows(_, s)
  virtual std::unique_ptr<std::vector<std::string>>
  getRelationValuesGivenSecondType(EntityType entity_type,
                                   RelationType rel_type) = 0;
  // Relation(declaration, value) e.g. Follows(s, 5)
  virtual std::unique_ptr<std::vector<std::string>> getRelationValues(
      EntityType entity_type, std::string value, RelationType rel_type) = 0;
  // Relation(value, declaration) e.g. Follows(5, s)
  virtual std::unique_ptr<std::vector<std::string>> getRelationValues(
      std::string value, EntityType entity_type, RelationType rel_type) = 0;

  // 2 Declarations
  // Relation (declaration, declaration) e.g. Follows(s1, s2)
  virtual std::unique_ptr<std::vector<std::pair<std::string, std::string>>>
  getRelationValues(EntityType entity_type_1, EntityType entity_type_2,
                    RelationType rel_type) = 0;

  virtual ~PkbApi();  // so that the subclass destructors will be called
};
