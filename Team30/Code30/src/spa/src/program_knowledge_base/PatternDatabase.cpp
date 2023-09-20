#include "PatternDatabase.h"

PatternDatabase::PatternDatabase() {}

void PatternDatabase::insert(std::string i, std::string var,
                             std::unordered_set<std::string> vars_and_consts) {
  inverse_lhs[var].insert(i);

  for (std::string v : vars_and_consts) {
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

std::string PatternDatabase::getVarGivenStatementNum(
    std::string v) {
  return lhs[v];
}