#pragma once

#include <stdio.h>

#include <iostream>
#include <string>
#include <unordered_map>
#include <vector>

#include "../source_processor/node/stmt_node/StmtNode.h"

class EntityDatabase {
  bool insert(StmtNode ent);
  std::vector<std::string> getAll(EntityType type);
};