#pragma once
#include <memory>

#include "../../program_knowledge_base/PKBQPSInterface.h"
#include "ClauseResult.h"

class Clause {
 private:
  bool is_not_clause = false;

 public:
  virtual std::unique_ptr<ClauseResult> Evaluate(PKBQPSInterface& pkb) = 0;
  virtual ~Clause();

  void FlagAsNotClause();
  bool IsNotClause() const;
};
