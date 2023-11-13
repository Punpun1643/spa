#include "Node.h"

Node::Node(std::string const& value) : value(value) {}

void Node::InsertNext(std::shared_ptr<Node> node) {
  next.insert(node);
}

void Node::InsertPrev(std::shared_ptr<Node> node) {
  prev.insert(node);
}

bool Node::IsNextEmpty() {
  return next.empty();
}

bool Node::IsPrevEmpty() {
  return prev.empty();
}
