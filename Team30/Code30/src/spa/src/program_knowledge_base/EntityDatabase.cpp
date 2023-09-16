#include "EntityDatabase.h"

#include <stdio.h>

#include <iostream>
#include <string>
#include <unordered_map>
#include <unordered_set>
#include <vector>

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
};

void EntityDatabase::insert(EntityType type, std::string value) {
  (entities[type])->insert(value);
  if (statementTypes.find(type) != statementTypes.end()) {
    (entities[EntityType::STMT])->insert(value);
  };
};

std::shared_ptr<std::unordered_set<std::string>> EntityDatabase::get(
    EntityType type) {
  std::shared_ptr<std::unordered_set<std::string>> results = entities[type];
  return results;
};