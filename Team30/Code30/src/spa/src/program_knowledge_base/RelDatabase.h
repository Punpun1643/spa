#pragma once

#include <unordered_map>

#include "tables/BaseTable.h"
#include "tables/FollowsTable.h"
#include "tables/ParentsTable.h"

enum class RelType { FOLLOWS, PARENT, USES, MODIFIES };

class RelDatabase {
  std::unordered_map<RelType, BaseTable> relationships;

 public:
  RelDatabase();
  ~RelDatabase() = default;
};