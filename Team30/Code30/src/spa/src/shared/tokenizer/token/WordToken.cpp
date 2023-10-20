#include "WordToken.h"

WordToken::WordToken(std::string val) : Token(val) {}

TokenType WordToken::getTokenType() { return TokenType::WORD_TOKEN; };
