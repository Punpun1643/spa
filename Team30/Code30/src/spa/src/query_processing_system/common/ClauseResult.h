#pragma once
#include <memory>
#include <string>
#include <unordered_map>
#include <unordered_set>
#include <vector>

#include "PqlDeclaration.h"

class ClauseResult {
 private:
  int num_declarations;
  bool boolean_clause_value;
  std::unordered_map<PqlDeclaration, std::vector<std::string>,
                     PqlDeclarationHash>
      value_map;

 public:
  // Constructor for zero declaration clauses
  explicit ClauseResult(bool is_valid);

  // Constructor for single declaration clauses
  ClauseResult(const PqlDeclaration& declaration,
               const std::vector<std::string>& values);

  // Constructor for 2 declaration clauses
  ClauseResult(
      const PqlDeclaration& d1, const PqlDeclaration& d2,
      const std::vector<std::pair<std::string, std::string>>& values);

  int getNumDeclarations() const;

  bool isBooleanResult() const;

  bool getBooleanClauseValue() const;

  std::vector<PqlDeclaration> getDeclarations() const;

  std::unique_ptr<std::vector<std::string>> getValues(
      PqlDeclaration const& declaration) const;

  bool contains(PqlDeclaration const& d) const;
};
