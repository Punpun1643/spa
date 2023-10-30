#pragma once

#include "../interpreter/QueryInterpreter.h"
#include "PatternExpression.h"

class PatternIfExpression : public PatternExpression {
 public:
  PatternIfExpression(std::string synonym, std::string arg1);

  void acceptInterpreter(QueryInterpreter& interpreter) override;

  ~PatternIfExpression() = default;
};
