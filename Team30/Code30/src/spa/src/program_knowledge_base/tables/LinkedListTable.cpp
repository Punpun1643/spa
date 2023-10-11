#include "LinkedListTable.h"

LinkedListTable::LinkedListTable() : BaseTable() {}

void LinkedListTable::insert(std::string i, std::string j) {
  relations.addEdge(i, j);
};

bool LinkedListTable::isEmpty() { return relations.isEmpty(); }

bool LinkedListTable::isRelated(std::string i, std::string j) {
  std::shared_ptr<Node> target = relations.getNode(i);
  std::shared_ptr<Node> curr = relations.getNode(j)->prev;
  while (curr != nullptr) {
    if (curr == target) {
      return true;
    }
    curr = curr->prev;
  }
  return false;
};

bool LinkedListTable::hasRelations(std::string val) {
  std::shared_ptr<Node> node = relations.getNode(val);
  return !node->next.empty();
};

bool LinkedListTable::hasInverseRelations(std::string val) {
  std::shared_ptr<Node> node = relations.getNode(val);
  return node->prev != nullptr;
}

std::unordered_set<std::string> LinkedListTable::getAllWithRelations(
    std::shared_ptr<std::unordered_set<std::string>> vals) {
  std::unordered_set<std::string> output;

  for (std::string val : *vals) {
    if (hasRelations(val)) {
      output.insert(val);
    }
  }
  return output;
};

std::unordered_set<std::string> LinkedListTable::getAllWithInverseRelations(
    std::shared_ptr<std::unordered_set<std::string>> vals) {
  std::unordered_set<std::string> output;

  for (std::string val : *vals) {
    if (hasInverseRelations(val)) {
      output.insert(val);
    }
  }
  return output;
};

std::unordered_set<std::string> LinkedListTable::getAllRelatedToValue(
    std::string val) {
  // TODO
  return std::unordered_set<std::string>();
}