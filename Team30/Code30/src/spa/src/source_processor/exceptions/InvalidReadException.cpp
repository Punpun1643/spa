#include "InvalidReadException.h"

InvalidReadException::InvalidReadException(std::string const& what_arg)
    : invalid_argument(what_arg) {}