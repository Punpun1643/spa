#include "EmptySourceFileException.h"

EmptySourceFileException::EmptySourceFileException(std::string const& what_arg)
    : invalid_argument(what_arg) {}
