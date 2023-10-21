#pragma once

#include <string>

#include "../interpreter/QueryInterpreter.h"
#include "SuchThatExpression.h"

class AffectsExpression : public SuchThatExpression {
 public:
  AffectsExpression(std::string arg1, std::string arg2);

  void acceptInterpreter(QueryInterpreter& interpreter) override;

  ~AffectsExpression() = default;
};
