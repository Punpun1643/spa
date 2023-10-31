#pragma once

#include <string>

#include "../interpreter/QueryInterpreter.h"
#include "PatternExpression.h"

class PatternWhileExpression : public PatternExpression {
 public:
  PatternWhileExpression(std::string synonym, std::string arg1, bool is_not);

  void AcceptInterpreter(QueryInterpreter& interpreter) override;

  ~PatternWhileExpression() = default;
};
