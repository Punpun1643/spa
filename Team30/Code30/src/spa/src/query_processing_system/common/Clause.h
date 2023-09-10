#pragma once
#include <memory>
#include "../../program_knowledge_base/PkbApi.h"
#include "ClauseResult.h"

class Clause {
 public:
  virtual std::unique_ptr<ClauseResult> evaluate(PkbApi& pkb) = 0;
  virtual ~Clause();
};
