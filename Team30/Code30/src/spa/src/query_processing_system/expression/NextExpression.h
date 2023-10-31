#pragma once

#include <string>

#include "../interpreter/QueryInterpreter.h"
#include "SuchThatExpression.h"

class NextExpression : public SuchThatExpression {
 public:
  NextExpression(std::string arg1, std::string arg2, bool is_not);

  void acceptInterpreter(QueryInterpreter& interpreter) override;

  ~NextExpression() = default;
};
