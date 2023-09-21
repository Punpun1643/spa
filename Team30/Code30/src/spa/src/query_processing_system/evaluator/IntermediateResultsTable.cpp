#include "IntermediateResultsTable.h"

IntermediateResultsTable::IntermediateResultsTable() = default;

void IntermediateResultsTable::addBooleanClauseResult(bool result) {
  if (!result) {
    has_no_results = true;
  }
}

void IntermediateResultsTable::addSingleDeclaration(const PqlDeclaration & d, std::unique_ptr<std::vector<std::string>> values) {
  if (value_map.count(d) == 0) {
    value_map[d] = *values; // easy case
  }
  // Declaration already exists
  if (linked_declarations.count(d) == 0) {
    auto existing_values = value_map[d];
    auto combined_values = intersectLists(existing_values, *values);
    if (combined_values->empty()) {
      has_no_results = true;
      return;
    } else {
      value_map[d] = *combined_values;
    }
  } else {

  }
}

std::unique_ptr<std::vector<int>> IntermediateResultsTable::getIndicesToKeep(std::vector<std::string> arr,
                                                                                   std::vector<std::string> overlap_arr) {
  if (arr.empty() || overlap_arr.empty()) {
    return std::make_unique<std::vector<int>>();
  }

  std::sort(arr.begin(), arr.end());
  std::sort(overlap_arr.begin(), overlap_arr.end());

  auto idx_to_keep = std::make_unique<std::vector<int>>();
  int arr_idx = 0;
  int overlap_arr_idx = 0;

  while (arr_idx < arr.size() && overlap_arr_idx < overlap_arr.size()) {
    if (arr[arr_idx] == overlap_arr[overlap_arr_idx]) {
      idx_to_keep->push_back(arr_idx);
      arr_idx++;
      overlap_arr_idx++;
    } else if (arr[arr_idx] > overlap_arr[overlap_arr_idx]) {
      overlap_arr_idx++;
    } else {
      arr_idx++;
    }
  }
  return idx_to_keep;
}


std::unique_ptr<std::vector<std::string>> IntermediateResultsTable::intersectLists(std::vector<std::string> arr_1, std::vector<std::string> arr_2) {
  // Sort the vectors to use std::set_intersection
  std::sort(arr_1.begin(), arr_1.end());
  std::sort(arr_2.begin(), arr_2.end());

  std::vector<std::string> intersecting_values;
  std::set_intersection(arr_1.begin(), arr_1.end(), arr_2.begin(), arr_2.end(),
                        std::back_inserter(intersecting_values));
  return std::make_unique<std::vector<std::string>>(intersecting_values);
}


void IntermediateResultsTable::addClauseResult(const ClauseResult& clause_result) {
  if (has_no_results) {
    return; // don't bother, stuck at false already
  }

  switch(clause_result.getNumDeclarations()) {
    case 0: {
      bool result = clause_result.getBooleanClauseValue();
      addBooleanClauseResult(result);
      break;
    }
    case 1: {
      auto declaration = clause_result.getDeclarations()->front();
      auto values = clause_result.getValues(declaration);
      addSingleDeclaration(declaration, std::move(values));
      break;
    }
    case 2: {
      break;
    }
    default: {
      break;
    }
  }

}

std::unique_ptr<std::vector<std::string>> IntermediateResultsTable::getValuesGivenDeclaration(const PqlDeclaration & declaration) const{
  assert(value_map.count(declaration) == 1); // must have been in select stmt

  if (has_no_results) {
    return std::make_unique<std::vector<std::string>>(); // empty values
  }
  return std::make_unique<std::vector<std::string>>(value_map[declaration]);
}
