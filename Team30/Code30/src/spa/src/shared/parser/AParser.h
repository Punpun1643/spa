#pragma once
//using namespace std;
//int Parse();

#include "../../source_processor/node/ProgramNode.h"
#include "../tokenizer/token/Token.h"

class AParser {
public:
    AParser();

    ProgramNode parse() {};

    Token nextToken();

    Token peekToken();

    Token getCurrToken();

    virtual ~AParser() = default;
};