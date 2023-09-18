#pragma once

#include "../interpreter/QueryInterpreter.h"

class QueryInterpreter;

class AExpression {
 public:
  virtual void acceptInterpreter(QueryInterpreter& interpreter) = 0;

  virtual ~AExpression() = default;
};
