#pragma once

#include <memory>
#include <string>
#include <unordered_set>

class Node {
 public:
  std::unordered_set<std::shared_ptr<Node>> next;
  std::unordered_set<std::shared_ptr<Node>> prev;
  std::string value;

  void InsertNext(const std::shared_ptr<Node>& node);
  void InsertPrev(const std::shared_ptr<Node>& node);
  bool IsNextEmpty() const;
  bool IsPrevEmpty() const;

  explicit Node(std::string  value);
  ~Node() = default;
};
