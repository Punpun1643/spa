#include "EmptyParenthesesException.h"

EmptyParenthesesException::EmptyParenthesesException(
    std::string const& what_arg)
    : invalid_argument(what_arg) {}