#pragma once

#include <unordered_set>
#include <memory>
#include <unordered_map>
#include <utility>
#include <string>

#include "PatternType.h"
#include "RelationType.h"
#include "tables/BaseTable.h"
#include "tables/DictionaryTable.h"
#include "../shared/parser/node/TreeNode.h"

class PatternDatabase {
 private:
  // {statement_number, (lhs, rhs)}
  std::unordered_map<std::string,
                     std::pair<std::string, std::shared_ptr<TreeNode>>>
      assignments;

  // {lhs variable : statement_number}, used for optimisation
  std::unordered_map<std::string, std::unordered_set<std::string>>
      lhs_assignments = {};

 public:
  PatternDatabase();
  ~PatternDatabase() = default;

  void Insert(PatternType type, std::string line_num, std::string lhs,
              std::shared_ptr<TreeNode> rhs);
  std::unordered_set<std::string> GetStatementNumbersGivenLHS(std::string v);
  std::unordered_map<std::string,
                     std::pair<std::string, std::shared_ptr<TreeNode>>>
  GetAssignmentPatterns();
};
