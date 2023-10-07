#include "InvalidIfException.h"

InvalidIfException::InvalidIfException(std::string const& what_arg)
    : invalid_argument(what_arg) {}