#pragma once
#include <source_processor/node/stmt_node/StmtNode.h>

#include <memory>
#include <optional>
#include <string>
#include <utility>
#include <vector>

#include "../query_processing_system/common/EntityType.h"
#include "program_knowledge_base/RelationType.h"

class PkbApi {
 public:
  // Cast integers to strings
  void insertEntity(EntityType type, std::string entity);

  // Relation (integer, integer)
  void insertRelation(RelationType rel_type, std::string s1_line_num,
                      std::string s2_line_num);

  // Relation (integer, EntityType)
  void insertRelation(RelationType rel_type, std::string s_line_num,
                      EntityType ent_type, std::string enity);

  // Relation (EntityType, integer)
  void insertRelation(RelationType rel_type, EntityType ent_type,
                      std::string entity, std::string s_line_num);

  // Relation (EntityType, EntityType)
  void insertRelation(RelationType rel_type, EntityType ent_type1,
                      std::string entity1, EntityType ent_type2,
                      std::string entity2);

  virtual std::unique_ptr<std::vector<std::string>> getEntitiesWithType(
      EntityType type) = 0;
  virtual std::optional<std::pair<int, int>> getFollows(int s1_line_num,
                                                        EntityType s2_type) = 0;
  virtual std::optional<std::pair<int, int>> getFollows(EntityType s1_type,
                                                        int s2_line_num) = 0;
  virtual std::unique_ptr<std::vector<std::pair<int, int>>> getFollows(
      EntityType s1_type, EntityType s2_type) = 0;
  virtual ~PkbApi();  // so that the subclass destructors will be called
};
