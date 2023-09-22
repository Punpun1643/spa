#pragma once

#include <memory>
#include <string>
#include <unordered_set>
class CondExprNode {
 public:
  explicit CondExprNode(std::unordered_set<std::string> vars,
                        std::unordered_set<int> consts);

  std::shared_ptr<std::unordered_set<std::string>> getVariables() const;
  std::shared_ptr<std::unordered_set<int>> getConstants() const;

  ~CondExprNode() = default;

 private:
  std::shared_ptr < std::unordered_set<std::string>> variables;
  std::shared_ptr < std::unordered_set<int>> constants;
};