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

std::unique_ptr<std::unordered_map<std::string, int>> IntermediateResultsTable::getValueCounts(const std::vector<std::string>& arr) {
  auto value_counts = std::make_unique<std::unordered_map<std::string, int>>();
  for (const std::string &value: arr) {
    if (value_counts->count(value) == 0) {
      (*value_counts)[value] = 1;
    } else {
      (*value_counts)[value] += 1;
    }
  }
  return value_counts;
}

std::unique_ptr<std::vector<int>> IntermediateResultsTable::getIndicesToKeep(std::vector<std::string> arr,
                                                                                   std::vector<std::string> overlap_arr) {
  /**
   * Given an arr and an overlap_arr, returns the list of indices in arr that should be kept
   * for all values in arr to also be present in overlap_arr. Takes into account duplicate
   * values.
   * Example: (['4','2','2','1'], ['2','2','4']) -> [0,1,2]
   */
  auto idx_to_keep = std::make_unique<std::vector<int>>();

  if (arr.empty() || overlap_arr.empty()) {
    return idx_to_keep; // empty
  }

  auto overlap_arr_value_counts = IntermediateResultsTable::getValueCounts(overlap_arr);

  for (int i = 0; i < arr.size(); i++) {
    if (overlap_arr_value_counts->count(arr[i]) == 0 ||
        (*overlap_arr_value_counts)[arr[i]] <= 0) {
      continue;
    } else {
      (*overlap_arr_value_counts)[arr[i]] -= 1;
      idx_to_keep->push_back(i);
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
    case (0): {
      bool result = clause_result.getBooleanClauseValue();
      addBooleanClauseResult(result);
      break;
    }
    case (1): {
//      auto declaration = clause_result.getDeclarations()->front();
//      auto values = clause_result.getValues(declaration);
//      addSingleDeclaration(declaration, std::move(values));
      break;
    }
    case (2): {
      break;
    }
    default: {
      break;
    }
  }
}

std::unique_ptr<std::vector<std::string>> IntermediateResultsTable::getValuesGivenDeclaration(const PqlDeclaration& declaration){
  assert(value_map.count(declaration) == 1); // must have been in select stmt

  if (has_no_results) {
    return std::make_unique<std::vector<std::string>>(); // empty values
  }
  return std::make_unique<std::vector<std::string>>(value_map[declaration]);
}
