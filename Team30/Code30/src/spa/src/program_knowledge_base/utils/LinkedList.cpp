#include "LinkedList.h"

LinkedList::LinkedList() = default;

std::shared_ptr<Node> LinkedList::GetNode(std::string const& val) {
  auto it = nodes.find(val);
  if (it == nodes.end()) {
    std::shared_ptr<Node> new_node = std::make_shared<Node>(Node(val));
    nodes[val] = new_node;
    return new_node;
  }
  return it->second;
}

void LinkedList::AddEdge(std::string const& val1, std::string const& val2) {
  std::shared_ptr<Node> node1 = GetNode(val1);
  std::shared_ptr<Node> node2 = GetNode(val2);
  node1->InsertNext(node2);
  node2->InsertPrev(node1);
}
