#pragma once
#include <memory>

#include "../../program_knowledge_base/PKBQPSInterface.h"
#include "ClauseResult.h"

class Clause {
 public:
  virtual std::unique_ptr<ClauseResult> evaluate(PKBQPSInterface& pkb) = 0;
  virtual ~Clause();
};
