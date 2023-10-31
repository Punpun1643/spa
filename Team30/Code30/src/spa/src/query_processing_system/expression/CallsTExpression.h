#pragma once

#include <string>

#include "../interpreter/QueryInterpreter.h"
#include "SuchThatExpression.h"

class CallsTExpression : public SuchThatExpression {
 public:
  CallsTExpression(std::string arg1, std::string arg2, bool is_not);

  void acceptInterpreter(QueryInterpreter& interpreter) override;

  ~CallsTExpression() = default;
};
