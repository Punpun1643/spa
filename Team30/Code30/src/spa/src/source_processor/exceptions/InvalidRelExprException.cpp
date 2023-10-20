#include "InvalidRelExprException.h"

InvalidRelExprException::InvalidRelExprException(std::string const& what_arg)
    : invalid_argument(what_arg) {}
