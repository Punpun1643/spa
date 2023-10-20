#include <string>

#include "FollowsExpression.h"

FollowsExpression::FollowsExpression(std::string arg1, std::string arg2)
    : SuchThatExpression(arg1, arg2) {}

void FollowsExpression::acceptInterpreter(QueryInterpreter& interpreter) {
  interpreter.Interpret(
      std::dynamic_pointer_cast<FollowsExpression>(shared_from_this()));
}
