#include "RelDatabase.h"

#include <stdio.h>

#include <iostream>
#include <string>
#include <unordered_map>
#include <vector>

#include "tables/StmtStmtRelTable.h"

RelDatabase::RelDatabase() {
  relationships = {{FOLLOWS, new StmtStmtRelTable()}};  // stmtNum, stmtNums[]
};

std::vector<int> RelDatabase::get(RelType type, int lineNum) {
  relationships[type]->queryData(lineNum);
  return {1};
}
// we can overload int, var for Uses/Modifies
bool RelDatabase::insert(RelType type, int lineNum1, int lineNum2) {
  relationships[type]->insertData(lineNum1, lineNum2);
  return true;
}
