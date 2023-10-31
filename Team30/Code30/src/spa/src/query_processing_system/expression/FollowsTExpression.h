#pragma once

#include <string>

#include "../interpreter/QueryInterpreter.h"
#include "SuchThatExpression.h"

class FollowsTExpression : public SuchThatExpression {
 public:
  FollowsTExpression(std::string arg1, std::string arg2, bool is_not);

  void AcceptInterpreter(QueryInterpreter& interpreter) override;

  ~FollowsTExpression() = default;
};
