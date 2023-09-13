#pragma once

#include <memory>
#include <string>
#include <vector>

#include "../../program_knowledge_base/PkbApi.h"
#include "../common/SelectClause.h"
#include "../common/SuchThatClause.h"

class QueryEvaluator {
 private:
  PkbApi& pkb;

  static std::unique_ptr<std::vector<std::string>> getIntersectingValues(
      std::vector<std::string> arr_1, std::vector<std::string> arr_2);

 public:
  explicit QueryEvaluator(PkbApi& pkb);

  std::unique_ptr<std::vector<std::string>> evaluateQuery(
      std::unique_ptr<SelectClause> select_clause);

  std::unique_ptr<std::vector<std::string>> evaluateQuery(
      std::unique_ptr<SelectClause> select_clause,
      std::unique_ptr<SuchThatClause> such_that_clause);
};
