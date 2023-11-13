#pragma once

#include <iostream>
#include <string>

#include "ClauseExpression.h"

class SuchThatExpression : public ClauseExpression {
 public:
  SuchThatExpression(std::string arg1, std::string arg2, bool is_not)
      : ClauseExpression(arg1, arg2, is_not) {}

  friend std::ostream& operator<<(std::ostream& os,
                                  SuchThatExpression const& obj) {
    // should this be here?
    os << "arg1: " << obj.GetArg1();
    os << ", arg2: " << obj.GetArg2();
    os << "\n";
    return os;
  }

  ~SuchThatExpression() = default;
};
