#pragma once

#include <memory>
#include <string>
#include <unordered_set>

class Node {
 public:
  std::unordered_set<std::shared_ptr<Node>> next;
  std::unordered_set<std::shared_ptr<Node>> prev;
  std::string value;

  void InsertNext(std::shared_ptr<Node> node);
  void InsertPrev(std::shared_ptr<Node> node);
  bool IsNextEmpty();
  bool IsPrevEmpty();

  explicit Node(std::string const& value);
  ~Node() = default;
};
