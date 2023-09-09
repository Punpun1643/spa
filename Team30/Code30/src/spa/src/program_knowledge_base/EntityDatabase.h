#pragma once

#include <iostream>
#include <stdio.h>
#include <string>
#include <vector>
#include <unordered_map>

class EntityDatabase {
  bool insert(EntityNode ent);
  std::vector<std::string> getAll(EntityType type);
};