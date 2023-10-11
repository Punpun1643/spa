#include "PatternDatabase.h"

PatternDatabase::PatternDatabase() {}

void PatternDatabase::insert(PatternType type, std::string line_num,
                             std::string lhs, std::shared_ptr<TreeNode> rhs) {
  assignments.insert({line_num, {lhs, rhs}});
  lhs_assignments[lhs].insert(line_num);
}

std::unordered_set<std::string> PatternDatabase::getStatementNumbersGivenLHS(
    std::string v) {
  return lhs_assignments[v];
}

std::unordered_map<std::string,
                   std::pair<std::string, std::shared_ptr<TreeNode>>>
PatternDatabase::getAssignmentPatterns() {
  return assignments;
};

// ------------ TO DELETE -------------------
void PatternDatabase::insert(std::string i, std::string lhs_var,
                             std::unordered_set<std::string> rhs_vars_consts) {
  lhs[i] = lhs_var;
  lhs_assignments[lhs_var].insert(i);
  for (std::string v : rhs_vars_consts) {
    inverse_rhs[v].insert(i);
  };
};

std::unordered_set<std::string> PatternDatabase::getStatementNumbersGivenRHS(
    std::string v) {
  return inverse_rhs[v];
}

std::string PatternDatabase::getVarGivenStatementNum(std::string v) {
  return lhs[v];
}
