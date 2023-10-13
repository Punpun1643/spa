#include "EmptyStmtLstException.h"

EmptyStmtLstException::EmptyStmtLstException(std::string const& what_arg)
    : invalid_argument(what_arg) {}