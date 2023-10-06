#include "UFDSTable.h"

UFDSTable::UFDSTable() : BaseTable() {}

void UFDSTable::insert(std::string i, std::string j) {
  empty = false;
  relations.unionSets(stoi(i), stoi(j));
};

bool UFDSTable::isRelated(std::string i, std::string j) {
  return stoi(i) < stoi(j) && relations.isSameSet(stoi(i), stoi(j));
}

bool UFDSTable::isEmpty() { return empty; }

std::unordered_set<std::string> UFDSTable::getAllRelated(
    std::shared_ptr<std::unordered_set<std::string>> vals) {
  return {};
};
std::unordered_set<std::string> UFDSTable::getAllInverseRelated(
    std::shared_ptr<std::unordered_set<std::string>> vals) {
  return {};
};
