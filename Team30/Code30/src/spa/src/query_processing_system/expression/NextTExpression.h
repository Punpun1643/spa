#pragma once

#include "../interpreter/QueryInterpreter.h"
#include "SuchThatExpression.h"

class NextTExpression : public SuchThatExpression {
 public:
  NextTExpression(std::string arg1, std::string arg2);

  void acceptInterpreter(QueryInterpreter& interpreter) override;

  ~NextTExpression() = default;
};
