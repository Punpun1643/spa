#include "RelDatabase.h"

#include <stdio.h>

#include <iostream>
#include <string>
#include <unordered_map>
#include <vector>

#include "tables/StmtStmtRelTable.h"

// TODO: Replace RelType with correct names based on implementation

RelDatabase::RelDatabase() {
  relationships = {{FOLLOWS, new StmtStmtRelTable()}};  // stmtNum, stmtNums[]
};

std::vector<int> RelDatabase::get(RelationType type, int lineNum) {
  relationships[type]->queryData(lineNum);
  return {1};
}
// we can overload int, var for Uses/Modifies
bool RelDatabase::insert(RelationType type, int lineNum1, int lineNum2) {
  relationships[type]->insertData(lineNum1, lineNum2);
  return true;
}
