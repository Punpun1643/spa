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
                     PqlDeclarationHash> value_map;

  std::unordered_map<PqlDeclaration,
                     std::unordered_set<PqlDeclaration, PqlDeclarationHash>,
                     PqlDeclarationHash> linked_declarations;
//
//  std::unique_ptr<std::vector<PqlDeclaration>> getCommonDeclarations(
//      const ClauseResult& other) const;

  // may want to use pointers in the future to speed this up.
 public:
  // Constructor for zero declaration clauses
  ClauseResult(bool is_valid);

  // Constructor for single declaration clauses
  ClauseResult(PqlDeclaration declaration, std::unique_ptr<std::vector<std::string>> values);

  // Constructor for 2 declaration clauses
  ClauseResult(PqlDeclaration d1, PqlDeclaration d2,
               std::unique_ptr<std::vector<std::pair<std::string, std::string>>> values);

  int getNumDeclarations() const;

  bool isBooleanResult() const;

  bool getBooleanClauseValue() const;

//  std::unique_ptr<ClauseResult> combineResults(const ClauseResult& other) const;

  std::unique_ptr<std::vector<std::string>> getValues(
       const PqlDeclaration& declaration) const;

  bool contains(PqlDeclaration const& d) const;
};
