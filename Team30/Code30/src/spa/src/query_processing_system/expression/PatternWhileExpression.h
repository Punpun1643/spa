#pragma once

#include <string>

#include "../interpreter/QueryInterpreter.h"
#include "PatternExpression.h"

class PatternWhileExpression : public PatternExpression {
 public:
  PatternWhileExpression(std::string synonym, std::string arg1);

  void acceptInterpreter(QueryInterpreter& interpreter) override;

  ~PatternWhileExpression() = default;
};
