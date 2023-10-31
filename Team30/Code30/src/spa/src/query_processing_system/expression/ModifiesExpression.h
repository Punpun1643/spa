#pragma once

#include <string>

#include "../interpreter/QueryInterpreter.h"
#include "SuchThatExpression.h"

class ModifiesExpression : public SuchThatExpression {
 public:
  ModifiesExpression(std::string arg1, std::string arg2, bool is_not);

  void AcceptInterpreter(QueryInterpreter& interpreter) override;

  ~ModifiesExpression() = default;
};
