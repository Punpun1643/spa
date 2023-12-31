#pragma once

#include <string>

#include "../interpreter/QueryInterpreter.h"
#include "SuchThatExpression.h"

class CallsExpression : public SuchThatExpression {
 public:
  CallsExpression(std::string arg1, std::string arg2, bool is_not);

  void AcceptInterpreter(QueryInterpreter& interpreter) override;

  ~CallsExpression() = default;
};
