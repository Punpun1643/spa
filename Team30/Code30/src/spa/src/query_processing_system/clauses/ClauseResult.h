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
  std::optional<PqlDeclaration> d1;
  std::optional<PqlDeclaration> d2;
  std::vector<std::string> d1_values;
  std::vector<std::string> d2_values;

  void SetResultToFalse();

  void ConstructSingleDeclResult(PqlDeclaration d,
                                 std::vector<std::string> values);

 public:
  // Constructor for zero declaration clauses
  explicit ClauseResult(bool is_valid);

  // Constructor for single declaration clauses
  ClauseResult(PqlDeclaration declaration,
               std::vector<std::string> values);

  // Constructor for 2 declaration clauses
  ClauseResult(PqlDeclaration d1, PqlDeclaration d2,
               std::vector<std::pair<std::string, std::string>> values);

  int GetNumDeclarations() const;

  bool IsBooleanResult() const;

  bool GetBooleanClauseValue() const;

  std::vector<PqlDeclaration> GetDeclarations() const;

  std::vector<std::string> GetValues(PqlDeclaration const& declaration) const;

  // Get Paired Values?

  bool Contains(PqlDeclaration const& d) const;
};
