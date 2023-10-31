#pragma once

#include <string>

#include "../interpreter/QueryInterpreter.h"
#include "SuchThatExpression.h"

class ParentExpression : public SuchThatExpression {
 public:
  ParentExpression(std::string arg1, std::string arg2, bool is_not);

  void acceptInterpreter(QueryInterpreter& interpreter) override;

  ~ParentExpression() = default;
};
