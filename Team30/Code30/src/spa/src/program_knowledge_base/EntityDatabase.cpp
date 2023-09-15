#include "EntityDatabase.h"

#include <stdio.h>

#include <iostream>
#include <string>
#include <unordered_map>
#include <unordered_set>
#include <vector>

using namespace std;

EntityDatabase::EntityDatabase() {
  entities[EntityType::PROCEDURE] = make_shared<unordered_set<string>>();
  entities[EntityType::STMT] = make_shared<unordered_set<string>>();
  entities[EntityType::READ] = make_shared<unordered_set<string>>();
  entities[EntityType::PRINT] = make_shared<unordered_set<string>>();
  entities[EntityType::ASSIGN] = make_shared<unordered_set<string>>();
  entities[EntityType::CALL] = make_shared<unordered_set<string>>();
  entities[EntityType::WHILE] = make_shared<unordered_set<string>>();
  entities[EntityType::IF] = make_shared<unordered_set<string>>();
  entities[EntityType::VARIABLE] = make_shared<unordered_set<string>>();
  entities[EntityType::CONSTANT] = make_shared<unordered_set<string>>();

  statementTypes = {EntityType::STMT,   EntityType::READ, EntityType::PRINT,
                    EntityType::ASSIGN, EntityType::CALL, EntityType::WHILE,
                    EntityType::IF};
};

void EntityDatabase::insert(EntityType type, string value) {
  (entities[type])->insert(value);
  if (statementTypes.find(type) != statementTypes.end()) {
    (entities[EntityType::STMT])->insert(value);
  };
};

shared_ptr<unordered_set<string>> EntityDatabase::get(EntityType type) {
  shared_ptr<unordered_set<string>> results = entities[type];
  return results;
};