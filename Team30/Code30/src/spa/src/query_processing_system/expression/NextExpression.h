#pragma once

#include "../interpreter/QueryInterpreter.h"
#include "SuchThatExpression.h"

class NextExpression : public SuchThatExpression {
 public:
  NextExpression(std::string arg1, std::string arg2);

  void acceptInterpreter(QueryInterpreter& interpreter) override;

  ~NextExpression() = default;
};
