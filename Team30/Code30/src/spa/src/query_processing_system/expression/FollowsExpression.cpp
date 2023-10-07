#include "FollowsExpression.h"

FollowsExpression::FollowsExpression(std::string arg1, std::string arg2)
    : ClauseExpression(arg1, arg2){};

void FollowsExpression::acceptInterpreter(QueryInterpreter& interpreter) {
  interpreter.Interpret(*this);
}
