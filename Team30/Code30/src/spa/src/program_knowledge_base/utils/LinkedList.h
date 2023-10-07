#pragma once

#include <memory>
#include <string>
#include <unordered_map>

#include "Node.h"

class LinkedList {
 private:
  std::unordered_map<std::string, std::shared_ptr<Node>> nodes;
  
 public:
  explicit LinkedList();
  ~LinkedList() = default;

  std::shared_ptr<Node> getNode(std::string val);
  void addEdge(std::string val1, std::string val2);
  bool isEmpty();
};