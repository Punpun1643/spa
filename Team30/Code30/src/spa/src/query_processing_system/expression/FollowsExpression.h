#pragma once

#include <string>

#include "../interpreter/QueryInterpreter.h"
#include "SuchThatExpression.h"

class FollowsExpression : public SuchThatExpression {
 public:
  FollowsExpression(std::string arg1, std::string arg2, bool is_not);

  void acceptInterpreter(QueryInterpreter& interpreter) override;

  ~FollowsExpression() = default;
};
