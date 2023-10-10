#pragma once

#include <string>

#include "AExpression.h"

class ClauseExpression : public AExpression {
 public:
  ClauseExpression(std::string arg1, std::string arg2);

  std::string GetArg1() const;
  std::string GetArg2() const;

  ~ClauseExpression() = default;

 private:
  std::string arg1;
  std::string arg2;
};
