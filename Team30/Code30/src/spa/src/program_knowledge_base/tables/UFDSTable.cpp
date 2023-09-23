#include "UFDSTable.h"

UFDSTable::UFDSTable() : BaseTable() {}

void UFDSTable::insert(std::string i, std::string j) {
  empty = false;
  relations.unionSets(stoi(i), stoi(j));
};

bool UFDSTable::isRelated(std::string i, std::string j) {
  return i < j && relations.isSameSet(stoi(i), stoi(j));
}

bool UFDSTable::isEmpty() { return empty; }