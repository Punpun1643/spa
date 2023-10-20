#include "InvalidAssignException.h"

InvalidAssignException::InvalidAssignException(std::string const& what_arg)
    : invalid_argument(what_arg) {}
