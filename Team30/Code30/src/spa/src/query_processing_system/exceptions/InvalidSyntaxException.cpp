#include "InvalidSyntaxException.h"

#include <string>

InvalidSyntaxException::InvalidSyntaxException(std::string const& what_arg)
    : invalid_argument(what_arg) {}
