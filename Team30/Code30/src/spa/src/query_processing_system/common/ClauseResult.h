#pragma once
#include <unordered_map>
#include <vector>
#include <string>
#include "PqlDeclaration.h"

class ClauseResult {
 private:
  std::unordered_map<PqlDeclaration, std::vector<std::string>,
      PqlDeclarationHash> results;

  static std::unique_ptr<std::vector<std::string>> getIntersectingValues(
      std::vector<std::string> &arr_1,
      std::vector<std::string> &arr_2);
  // may want to use pointers in the future to speed this up.
 public:
  ClauseResult();
  void insertData(PqlDeclaration declaration,
             std::vector<std::string> possible_values);

  std::unique_ptr<ClauseResult> combineResults(ClauseResult other);
};
