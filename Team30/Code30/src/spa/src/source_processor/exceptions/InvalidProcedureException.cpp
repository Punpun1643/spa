#include "InvalidProcedureException.h"

InvalidProcedureException::InvalidProcedureException(
    std::string const& what_arg)
    : invalid_argument(what_arg) {}
