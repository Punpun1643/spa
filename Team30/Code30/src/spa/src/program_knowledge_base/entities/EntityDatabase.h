#pragma once

#include <stdio.h>

#include <iostream>
#include <memory>
#include <string>
#include <unordered_map>
#include <unordered_set>
#include <utility>
#include <vector>

#include "program_knowledge_base/utils/PairHash.h"
#include "shared/types/AttrType.h"
#include "shared/types/EntityAttrPairings.h"
#include "shared/types/EntityType.h"

class EntityDatabase {
 private:
  std::unordered_map<EntityType, std::unordered_set<std::string>> entities;

  std::unordered_map<std::pair<EntityType, AttrType>,
                     std::unordered_map<std::string, std::string>, PairHash>
      entity_attr_map;

  std::unordered_map<
      std::pair<EntityType, AttrType>,
      std::unordered_map<std::string, std::unordered_set<std::string>>,
      PairHash>
      attr_ent_map;

  std::unordered_set<EntityType> statement_types;

  std::unordered_set<std::string> GetUniqueAttributes(EntityType ent_type,
                                                      AttrType attr_type);

 public:
  EntityDatabase();
  ~EntityDatabase() = default;
  std::unordered_set<std::string> Get(EntityType type);

  void InsertEntity(EntityType type, std::string value);

  void InsertEntity(EntityType type, AttrType attr_type,
                    std::string statement_number, std::string attribute);

  std::string ConvertEntityValueToAlias(std::string value, EntityType type,
                                        AttrType wanted_attr_type);

  std::vector<std::string> GetEntitiesMatchingAttrValue(EntityType type,
                                                        AttrType attr_type,
                                                        std::string value);

  std::vector<std::pair<std::string, std::string>>
  GetEntitiesWhereAttributesMatch(EntityType type_1, AttrType attr_type_1,
                                  EntityType type_2, AttrType attr_type_2);
};
