#pragma once

#include "../interpreter/QueryInterpreter.h"
#include "SuchThatExpression.h"

class CallsTExpression : public SuchThatExpression {
 public:
  CallsTExpression(std::string arg1, std::string arg2);

  void acceptInterpreter(QueryInterpreter& interpreter) override;

  ~CallsTExpression() = default;
};
