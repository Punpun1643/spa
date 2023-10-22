#pragma once

#include <memory>
#include <string>
#include <unordered_map>
#include <unordered_set>
#include <utility>
#include <vector>

#include "query_processing_system/references/PqlDeclaration.h"

class ClauseResult {
 private:
  int num_declarations;
  bool boolean_clause_value;
  std::unordered_map<PqlDeclaration, std::vector<std::string>,
                     PqlDeclarationHash>
      value_map;

  void SetResultToFalse();

  void ConstructSingleDeclResult(PqlDeclaration const& d,
                                 std::vector<std::string> const& values);

 public:
  // Constructor for zero declaration clauses
  explicit ClauseResult(bool is_valid);

  // Constructor for single declaration clauses
  ClauseResult(PqlDeclaration const& declaration,
               std::vector<std::string> const& values);

  // Constructor for 2 declaration clauses
  ClauseResult(PqlDeclaration const& d1, PqlDeclaration const& d2,
               std::vector<std::pair<std::string, std::string>> const& values);

  int GetNumDeclarations() const;

  bool IsBooleanResult() const;

  bool GetBooleanClauseValue() const;

  std::vector<PqlDeclaration> GetDeclarations() const;

  std::vector<std::string> GetValues(PqlDeclaration const& declaration) const;

  bool Contains(PqlDeclaration const& d) const;
};
