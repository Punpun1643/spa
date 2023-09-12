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
};

bool EntityDatabase::insert(EntityType type, string value) {
  entities[type].insert(value);
  return true;
}

unordered_set<string> EntityDatabase::get(EntityType type) {
  unordered_set<string> results = entities[type];
  return results;
}
