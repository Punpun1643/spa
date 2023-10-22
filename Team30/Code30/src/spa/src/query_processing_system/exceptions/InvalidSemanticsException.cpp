#include "InvalidSemanticsException.h"

#include <string>
InvalidSemanticsException::InvalidSemanticsException(
    std::string const& what_arg)
    : invalid_argument(what_arg) {}
