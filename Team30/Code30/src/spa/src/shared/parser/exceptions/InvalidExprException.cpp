#include "InvalidExprException.h"

InvalidExprException::InvalidExprException(std::string const& what_arg)
    : std::invalid_argument(what_arg) {}
