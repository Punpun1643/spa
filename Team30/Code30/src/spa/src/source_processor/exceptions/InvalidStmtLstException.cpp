#include "InvalidStmtLstException.h"

InvalidStmtLstException::InvalidStmtLstException(std::string const& what_arg)
    : invalid_argument(what_arg) {}