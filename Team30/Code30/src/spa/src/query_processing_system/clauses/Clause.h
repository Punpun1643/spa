#pragma once
#include "PKB.h"
#include "ClauseResult.h"

class Clause {
public:
    virtual ClauseResult evaluate(PKB pkb) = 0;
};
