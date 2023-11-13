#include "Node.h"

#include <utility>

Node::Node(std::string value) : value(std::move(value)) {}

void Node::InsertNext(std::shared_ptr<Node> const& node) {
  next.insert(node);
}

void Node::InsertPrev(std::shared_ptr<Node> const& node) {
  prev.insert(node);
}

bool Node::IsNextEmpty() const {
  return next.empty();
}

bool Node::IsPrevEmpty() const {
  return prev.empty();
}
