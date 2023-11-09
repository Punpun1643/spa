#include "LinkedListTable.h"

#include <iostream>
#include <memory>
#include <queue>
#include <string>

LinkedListTable::LinkedListTable() : RelationTable() {}

void LinkedListTable::Insert(std::string i, std::string j) {
  relations.AddEdge(i, j);
  empty = false;
}

bool LinkedListTable::IsEmpty() {
  return empty;
}

bool LinkedListTable::IsRelated(std::string i, std::string j) {
  std::shared_ptr<Node> target = relations.GetNode(i);
  std::shared_ptr<Node> curr = relations.GetNode(j);

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

bool LinkedListTable::HasRelations(std::string val) {
  std::shared_ptr<Node> node = relations.GetNode(val);
  return !node->next.empty();
}

bool LinkedListTable::HasInverseRelations(std::string val) {
  std::shared_ptr<Node> node = relations.GetNode(val);
  return !node->prev.empty();
}

std::unordered_set<std::string> LinkedListTable::GetAllWithRelations(
    std::unordered_set<std::string> vals) {
  std::unordered_set<std::string> output;

  for (std::string val : vals) {
    if (HasRelations(val)) {
      output.insert(val);
    }
  }
  return output;
}

std::unordered_set<std::string> LinkedListTable::GetAllWithInverseRelations(
    std::unordered_set<std::string> vals) {
  std::unordered_set<std::string> output;

  for (std::string val : vals) {
    if (HasInverseRelations(val)) {
      output.insert(val);
    }
  }
  return output;
}

std::unordered_set<std::string> LinkedListTable::GetAllRelatedToValue(
    std::string val) {
  std::shared_ptr<Node> curr = relations.GetNode(val);
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

std::unordered_set<std::string> LinkedListTable::GetAllInverseRelatedToValue(
    std::string val) {
  std::shared_ptr<Node> curr = relations.GetNode(val);
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
