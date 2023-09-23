#include "PatternDatabase.h"

PatternDatabase::PatternDatabase() {}

void PatternDatabase::insert(std::string i, std::string lhs_var,
                             std::unordered_set<std::string> rhs_vars_consts) {
  lhs[i] = lhs_var;
  inverse_lhs[lhs_var].insert(i);
  for (std::string v : rhs_vars_consts) {
    inverse_rhs[v].insert(i);
  };
};

std::unordered_set<std::string> PatternDatabase::getStatementNumbersGivenLHS(
    std::string v) {
  return inverse_lhs[v];
}

std::unordered_set<std::string> PatternDatabase::getStatementNumbersGivenRHS(
    std::string v) {
  return inverse_rhs[v];
}

std::string PatternDatabase::getVarGivenStatementNum(std::string v) {
  return lhs[v];
}