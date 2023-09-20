#pragma once

#include "AExpression.h"

class SuchThatExpression : public AExpression {
  public:
    SuchThatExpression(std::string arg1, std::string arg2);
    std::string GetArg1();
    std::string GetArg2();
    virtual void acceptInterpreter(QueryInterpreter& interperter) = 0;

    virtual ~SuchThatExpression() = default;

  private:
    std::string arg1;
    std::string arg2;
};
