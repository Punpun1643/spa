#include "PatternWhileExpression.h"

#include <memory>
#include <string>

PatternWhileExpression::PatternWhileExpression(std::string synonym,
                                               std::string arg1)
    : PatternExpression(synonym, arg1, ""){};

void PatternWhileExpression::acceptInterpreter(QueryInterpreter& interpreter) {
  interpreter.Interpret(
      std::dynamic_pointer_cast<PatternWhileExpression>(shared_from_this()));
}
