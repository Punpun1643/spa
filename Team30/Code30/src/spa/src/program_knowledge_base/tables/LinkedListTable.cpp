#include "LinkedListTable.h"

LinkedListTable::LinkedListTable() : BaseTable() {}

void LinkedListTable::insert(std::string i, std::string j) {
  relations.addEdge(i, j);
};

bool LinkedListTable::isRelated(std::string i, std::string j) {
  return relations.hasPath(i, j);
}

bool LinkedListTable::isEmpty() { return relations.isEmpty(); }