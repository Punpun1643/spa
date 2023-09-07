#include <iostream>
#include <stdio.h>
#include <string>
#include <unordered_map>
#include <vector>

#include "EntityTable.h"

//StmtStmtRelTable stores relationships that have (stmt,stmt) parameters
//i.e follows, parent

class StmtStmtRelTable
{
  std::unordered_map<int, std::vector<int>> dataTable; 
  //TODO: Replace int with StmtNode?

  //insert a value into the table with key
  bool insertData(int key, int value)
  {
    std::unordered_map<int, std::vector<int>>::const_iterator got =
        dataTable.find(key);

    if (got == dataTable.end()) { //add new key-value pair if not in map.
      std::vector<int> vect = {value};
      dataTable[key] = vect;
    }
    else { //add to existing vector if alr in map
      dataTable[key].push_back(value);
    }

    return true;
  }

  //query a value from the table based on a key
  std::vector<int> queryData(int key)
  {
    std::unordered_map<int, std::vector<int>>::const_iterator got =
        dataTable.find(key);
    if (got == dataTable.end()) { //data not found
      return -1; //TODO: Should we use optional instead of hardcoding a -1 value?
    }
    else { //return value from hashmap
      return got->second;
    }
  }
}