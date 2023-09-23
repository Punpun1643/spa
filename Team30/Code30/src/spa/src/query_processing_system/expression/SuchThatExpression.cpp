#include "SuchThatExpression.h"

SuchThatExpression ::SuchThatExpression(std::string arg1, std::string arg2)
    : arg1(arg1), arg2(arg2){};

std::string SuchThatExpression::GetArg1() { return this->arg1; }
std::string SuchThatExpression::GetArg2() { return this->arg2; }
