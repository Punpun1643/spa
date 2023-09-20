#pragma once

#include "../../spa/src/shared/tokenizer/token/EofToken.h"
#include "../../spa/src/shared/tokenizer/token/IntegerToken.h"
#include "../../spa/src/shared/tokenizer/token/SpecialCharToken.h"
#include "../../spa/src/shared/tokenizer/token/Token.h"
#include "../../spa/src/shared/tokenizer/token/WordToken.h"

void AddWordVector(std::vector<std::shared_ptr<Token>>& tokens, std::vector<std::string> word_vector);
void AddSpecialCharVector(std::vector<std::shared_ptr<Token>>& tokens, std::vector<std::string> special_char_vector);
void AddEOF(std::vector<std::shared_ptr<Token>>& tokens);
void AddInteger(std::vector<std::shared_ptr<Token>>& tokens, std::string integer);
