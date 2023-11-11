#include "EntityDatabase.h"

#include <utility>

EntityDatabase::EntityDatabase() {
  entities[EntityType::PROCEDURE] = std::unordered_set<std::string>();
  entities[EntityType::STMT] = std::unordered_set<std::string>();
  entities[EntityType::READ] = std::unordered_set<std::string>();
  entities[EntityType::PRINT] = std::unordered_set<std::string>();
  entities[EntityType::ASSIGN] = std::unordered_set<std::string>();
  entities[EntityType::CALL] = std::unordered_set<std::string>();
  entities[EntityType::WHILE] = std::unordered_set<std::string>();
  entities[EntityType::IF] = std::unordered_set<std::string>();
  entities[EntityType::VARIABLE] = std::unordered_set<std::string>();
  entities[EntityType::CONSTANT] = std::unordered_set<std::string>();

  statement_types = {EntityType::STMT,   EntityType::READ, EntityType::PRINT,
                     EntityType::ASSIGN, EntityType::CALL, EntityType::WHILE,
                     EntityType::IF};
}

std::unordered_set<std::string> EntityDatabase::GetUniqueAttributes(
    EntityType ent_type, AttrType attr_type) {
  if (EntityAttrPairings::IsDefaultPair(ent_type, attr_type)) {
    return Get(ent_type);
  }

  std::pair key = std::make_pair(ent_type, attr_type);
  if (entity_attr_map.find(key) == entity_attr_map.end()) {
    return std::unordered_set<std::string>();
  }

  std::unordered_set<std::string> result;
  for (auto const& ent_attr : entity_attr_map.at(key)) {
    result.insert(ent_attr.second);
  }

  return result;
}

void EntityDatabase::InsertEntity(EntityType type, std::string value) {
  (entities[type]).insert(value);
  if (statement_types.find(type) != statement_types.end()) {
    (entities[EntityType::STMT]).insert(value);
  }
}

void EntityDatabase::InsertEntity(EntityType type, AttrType attr_type,
                                  std::string ent, std::string attribute) {
  InsertEntity(type, ent);
  entity_attr_map[std::make_pair(type, attr_type)][ent] = attribute;
  attr_ent_map[std::make_pair(type, attr_type)][attribute].insert(ent);
}

std::unordered_set<std::string>& EntityDatabase::Get(
    EntityType type) {
  return entities.at(type);
}

// TODO(@tyanhan): Remove curr_attr_type
std::string EntityDatabase::ConvertEntityValueToAlias(
    std::string value, EntityType type, AttrType wanted_attr_type) {
  std::pair key = std::make_pair(type, wanted_attr_type);

  if (entity_attr_map.find(key) == entity_attr_map.end() ||
      entity_attr_map.at(key).find(value) == entity_attr_map.at(key).end()) {
    throw std::runtime_error("Entity does not have the attribute type");
  }

  return entity_attr_map.at(key).at(value);
}

std::vector<std::string> EntityDatabase::GetEntitiesMatchingAttrValue(
    EntityType type, AttrType attr_type, std::string value) {
  if (EntityAttrPairings::IsDefaultPair(type, attr_type)) {
    std::unordered_set<std::string> ents = Get(type);
    if (ents.find(value) != ents.end()) {
      return std::vector<std::string>({value});
    }
    return std::vector<std::string>();
  }
  std::pair<EntityType, AttrType> key = std::make_pair(type, attr_type);

  if (attr_ent_map.find(key) == attr_ent_map.end() ||
      attr_ent_map.at(key).find(value) == attr_ent_map.at(key).end()) {
    return std::vector<std::string>();
  }

  std::unordered_set<std::string> set = attr_ent_map.at(key).at(value);
  return std::vector<std::string>(set.begin(), set.end());
}

std::vector<std::pair<std::string, std::string>>
EntityDatabase::GetEntitiesWhereAttributesMatch(EntityType type_1,
                                                AttrType attr_type_1,
                                                EntityType type_2,
                                                AttrType attr_type_2) {
  std::vector<std::pair<std::string, std::string>> res;

  std::unordered_set<std::string> unique_attributes_1 =
      GetUniqueAttributes(type_1, attr_type_1);
  std::unordered_set<std::string> unique_attributes_2 =
      GetUniqueAttributes(type_2, attr_type_2);

  std::unordered_set<std::string> matching_attributes;
  for (auto const& val : unique_attributes_1) {
    if (unique_attributes_2.find(val) != unique_attributes_2.end()) {
      matching_attributes.insert(val);
    }
  }

  for (auto const& attr : matching_attributes) {
    std::vector<std::string> ent_vec_1 =
        GetEntitiesMatchingAttrValue(type_1, attr_type_1, attr);
    std::vector<std::string> ent_vec_2 =
        GetEntitiesMatchingAttrValue(type_2, attr_type_2, attr);

    for (auto const& ent_1 : ent_vec_1) {
      for (auto const& ent_2 : ent_vec_2) {
        res.push_back(std::make_pair(ent_1, ent_2));
      }
    }
  }
  return res;
}
