#include <utility>

#include "EntityDatabase.h"

EntityDatabase::EntityDatabase() {
  entities[EntityType::PROCEDURE] =
      std::make_shared<std::unordered_set<std::string>>();
  entities[EntityType::STMT] =
      std::make_shared<std::unordered_set<std::string>>();
  entities[EntityType::READ] =
      std::make_shared<std::unordered_set<std::string>>();
  entities[EntityType::PRINT] =
      std::make_shared<std::unordered_set<std::string>>();
  entities[EntityType::ASSIGN] =
      std::make_shared<std::unordered_set<std::string>>();
  entities[EntityType::CALL] =
      std::make_shared<std::unordered_set<std::string>>();
  entities[EntityType::WHILE] =
      std::make_shared<std::unordered_set<std::string>>();
  entities[EntityType::IF] =
      std::make_shared<std::unordered_set<std::string>>();
  entities[EntityType::VARIABLE] =
      std::make_shared<std::unordered_set<std::string>>();
  entities[EntityType::CONSTANT] =
      std::make_shared<std::unordered_set<std::string>>();

  statementTypes = {EntityType::STMT,   EntityType::READ, EntityType::PRINT,
                    EntityType::ASSIGN, EntityType::CALL, EntityType::WHILE,
                    EntityType::IF};
}

std::unordered_set<std::string> EntityDatabase::GetUniqueAttributes(
    EntityType ent_type, AttrType attr_type) {
  if (AttrRef::IsDefaultAttribute(ent_type, attr_type)) {
    return *get(ent_type);
  }
  std::unordered_set<std::string> result;
  std::unordered_map<std::string, std::string> ent_attr =
      entity_attr_map[std::make_pair(ent_type, attr_type)];
  for (auto const& pair : ent_attr) {
    result.insert(pair.second);
  }
  return result;
}

void EntityDatabase::InsertEntity(EntityType type, std::string value) {
  (entities[type])->insert(value);
  if (statementTypes.find(type) != statementTypes.end()) {
    (entities[EntityType::STMT])->insert(value);
  }
}

void EntityDatabase::InsertEntity(EntityType type, AttrType attr_type,
                                  std::string ent, std::string attribute) {
  InsertEntity(type, ent);
  entity_attr_map[std::make_pair(type, attr_type)][ent] = attribute;
  attr_ent_map[std::make_pair(type, attr_type)][attribute].insert(ent);
}

std::shared_ptr<std::unordered_set<std::string>> EntityDatabase::get(
    EntityType type) {
  std::shared_ptr<std::unordered_set<std::string>> results = entities[type];
  return results;
}

// TODO(@tyanhan): Remove curr_attr_type
std::string EntityDatabase::ConvertEntityValueToAlias(
    std::string value, EntityType type, AttrType curr_attr_type,
    AttrType wanted_attr_type) {
  return entity_attr_map[std::make_pair(type, wanted_attr_type)][value];
}

std::vector<std::string> EntityDatabase::GetEntitiesMatchingAttrValue(
    EntityType type, AttrType attr_type, std::string value) {
  if (AttrRef::IsDefaultAttribute(type, attr_type)) {
    std::shared_ptr<std::unordered_set<std::string>> ents = get(type);
    if (ents->find(value) != ents->end()) {
      return std::vector<std::string>({value});
    }
    return std::vector<std::string>();
  }
  std::pair<EntityType, AttrType> key = std::make_pair(type, attr_type);
  std::unordered_set<std::string> set = attr_ent_map[key][value];
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
