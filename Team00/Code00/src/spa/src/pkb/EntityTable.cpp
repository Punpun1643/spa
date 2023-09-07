#include <iostream>
#include <stdio.h>
#include <string>
#include <vector>
#include <unordered_map>
#include <any>

using namespace std;

#include "EntityTable.h"

//Entity Table class extended from base table class, will be used for entities
//i.e proc, stmt, var, const
class EntityTable {
  std::unordered_map<std::string, vector<int>> dataTable; //data stored as a hashmap
  
  //insert a value into the table with key
  bool insertData(std::string key, int value) {
    std::unordered_map<std::string, vector<int>>::const_iterator got = dataTable.find(key);

    if (got == dataTable.end()) { //add new key-value pair
      dataTable[key] = vector<int> vect{value};
    }
    else { //add to existing vector
      dataTable[key].insert(value);
    }
  
    return true;
  }

  //query a value from the table based on a key
  vector<int> queryData(std::string key) {
    std::unordered_map<std::string, vector<int>>::const_iterator got = dataTable.find(key);
    if (got == dataTable.end()) { //data not found
      return -1; //TODO: Should we use optional instead of hardcoding a -1 value?
    }
    else { //return value from hashmap
      return got->second;
    }
  }
}