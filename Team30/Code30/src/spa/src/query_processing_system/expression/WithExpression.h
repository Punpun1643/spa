#pragma once

#include <string>

#include "ClauseExpression.h"

class WithExpression : public ClauseExpression {
 public:
  WithExpression(std::string first_ref, std::string second_ref, bool is_not);

  void acceptInterpreter(QueryInterpreter& interpreter) override;

  ~WithExpression() = default;
};
