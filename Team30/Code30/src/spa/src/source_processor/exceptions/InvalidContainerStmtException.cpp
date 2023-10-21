#include "InvalidContainerStmtException.h"

InvalidContainerStmtException::InvalidContainerStmtException(
    std::string const& what_arg)
    : invalid_argument(what_arg) {}
