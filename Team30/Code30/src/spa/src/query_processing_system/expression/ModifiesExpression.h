#pragma once

#include "../interpreter/QueryInterpreter.h"
#include "SuchThatExpression.h"

class ModifiesExpression : public SuchThatExpression {
 public:
  ModifiesExpression(std::string arg1, std::string arg2);

  void acceptInterpreter(QueryInterpreter& interpreter) override;

  ~ModifiesExpression() = default;

 private:
  std::string arg1;
  std::string arg2;
};
