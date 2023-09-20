#pragma once

#include "../interpreter/QueryInterpreter.h"
#include "SuchThatExpression.h"

class ParentExpression : public SuchThatExpression {
 public:
  ParentExpression(std::string arg1, std::string arg2);

  void acceptInterpreter(QueryInterpreter& interpreter) override;

  ~ParentExpression() = default;

 private:
  std::string arg1;
  std::string arg2;
};
