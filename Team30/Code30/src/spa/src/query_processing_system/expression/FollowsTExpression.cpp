#include "FollowsTExpression.h"

FollowsTExpression::FollowsTExpression(std::string arg1, std::string arg2)
    : SuchThatExpression(arg1, arg2){};

void FollowsTExpression::acceptInterpreter(QueryInterpreter& interpreter) {
  interpreter.Interpret(*this);
}
