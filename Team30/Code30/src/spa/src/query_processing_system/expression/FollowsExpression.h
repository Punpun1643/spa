#pragma once

#include "../interpreter/QueryInterpreter.h"
#include "SuchThatExpression.h"

class FollowsExpression : public SuchThatExpression {
 public:
  FollowsExpression(std::string arg1, std::string arg2);

  void acceptInterpreter(QueryInterpreter& interpreter) override;

  ~FollowsExpression() = default;
};
