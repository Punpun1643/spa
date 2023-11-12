#pragma once
#include "Clause.h"

class ConditionalClause : public Clause {
 public:
  virtual std::unique_ptr<ClauseResult> EvaluateOnCondition(
      PKBQPSInterface& pkb, std::unordered_set<std::string>& decl_1_subset,
      std::unordered_set<std::string>& decl_2_subset) = 0;
  virtual ~ConditionalClause();

  virtual int GetNumDeclarations() const = 0;
  virtual std::vector<PqlDeclaration> GetDeclarations() const = 0;
};
