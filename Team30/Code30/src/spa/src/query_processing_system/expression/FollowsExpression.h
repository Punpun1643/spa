#pragma once

#include "../interpreter/QueryInterpreter.h"
#include "ClauseExpression.h"

class FollowsExpression : public ClauseExpression {
 public:
  FollowsExpression(std::string arg1, std::string arg2);

  void acceptInterpreter(QueryInterpreter& interpreter) override;

  ~FollowsExpression() = default;
};
