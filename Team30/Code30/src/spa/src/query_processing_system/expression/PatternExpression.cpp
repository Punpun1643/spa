#include "PatternExpression.h"

PatternExpression::PatternExpression(std::string syn_assign, std::string arg1,
                                     std::string arg2)
    : syn_assign(syn_assign), arg1(arg1), arg2(arg2){};

void PatternExpression::acceptInterpreter(QueryInterpreter& interpreter) {
  interpreter.Interpret(*this);
}

std::string PatternExpression::GetArg1() { return this->arg1; }
std::string PatternExpression::GetArg2() { return this->arg2; }
std::string PatternExpression::GetSynAssign() { return this->syn_assign; }
