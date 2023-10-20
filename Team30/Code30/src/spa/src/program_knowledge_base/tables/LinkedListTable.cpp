#include "LinkedListTable.h"

#include <iostream>
#include <queue>
#include <string>
#include <memory>

LinkedListTable::LinkedListTable() : BaseTable() {}

void LinkedListTable::insert(std::string i, std::string j) {
  relations.addEdge(i, j);
}

bool LinkedListTable::isEmpty() {
  return relations.isEmpty();
}

bool LinkedListTable::isRelated(std::string i, std::string j) {
  std::shared_ptr<Node> target = relations.getNode(i);
  std::shared_ptr<Node> curr = relations.getNode(j);

  std::queue<std::shared_ptr<Node>> q;
  for (std::shared_ptr<Node> n : curr->prev) {
    q.push(n);
  }

  while (!q.empty()) {
    curr = q.front();
    if (curr == target) {
      return true;
    }
    q.pop();

    for (std::shared_ptr<Node> n : curr->prev) {
      q.push(n);
    }
  }
  return false;
}

bool LinkedListTable::hasRelations(std::string val) {
  std::shared_ptr<Node> node = relations.getNode(val);
  return !node->next.empty();
}

bool LinkedListTable::hasInverseRelations(std::string val) {
  std::shared_ptr<Node> node = relations.getNode(val);
  return !node->prev.empty();
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
}

std::unordered_set<std::string> LinkedListTable::getAllWithInverseRelations(
    std::shared_ptr<std::unordered_set<std::string>> vals) {
  std::unordered_set<std::string> output;

  for (std::string val : *vals) {
    if (hasInverseRelations(val)) {
      output.insert(val);
    }
  }
  return output;
}

std::unordered_set<std::string> LinkedListTable::getAllRelatedToValue(
    std::string val) {
  std::shared_ptr<Node> curr = relations.getNode(val);
  std::queue<std::shared_ptr<Node>> q;
  std::unordered_set<std::string> output;

  q.push(curr);

  while (!q.empty()) {
    curr = q.front();
    for (std::shared_ptr<Node> neighbour : curr->next) {
      if (output.count(neighbour->value) == 0) {
        output.insert(neighbour->value);
        q.push(neighbour);
      }
    }

    q.pop();
  }

  return output;
}

std::unordered_set<std::string> LinkedListTable::getAllInverseRelatedToValue(
    std::string val) {
  std::shared_ptr<Node> curr = relations.getNode(val);
  std::queue<std::shared_ptr<Node>> q;
  std::unordered_set<std::string> output;

  q.push(curr);

  while (!q.empty()) {
    curr = q.front();
    for (std::shared_ptr<Node> neighbour : curr->prev) {
      if (output.count(neighbour->value) == 0) {
        output.insert(neighbour->value);
        q.push(neighbour);
      }
    }

    q.pop();
  }

  return output;
}
