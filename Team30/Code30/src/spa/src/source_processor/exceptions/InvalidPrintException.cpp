#include "InvalidPrintException.h"

InvalidPrintException::InvalidPrintException(std::string const& what_arg)
    : invalid_argument(what_arg) {}
