#pragma once

#include <memory>
#include <string>
#include <vector>

#include "../../program_knowledge_base/PkbApi.h"
#include "../common/SelectClause.h"
#include "../common/SuchThatClause.h"
#include "query_processing_system/common/PatternClause.h"

class QueryEvaluator {
 private:
  PkbApi& pkb;

 public:
  explicit QueryEvaluator(PkbApi& pkb);

  std::vector<std::string> evaluateQuery(
      std::unique_ptr<SelectClause> select_clause,
      std::vector<std::shared_ptr<Clause>> other_clauses);
};
