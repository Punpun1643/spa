#pragma once

#include <string>

#include "AExpression.h"

class ClauseExpression : public AExpression {
 public:
  ClauseExpression(std::string arg1, std::string arg2, bool is_not);

  std::string GetArg1() const;
  std::string GetArg2() const;
  bool IsNot() const;

  ~ClauseExpression() = default;

 private:
  std::string arg1;
  std::string arg2;
  bool is_not;
};
