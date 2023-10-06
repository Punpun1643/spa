#include "LinkedListTable.h"

LinkedListTable::LinkedListTable() : BaseTable() {}

void LinkedListTable::insert(std::string i, std::string j) {
  relations.addEdge(i, j);
};

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

bool LinkedListTable::isEmpty() { return relations.isEmpty(); }

std::unordered_set<std::string> LinkedListTable::getAllRelated(
    std::shared_ptr<std::unordered_set<std::string>> vals) {
  std::unordered_set<std::string> output;

  for (std::string val : *vals) {
    std::shared_ptr<Node> curr = relations.getNode(val);
    
    if (!curr->next.empty()) {
      output.insert(curr->value);
    }
  }

  return output;
};

std::unordered_set<std::string> LinkedListTable::getAllInverseRelated(
    std::shared_ptr<std::unordered_set<std::string>> vals) {
  std::unordered_set<std::string> output;
  for (std::string val : *vals) {
    std::shared_ptr<Node> curr = relations.getNode(val);

    if (curr->prev != nullptr) {
      output.insert(curr->value);
    }
  }
  return output;
};