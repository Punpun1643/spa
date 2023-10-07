#include "ClauseExpression.h"

ClauseExpression::ClauseExpression(std::string arg1, std::string arg2)
    : arg1(arg1), arg2(arg2) {}

std::string ClauseExpression::GetArg1() { return this->arg1; }
std::string ClauseExpression::GetArg2() { return this->arg2; }
