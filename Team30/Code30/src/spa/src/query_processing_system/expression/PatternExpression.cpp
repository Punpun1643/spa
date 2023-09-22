#include "PatternExpression.h"

PatternExpression::PatternExpression(std::string syn_assign,
                                     std::string pattern)
    : syn_assign(syn_assign), pattern(pattern){};

void PatternExpression::acceptInterpreter(QueryInterpreter& interpreter) {
  interpreter.Interpret(*this);
}

std::string PatternExpression::GetPattern() { return this->pattern; }

std::string PatternExpression::GetSynAssign() { return this->syn_assign; }
