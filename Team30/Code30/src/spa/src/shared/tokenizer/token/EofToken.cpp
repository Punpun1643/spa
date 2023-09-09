#include "EofToken.h"

EofToken::EofToken(std::string val) : Token(val) {}

TokenType EofToken::getTokenType() { return TokenType::EOF_TOKEN; };