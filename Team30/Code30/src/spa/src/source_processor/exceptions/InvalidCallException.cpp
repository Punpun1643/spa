#include "InvalidCallException.h"

InvalidCallException::InvalidCallException(std::string const& what_arg)
    : invalid_argument(what_arg) {}
