#pragma once

#include <stdio.h>

#include <iostream>
#include <string>
#include <utility>
#include <vector>

#include "../source_processor/node/ANode.h"
#include "program_knowledge_base/EntityDatabase.h"
#include "program_knowledge_base/RelDatabase.h"

class PKB {
  bool insertFollows(ANode stmt1, ANode stmt2);
  bool checkFollows(int lineNum1, int lineNum2);
  std::vector<std::string> getFollows(int lineNum, EntityType type);
  std::vector<std::string> getFollows(EntityType type, int lineNum);
  std::vector<std::string> getFollows(EntityType type1, EntityType type2);
};