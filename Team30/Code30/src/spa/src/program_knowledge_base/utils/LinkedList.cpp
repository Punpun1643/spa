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
  node1->next.insert(node2);
  node2->prev.insert(node1);
}



bool LinkedList::isEmpty() { return nodes.empty(); }