#pragma once

#include <memory>
#include <unordered_map>

#include "RelationType.h"
#include "tables/BaseTable.h"
#include "tables/DictionaryTable.h"
#include "tables/UFDSTable.h"

class PatternDatabase {
  // stores {statement_number : assigned variable}
  std::unordered_map<std::string, std::string> lhs = {};
  // stores {assigned variable : statement_number}
  std::unordered_map<std::string, std::unordered_set<std::string>> inverse_lhs =
      {};
  // stores {RHS variable/constant : statement_number}
  std::unordered_map<std::string, std::unordered_set<std::string>> inverse_rhs =
      {};

 public:
  PatternDatabase();
  ~PatternDatabase() = default;
  void insert(std::string i, std::string lhs_var,
              std::unordered_set<std::string> rhs_vars_consts);

  std::unordered_set<std::string> getStatementNumbersGivenLHS(std::string v);
  std::unordered_set<std::string> getStatementNumbersGivenRHS(std::string v);
  std::string getVarGivenStatementNum(std::string v);
};