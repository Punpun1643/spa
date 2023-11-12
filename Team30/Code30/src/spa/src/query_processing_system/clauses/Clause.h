#pragma once
#include <memory>

#include "../../program_knowledge_base/PKBQPSInterface.h"
#include "ClauseResult.h"

class Clause {
 private:
  bool is_negated = false;

 public:
  virtual std::unique_ptr<ClauseResult> Evaluate(PKBQPSInterface& pkb) = 0;
  virtual ~Clause();

  virtual int GetNumDeclarations() const = 0;
  virtual std::vector<PqlDeclaration> GetDeclarations() const = 0;

  void FlagAsNegated();
  bool IsNegated() const;
};
