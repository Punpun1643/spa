#include "UFDSTable.h"

UFDSTable::UFDSTable() : BaseTable() {}

void UFDSTable::insert(string i, string j) {
  relations.unionSets(stoi(i), stoi(j));
};

bool UFDSTable::isRelated(string i, string j) {
  return i < j && relations.isSameSet(stoi(i), stoi(j));
}