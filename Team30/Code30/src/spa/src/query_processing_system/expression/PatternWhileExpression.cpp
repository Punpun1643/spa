#include "PatternWhileExpression.h"

#include <memory>
#include <string>

PatternWhileExpression::PatternWhileExpression(std::string synonym,
                                               std::string arg1, bool is_not)
    : PatternExpression(synonym, arg1, "", is_not) {}

void PatternWhileExpression::AcceptInterpreter(QueryInterpreter& interpreter) {
  interpreter.Interpret(
      std::dynamic_pointer_cast<PatternWhileExpression>(shared_from_this()));
}
