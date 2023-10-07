#pragma once

#include <string>

#include "AExpression.h"

class ClauseExpression : AExpression {
 public:
  ClauseExpression(std::string arg1, std::string arg2);

  /* void virtual acceptInterpreter(QueryInterpreter& interpreter) */

  std::string GetArg1();
  std::string GetArg2();

  ~ClauseExpression() = default;

 private:
  std::string arg1;
  std::string arg2;
};
