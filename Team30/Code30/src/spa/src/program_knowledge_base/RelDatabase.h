#pragma once

#include <iostream>
#include <stdio.h>
#include <string>
#include <vector>
#include <unordered_map>

#include "tables/StmtStmtRelTable.h"

enum RelType { FOLLOWS, PARENT, USES, MODIFIES };

class RelDatabase
{
  std::unordered_map<RelType, StmtStmtRelTable*> relationships;

 public:
  RelDatabase();
  std::vector<int> get(RelType type, int lineNum);
  bool insert(RelType type, int lineNum1, int lineNum2);
};