#include "FollowsTExpression.h"

#include <string>

FollowsTExpression::FollowsTExpression(std::string arg1, std::string arg2, bool is_not)
    : SuchThatExpression(arg1, arg2, is_not) {}

void FollowsTExpression::acceptInterpreter(QueryInterpreter& interpreter) {
  interpreter.Interpret(
      std::dynamic_pointer_cast<FollowsTExpression>(shared_from_this()));
}
