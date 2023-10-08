#include "EndOfFileException.h"

EndOfFileException::EndOfFileException(std::string const& errorMessage)
    : std::runtime_error(errorMessage) {}
