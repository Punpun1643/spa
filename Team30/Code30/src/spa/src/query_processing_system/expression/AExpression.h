#pragma once

#include "../interpreter/QueryInterpreter.h"

class QueryInterpreter;

class AExpression {
 public:
  virtual void acceptInterpreter(QueryInterpreter& interpreter) = 0;

  void SetNextExpression(std::optional<std::shared_ptr<AExpression>> next) {
    this->next_expression = next;
  }
  std::optional<std::shared_ptr<AExpression>> GetNextExpression() {
    return this->next_expression;
  }

  virtual ~AExpression() = default;

 private:
  std::optional<std::shared_ptr<AExpression>> next_expression;
};
