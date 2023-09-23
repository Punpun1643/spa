#pragma once

#include "../interpreter/QueryInterpreter.h"
#include "SuchThatExpression.h"

class UsesExpression : public SuchThatExpression {
 public:
  UsesExpression(std::string arg1, std::string arg2);

  void acceptInterpreter(QueryInterpreter& interpreter) override;

  ~UsesExpression() = default;

 private:
  std::string arg1;
  std::string arg2;
};
