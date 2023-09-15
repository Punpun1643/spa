#pragma once

#include <unordered_map>

#include "../utils/UFDS.h"
#include "BaseTable.h"

using namespace std;

class UFDSTable : public BaseTable {
  UFDS relations = UFDS(100);

 public:
  UFDSTable();
  ~UFDSTable() = default;
  void insert(string i, string j) ;
  bool isRelated(string i, string j);
  //unique_ptr<vector<string>> isRelatedAll(unordered_set<string> ls);
};