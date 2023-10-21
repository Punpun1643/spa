#include "InvalidExprSpecException.h"

InvalidExprSpecException::InvalidExprSpecException(std::string const& what_arg)
    : std::invalid_argument(what_arg) {}
