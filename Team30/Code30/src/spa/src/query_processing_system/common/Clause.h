#pragma once

#include <memory>

#include "../../program_knowledge_base/PKB.h"
#include "ClauseResult.h"

class Clause {
public:
    virtual std::unique_ptr<ClauseResult> evaluate(PKB& pkb) = 0;
};
