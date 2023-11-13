#include "Node.h"

#include <utility>

Node::Node(std::string  value) : value(std::move(value)) {}

void Node::InsertNext(const std::shared_ptr<Node>& node) {
  next.insert(node);
}

void Node::InsertPrev(const std::shared_ptr<Node>& node) {
  prev.insert(node);
}

bool Node::IsNextEmpty() const {
  return next.empty();
}

bool Node::IsPrevEmpty() const {
  return prev.empty();
}
