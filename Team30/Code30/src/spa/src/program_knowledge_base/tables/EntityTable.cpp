#include <iostream>
#include <stdio.h>
#include <string>
#include <vector>
#include <unordered_map>

#include "EntityTable.h"

//Entity Table class extended from base table class, will be used for NAMED entities
//i.e proc, stmt, var, const

class EntityTable {
  std::unordered_map<std::string, std::vector<int>> dataTable; 
  //int = StmtNum

  public:

  EntityTable() {
    dataTable = {};
  }
  
  //Based on Entity Node, inserts the relevant name/value/type into the table as key
  //Linenum position as value.

  bool insertData(std::string key, int value) {
    std::unordered_map<std::string, std::vector<int>>::const_iterator got = dataTable.find(key);

    if (got == dataTable.end()) { //add new key-value pair if not in map.
      std::vector<int> vect = {value};
      dataTable[key] = vect;
    }
    else { //add to existing vector if alr in map
      dataTable[key].push_back(value);
    }
  
    return true;
  }

  //Returns Stmtnums of entity types from the table based on their name
  std::vector<int> queryData(std::string key) {
    std::unordered_map<std::string, std::vector<int>>::const_iterator got = dataTable.find(key);
    
    if (got == dataTable.end()) { //data not found
      return -1; //TODO: Should we use optional instead of hardcoding a -1 value?
    }
    else { //return value from hashmap
      return got->second;
    }
  }

  //Returns all names/values of entities
  std::vector<std::string> queryAllKeys() {
    std::vector<std::string> keys;
    keys.reserve(dataTable.size());

    for (auto kv : dataTable) {
      keys.push_back(kv.first);
    } 

    return keys;
  }
}