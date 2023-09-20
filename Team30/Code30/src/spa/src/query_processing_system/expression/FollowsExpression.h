#pragma once

#include "SuchThatExpression.h"
#include "../interpreter/QueryInterpreter.h"

class FollowsExpression : public SuchThatExpression {
  public:
    FollowsExpression(std::string arg1, std::string arg2);

    void acceptInterpreter(QueryInterpreter& interpreter) override;

    ~FollowsExpression() = default;
  private:
    std::string arg1;
    std::string arg2;
};
