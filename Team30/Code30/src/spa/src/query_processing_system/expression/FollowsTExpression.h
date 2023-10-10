#pragma once

#include "../interpreter/QueryInterpreter.h"
#include "SuchThatExpression.h"

class FollowsTExpression : public SuchThatExpression {
 public:
  FollowsTExpression(std::string arg1, std::string arg2);

  void acceptInterpreter(QueryInterpreter& interpreter) override;

  ~FollowsTExpression() = default;
};
