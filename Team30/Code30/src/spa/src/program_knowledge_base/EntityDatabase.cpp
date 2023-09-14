#include "EntityDatabase.h"

#include <stdio.h>

#include <iostream>
#include <string>
#include <unordered_map>
#include <unordered_set>
#include <vector>

using namespace std;

EntityDatabase::EntityDatabase() {
  unordered_map<EntityType, unordered_set<string>> entities;
  unordered_map<EntityValueType, unordered_set<string>> entities_by_value_type;

  entities[EntityType::PROCEDURE] = {};
  entities[EntityType::STMT] = {};
  entities[EntityType::READ] = {};
  entities[EntityType::PRINT] = {};
  entities[EntityType::ASSIGN] = {};
  entities[EntityType::CALL] = {};
  entities[EntityType::WHILE] = {};
  entities[EntityType::IF] = {};
  entities[EntityType::VARIABLE] = {};
  entities[EntityType::CONSTANT] = {};

  entities_by_value_type[EntityValueType::STATEMENT_NUMBER] = {};
  entities_by_value_type[EntityValueType::VALUE] = {};

  unordered_map<EntityType, EntityValueType> entityToValueType = {
      {EntityType::PROCEDURE, EntityValueType::VALUE},
      {EntityType::STMT, EntityValueType::STATEMENT_NUMBER},
      {EntityType::READ, EntityValueType::STATEMENT_NUMBER},
      {EntityType::PRINT, EntityValueType::STATEMENT_NUMBER},
      {EntityType::ASSIGN, EntityValueType::STATEMENT_NUMBER},
      {EntityType::CALL, EntityValueType::STATEMENT_NUMBER},
      {EntityType::WHILE, EntityValueType::STATEMENT_NUMBER},
      {EntityType::IF, EntityValueType::STATEMENT_NUMBER},
      {EntityType::VARIABLE, EntityValueType::VALUE},
      {EntityType::CONSTANT, EntityValueType::VALUE}};
};

void EntityDatabase::insert(EntityType type, string value) {
  entities[type].insert(value);
  entities_by_value_type[entityToValueType[type]].insert(value);
}

unordered_set<string> EntityDatabase::get(EntityType type) {
  unordered_set<string> results = entities[type];
  return results;
}

unordered_set<string> EntityDatabase::getByValueType(EntityValueType type) {
  unordered_set<string> results = entities_by_value_type[type];
  return results;
}

EntityValueType EntityDatabase::getEntityValueType(EntityType type) {
  return entityToValueType[type];
}