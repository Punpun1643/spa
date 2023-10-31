#include "PatternExpression.h"

#include <memory>
#include <string>

PatternExpression::PatternExpression(std::string synonym, std::string arg1,
                                     std::string arg2, bool is_not)
    : ClauseExpression(arg1, arg2, is_not), synonym(synonym){}

std::string PatternExpression::GetSynonym() {
  return this->synonym;
}
