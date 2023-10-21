#include "InvalidTokenException.h"

InvalidTokenException::InvalidTokenException(std::string const& what_arg)
    : invalid_argument(what_arg) {}
