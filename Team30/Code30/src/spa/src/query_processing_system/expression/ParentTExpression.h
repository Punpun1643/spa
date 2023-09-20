#pragma once

#include "../interpreter/QueryInterpreter.h"
#include "SuchThatExpression.h"

class ParentTExpression : public SuchThatExpression {
 public:
  ParentTExpression(std::string arg1, std::string arg2);

  void acceptInterpreter(QueryInterpreter& interpreter) override;

  ~ParentTExpression() = default;

 private:
  std::string arg1;
  std::string arg2;
};
