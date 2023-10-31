#include "PatternExpression.h"

#include <memory>
#include <string>

PatternExpression::PatternExpression(std::string synonym, std::string arg1,
                                     std::string arg2)
    : ClauseExpression(arg1, arg2), synonym(synonym) {}

std::string PatternExpression::GetSynonym() {
  return this->synonym;
}
