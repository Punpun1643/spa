#pragma once

#include <string>
#include <unordered_map>

#include "AttrType.h"
#include "EntityType.h"

class EntityAttrPairings {
 private:
  static std::unordered_map<EntityType, AttrType> const DEFAULT_ATTR_TYPE;
  static std::unordered_map<EntityType, AttrType> const ATTR_TYPE_ALIASES;

 public:
  static bool IsDefaultPair(EntityType ent_type, AttrType attr_type);
  static bool IsAliasPair(EntityType ent_type, AttrType attr_type);
  static AttrType GetDefaultAttrType(EntityType ent_type);
};;