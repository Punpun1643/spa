#pragma once

#include <memory>
#include <string>
#include <vector>

#include "../../program_knowledge_base/PKBQPSInterface.h"
#include "../common/SelectClause.h"
#include "../common/SuchThatClause.h"
#include "query_processing_system/common/PatternClause.h"

class QueryEvaluator {
 private:
  PKBQPSInterface& pkb;

 public:
  explicit QueryEvaluator(PKBQPSInterface& pkb);

  std::vector<std::string> evaluateQuery(
      std::unique_ptr<SelectClause> select_clause,
      std::vector<std::shared_ptr<Clause>> const& other_clauses);
};
