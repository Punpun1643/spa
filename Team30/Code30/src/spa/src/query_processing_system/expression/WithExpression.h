#pragma once

#include "ClauseExpression.h"

class WithExpression : public ClauseExpression {
 public:
  WithExpression(std::string first_ref, std::string second_ref);

  void acceptInterpreter(QueryInterpreter& interpreter) override;

  ~WithExpression() = default;
};
