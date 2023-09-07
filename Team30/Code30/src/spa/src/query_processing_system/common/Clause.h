#pragma once
#include "../../program_knowledge_base/PKB.h"
#include "ClauseResult.h"

class Clause {
public:
    virtual ClauseResult evaluate(PKB& pkb) = 0;
};
