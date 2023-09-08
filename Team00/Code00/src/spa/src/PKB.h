#pragma once

#include <stdio.h>
#include <iostream>
#include <string>
#include <vector>
#include <utility> 

#include  "stmtNode.h" //change based on file name
#include  "pkb/EntityDatabase.h"
#include  "pkb/RelDatabase.h"

class PKB {
  bool insertFollows(stmtNode stmt1, stmtNode stmt2);
  bool checkFollows(int lineNum1, int lineNum2);
  std::vector<std::string> getFollows(int lineNum, EntityType type);
  std::vector<std::string> getFollows(EntityType type, int lineNum);
  std::vector<std::string> getFollows(EntityType type1, EntityType type2);
};