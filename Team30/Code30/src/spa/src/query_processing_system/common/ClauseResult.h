#pragma once
#include <unordered_map>
#include <unordered_set>
#include <vector>
#include <string>
#include <memory>
#include "PqlDeclaration.h"

class ClauseResult {
 private:
  std::unordered_map<PqlDeclaration,
                     std::vector<std::string>,
                     PqlDeclarationHash> value_map;

  std::unordered_map<PqlDeclaration,
                     std::unordered_set<PqlDeclaration, PqlDeclarationHash>,
                     PqlDeclarationHash> linkedDeclarations;

  void addLinkedDeclarations(PqlDeclaration d1, PqlDeclaration d2);

  static std::unique_ptr<std::vector<std::string>> getIntersectingValues(
      std::vector<std::string> &arr_1,
      std::vector<std::string> &arr_2);
  // may want to use pointers in the future to speed this up.
 public:
  // empty ClauseResult
  ClauseResult();

  // Constructor for single declaration clauses
  ClauseResult(PqlDeclaration declaration, std::vector<std::string> values);

  // Constructor for linked declaration clauses
  ClauseResult(PqlDeclaration d1, PqlDeclaration d2,
               std::vector<std::pair<std::string, std::string>> values);

  std::unique_ptr<ClauseResult> combineResults(ClauseResult &other);

  std::unique_ptr<std::vector<std::string>> getValues(
      PqlDeclaration declaration);
};
