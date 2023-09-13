#include "StmtStmtRelTable.h"

#include <stdio.h>

#include <iostream>
#include <string>
#include <unordered_map>
#include <vector>

// StmtStmtRelTable stores relationships that have (stmt,stmt) parameters
// i.e follows, parent

StmtStmtRelTable::StmtStmtRelTable() { dataTable = {}; }

// inserts relation into table, key=stmtnum1, val=stmtnum2
bool StmtStmtRelTable::insertData(int key, int value) {
  std::unordered_map<int, std::vector<int>>::const_iterator got =
      dataTable.find(key);

  if (got == dataTable.end()) {  // add new key-value pair if not in map.
    std::vector<int> vect = {value};
    dataTable[key] = vect;
  } else {  // add to existing vector if alr in map
    dataTable[key].push_back(value);
  }

  return true;
}

// query a value from the table based on a key, useful for getRelation APIs.
std::vector<int> StmtStmtRelTable::queryData(int key) {
  std::unordered_map<int, std::vector<int>>::const_iterator got =
      dataTable.find(key);
  if (got == dataTable.end()) {  // data not found
    return {
        -1};  // TODO: Should we use optional instead of hardcoding a -1 value?
  } else {    // return value from hashmap
    return got->second;
  }
}
