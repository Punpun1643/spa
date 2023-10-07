#pragma once

#include "ClauseExpression.h"

class SuchThatExpression : public ClauseExpression {
 public:
  SuchThatExpression(std::string arg1, std::string arg2)
      : ClauseExpression(arg1, arg2){};
};
