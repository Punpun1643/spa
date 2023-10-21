#include "UnmatchedParenthesesException.h"

UnmatchedParenthesesException::UnmatchedParenthesesException(
    std::string const& what_arg)
    : invalid_argument(what_arg) {}
