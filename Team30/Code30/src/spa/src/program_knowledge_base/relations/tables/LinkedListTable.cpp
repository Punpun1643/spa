#include "LinkedListTable.h"

#include <memory>
#include <queue>
#include <string>

LinkedListTable::LinkedListTable() : RelationTable() {}

void LinkedListTable::Insert(std::string const& i, std::string const& j) {
  relations.AddEdge(i, j);
  empty = false;
}

bool LinkedListTable::IsEmpty() {
  return empty;
}

bool LinkedListTable::IsRelated(std::string const& i, std::string const& j) {
  std::shared_ptr<Node> target = relations.GetNode(i);
  std::shared_ptr<Node> curr = relations.GetNode(j);

  std::queue<std::shared_ptr<Node>> q;
  for (std::shared_ptr<Node> const& n : curr->prev) {
    q.push(n);
  }

  while (!q.empty()) {
    curr = q.front();
    if (curr == target) {
      return true;
    }
    q.pop();

    for (std::shared_ptr<Node> const& n : curr->prev) {
      q.push(n);
    }
  }
  return false;
}

bool LinkedListTable::HasRelations(std::string const& val) {
  std::shared_ptr<Node> node = relations.GetNode(val);
  return !node->IsNextEmpty();
}

bool LinkedListTable::HasInverseRelations(std::string const& val) {
  std::shared_ptr<Node> node = relations.GetNode(val);
  return !node->IsPrevEmpty();
}

std::unordered_set<std::string> LinkedListTable::GetAllWithRelations(
    std::unordered_set<std::string> const& vals) {
  std::unordered_set<std::string> output;

  for (std::string const& val : vals) {
    if (HasRelations(val)) {
      output.insert(val);
    }
  }
  return output;
}

std::unordered_set<std::string> LinkedListTable::GetAllWithInverseRelations(
    std::unordered_set<std::string> const& vals) {
  std::unordered_set<std::string> output;

  for (std::string const& val : vals) {
    if (HasInverseRelations(val)) {
      output.insert(val);
    }
  }
  return output;
}

std::unordered_set<std::string> LinkedListTable::GetAllRelatedToValue(
    std::string const& val) {
  std::shared_ptr<Node> curr = relations.GetNode(val);
  std::queue<std::shared_ptr<Node>> q;
  std::unordered_set<std::string> output;

  q.push(curr);

  while (!q.empty()) {
    curr = q.front();
    for (std::shared_ptr<Node> const& neighbour : curr->next) {
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
    std::string const& val) {
  std::shared_ptr<Node> curr = relations.GetNode(val);
  std::queue<std::shared_ptr<Node>> q;
  std::unordered_set<std::string> output;

  q.push(curr);

  while (!q.empty()) {
    curr = q.front();
    for (std::shared_ptr<Node> const& neighbour : curr->prev) {
      if (output.count(neighbour->value) == 0) {
        output.insert(neighbour->value);
        q.push(neighbour);
      }
    }

    q.pop();
  }

  return output;
}
