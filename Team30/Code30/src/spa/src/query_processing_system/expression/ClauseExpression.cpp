#include "ClauseExpression.h"

ClauseExpression::ClauseExpression(std::string arg1, std::string arg2, bool is_not)
    : arg1(arg1), arg2(arg2), is_not(is_not) {}

std::string ClauseExpression::GetArg1() const {
  return this->arg1;
}
std::string ClauseExpression::GetArg2() const {
  return this->arg2;
}
bool ClauseExpression::IsNot() const {
  return this->is_not;
}
