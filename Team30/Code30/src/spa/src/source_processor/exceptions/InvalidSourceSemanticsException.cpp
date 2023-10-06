#include "InvalidSourceSemanticsException.h"
InvalidSourceSemanticsException::InvalidSourceSemanticsException(
    std::string const& what_arg)
    : invalid_argument(what_arg) {}
