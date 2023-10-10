#include "StartOfFileException.h"

StartOfFileException::StartOfFileException(std::string const& errorMessage)
    : std::runtime_error(errorMessage) {}