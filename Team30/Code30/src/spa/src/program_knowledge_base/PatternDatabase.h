#pragma once

#include <memory>
#include <unordered_map>

#include "RelationType.h"
#include "tables/BaseTable.h"
#include "tables/DictionaryTable.h"
#include "tables/UFDSTable.h"

class PatternDatabase {
  std::unordered_map<std::string, std::string> lhs = {};
  std::unordered_map<std::string, std::unordered_set<std::string>> inverse_lhs =
      {};
  std::unordered_map<std::string, std::unordered_set<std::string>> inverse_rhs =
      {};

 public:
  PatternDatabase();
  ~PatternDatabase() = default;
  void insert(std::string i, std::string var,
              std::unordered_set<std::string> vars_and_consts);

  std::unordered_set<std::string> getStatementNumbersGivenLHS(std::string v);
  std::unordered_set<std::string> getStatementNumbersGivenRHS(std::string v);
  std::string getVarGivenStatementNum(std::string v);
};