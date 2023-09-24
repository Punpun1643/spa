#pragma once

#include <memory>
#include <string>

class Node {
 private:
  std::string value;

 public:
  std::shared_ptr<Node> next;
  explicit Node(std::string value);
  ~Node() = default;
};