#include "EntityTable.h"

#include <stdio.h>

#include <iostream>
#include <string>
#include <unordered_map>
#include <vector>

// Entity Table class extended from base table class, will be used for NAMED
// entities i.e proc, stmt, var, const

// key = value or stmtType; int = StmtNum

EntityTable::EntityTable() { dataTable = {}; }

// Based on Entity Node, inserts the relevant name/value/type into the table as
// key Linenum position as value.

bool EntityTable::insertData(std::string key, int value) {
  std::unordered_map<std::string, std::vector<int>>::const_iterator got =
      dataTable.find(key);

  if (got == dataTable.end()) {  // add new key-value pair if not in map.
    std::vector<int> vect = {value};
    dataTable[key] = vect;
  } else {  // add to existing vector if alr in map
    dataTable[key].push_back(value);
  }

  return true;
}

// Returns Stmtnums of entity types from the table based on their name
std::vector<int> EntityTable::queryData(std::string key) {
  std::unordered_map<std::string, std::vector<int>>::const_iterator got =
      dataTable.find(key);

  if (got == dataTable.end()) {  // data not found
    return {
        -1};  // TODO: Should we use optional instead of hardcoding a -1 value?
  } else {    // return value from hashmap
    return got->second;
  }
}

// Returns all names/values of entities, for SP-PKB API.
std::vector<std::string> EntityTable::queryAllKeys() {
  std::vector<std::string> keys;
  keys.reserve(dataTable.size());

  for (auto kv : dataTable) {
    keys.push_back(kv.first);
  }

  return keys;
}