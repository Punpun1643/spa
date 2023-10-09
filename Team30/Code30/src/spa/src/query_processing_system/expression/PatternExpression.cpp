#include "PatternExpression.h"

PatternExpression::PatternExpression(std::string syn_assign, std::string arg1,
                                     std::string arg2)
    : ClauseExpression(arg1, arg2), syn_assign(syn_assign){};

void PatternExpression::acceptInterpreter(QueryInterpreter& interpreter) {
  interpreter.Interpret(
      std::dynamic_pointer_cast<PatternExpression>(shared_from_this()));
}

std::string PatternExpression::GetSynAssign() { return this->syn_assign; }
