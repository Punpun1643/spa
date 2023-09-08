#include <iostream>
#include <stdio.h>
#include <string>
#include <vector>
#include <unordered_map>

#include "tables/StmtStmtRelTable.h"
#include "RelDatabase.h"

enum RelType
{
  Follows,
  Parent,
  Uses,
  Modifies
};

//TODO: Replace RelType with correct names based on implementation
class RelDatabase
{

  std::unordered_map<RelType, StmtStmtRelTable> relationships;
  
  public:  

  RelDatabase() {
    relationships = {Follows, new StmtStmtRelTable()}; //stmtNum, stmtNums[]
  };

  std::vector<int> get(RelType type, int lineNum)
  {
    return relationships[type].queryData(lineNum);
  }
  //we can overload int, var for Uses/Modifies
  bool insert(RelType type, int lineNum1, int lineNum2)
  {
    relationships[type].insertData(lineNum1, lineNum2);
    return true;
  }
};