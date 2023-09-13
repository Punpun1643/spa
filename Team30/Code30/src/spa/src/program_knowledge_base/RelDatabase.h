#pragma once

#include <memory>
#include <unordered_map>

#include "RelationType.h"
#include "tables/BaseTable.h"
#include "tables/DictionaryTable.h"
#include "tables/UFDSTable.h"

class RelDatabase {
  std::unordered_map<RelationType, shared_ptr<BaseTable>> relationships;

 public:
  RelDatabase();
  ~RelDatabase() = default;
  shared_ptr<BaseTable> getTable(RelationType type);
};