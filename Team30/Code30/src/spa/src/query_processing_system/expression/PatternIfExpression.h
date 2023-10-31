#pragma once

#include <string>

#include "../interpreter/QueryInterpreter.h"
#include "PatternExpression.h"

class PatternIfExpression : public PatternExpression {
 public:
  PatternIfExpression(std::string synonym, std::string arg1, bool is_not);

  void AcceptInterpreter(QueryInterpreter& interpreter) override;

  ~PatternIfExpression() = default;
};
