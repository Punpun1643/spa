#pragma once

#include <memory>
#include <string>
#include <unordered_set>

class Node {
 public:
  std::unordered_set<std::shared_ptr<Node>> next;
  std::unordered_set<std::shared_ptr<Node>> prev;
  std::string value;
  explicit Node(std::string value);
  ~Node() = default;
};