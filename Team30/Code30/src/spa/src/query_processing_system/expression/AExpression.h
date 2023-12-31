#pragma once

#include <memory>
#include <optional>
#include <utility>

class QueryInterpreter;

class AExpression : public std::enable_shared_from_this<AExpression> {
 public:
  virtual void AcceptInterpreter(QueryInterpreter& interpreter) = 0;

  void SetNextExpression(std::optional<std::shared_ptr<AExpression>> next) {
    if (next_expression.has_value()) {
      this->next_expression.value()->SetNextExpression(std::move(next));
    } else {
      this->next_expression = std::move(next);
    }
  }

  std::optional<std::shared_ptr<AExpression>> GetNextExpression() {
    return this->next_expression;
  }

  virtual ~AExpression() = default;

 private:
  std::optional<std::shared_ptr<AExpression>> next_expression;
};
