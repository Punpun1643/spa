#include "InvalidCondExprException.h"

InvalidCondExprException::InvalidCondExprException(std::string const& what_arg)
    : invalid_argument(what_arg) {}
