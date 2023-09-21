#include "IntermediateResultsTable.h"
#include "ArrayUtility.h"

IntermediateResultsTable::IntermediateResultsTable() = default;

void IntermediateResultsTable::addBooleanClauseResult(bool result) {
  if (!result) {
    has_no_results = true;
  } // otherwise, do nothing
}

void IntermediateResultsTable::addSingleDeclaration(const PqlDeclaration & d, std::unique_ptr<std::vector<std::string>> values) {
  // Declaration doesn't exist
  if (value_map.count(d) == 0) {
    value_map[d] = *values; // easy case
  }
  // Declaration already exists
  auto existing_values = value_map[d];
  auto indices_to_keep = ArrayUtility::getIndicesToKeep(existing_values, *values);
  if (indices_to_keep->empty()) {
    has_no_results = true;
    return;
  }
  // filter this declaration
  value_map[d] = *(ArrayUtility::getArrSubsetByIndices(value_map[d], *indices_to_keep));
  // filter all connected declarations, if any
  if (linked_declarations.count(d) == 1) {
    for (const auto& linked_d: linked_declarations[d]) {
      value_map[linked_d] = *(ArrayUtility::getArrSubsetByIndices(value_map[linked_d], *indices_to_keep));
    }
  }
}

void IntermediateResultsTable::addPairedDeclarations(const PqlDeclaration & d1, const PqlDeclaration & d2, std::unique_ptr<std::vector<std::string>> d1_values, std::unique_ptr<std::vector<std::string>> d2_values) {

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
