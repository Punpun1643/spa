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
  unordered_set<string> statement_numbers;

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

  unordered_set<EntityType> entityTypesThatStoreStatementNumbers = {
      EntityType::STMT,   EntityType::READ, EntityType::PRINT,
      EntityType::ASSIGN, EntityType::CALL, EntityType::WHILE,
      EntityType::IF};
};

void EntityDatabase::insert(EntityType type, string value) {
  entities[type].insert(value);
  if (entityTypesThatStoreStatementNumbers.find(type) !=
      entityTypesThatStoreStatementNumbers.end()) {
    statement_numbers.insert(value);
  };
};

unordered_set<string> EntityDatabase::get(EntityType type) {
  unordered_set<string> results = entities[type];
  return results;
};

unordered_set<string> EntityDatabase::getAllStatements() {
  return statement_numbers;
};
