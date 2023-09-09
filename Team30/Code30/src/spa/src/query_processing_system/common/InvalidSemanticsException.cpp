//
// Created by Lim Wei Liang on 9/9/23.
//

#include "InvalidSemanticsException.h"
InvalidSemanticsException::InvalidSemanticsException(
    const std::string& what_arg): invalid_argument(what_arg) {}
