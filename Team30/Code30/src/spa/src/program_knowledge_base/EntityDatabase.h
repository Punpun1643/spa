#pragma once

#include <stdio.h>

#include <iostream>
#include <string>
#include <unordered_map>
#include <vector>
#include <memory>

#include "../source_processor/node/stmt_node/StmtNode.h"
#include "../query_processing_system/common/EntityType.h"
#include "tables/EntityTable.h"


class EntityDatabase {
  std::unordered_map<EntityType, EntityTable*> entities;

 public:
  EntityDatabase();
  bool insert(std::shared_ptr<StmtNode> ent);
  std::vector<std::string> getAll(EntityType type);
};