#pragma once

#include <unordered_set>
#include <string>

class CondExprNode {
 public:
  explicit CondExprNode(std::unordered_set<std::string> variables, std::unordered_set<int> constants);

        std::unordered_set<std::string> getVariables() const;
        std::unordered_set<int> getConstants() const;

  ~CondExprNode() = default;

 private:
        std::unordered_set<std::string> variables;
        std::unordered_set<int> constants;
};