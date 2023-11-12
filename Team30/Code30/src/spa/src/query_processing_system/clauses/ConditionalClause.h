#pragma once
#include "Clause.h"


class ConditionalClause: public Clause {

  virtual std::unique_ptr<ClauseResult> EvaluateOnCondition(PKBQPSInterface& pkb,
                                                 std::unordered_set<std::string> const& decl_1_subset = {},
                                                 std::unordered_set<std::string> const& decl_2_subset = {}) = 0;
  virtual ~ConditionalClause();
};

