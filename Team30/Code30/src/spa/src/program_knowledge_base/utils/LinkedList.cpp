#include "LinkedList.h"

LinkedList::LinkedList(){};

std::shared_ptr<Node> LinkedList::getNode(std::string val) {
  auto it = nodes.find(val);
  if (it == nodes.end()) {
    std::shared_ptr<Node> new_node = std::make_shared<Node>(Node(val));
    nodes[val] = new_node;
    return new_node;
  };
  return it->second;
}

void LinkedList::addEdge(std::string val1, std::string val2) {
  std::shared_ptr<Node> node1 = getNode(val1);
  std::shared_ptr<Node> node2 = getNode(val2);
  node2->next = node1;
}

bool LinkedList::hasPath(std::string val1, std::string val2) {
  std::shared_ptr<Node> curr = getNode(val2);
  std::shared_ptr<Node> target = getNode(val1);
  curr = curr->next;
  while (curr != nullptr) {
    if (curr == target) {
      return true;
    }
    curr = curr->next;
  }
  return false;
}

bool LinkedList::isEmpty() { return nodes.empty(); }