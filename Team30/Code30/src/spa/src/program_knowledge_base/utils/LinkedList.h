#pragma once

#include <memory>
#include <string>
#include <unordered_map>

#include "Node.h"

class LinkedList {
 private:
  std::unordered_map<std::string, std::shared_ptr<Node>> nodes;

 public:
  LinkedList();
  ~LinkedList() = default;

  std::shared_ptr<Node> GetNode(std::string val);
  void AddEdge(std::string val1, std::string val2);
  bool IsEmpty();
};
