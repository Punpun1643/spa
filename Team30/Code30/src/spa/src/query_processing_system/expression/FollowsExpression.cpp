#include "FollowsExpression.h"

#include <string>

FollowsExpression::FollowsExpression(std::string arg1, std::string arg2,
                                     bool is_not)
    : SuchThatExpression(arg1, arg2, is_not) {}

void FollowsExpression::acceptInterpreter(QueryInterpreter& interpreter) {
  interpreter.Interpret(
      std::dynamic_pointer_cast<FollowsExpression>(shared_from_this()));
}
