#pragma once
#include <unordered_map>
#include <vector>
#include <string>
#include "PqlDeclaration.h"

class ClauseResult {
 private:
  std::unordered_map<PqlDeclaration, std::vector<std::string>,
      PqlDeclarationHash> declaration_possible_values;
  // may wanna make pointers in the future to speed this up.
 public:
  // ClauseResult();
};