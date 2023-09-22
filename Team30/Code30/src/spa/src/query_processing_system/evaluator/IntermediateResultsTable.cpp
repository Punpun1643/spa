#include "IntermediateResultsTable.h"
#include "ArrayUtility.h"

IntermediateResultsTable::IntermediateResultsTable() = default;

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
      auto declaration = clause_result.getDeclarations().front();
      auto values = clause_result.getValues(declaration);
      addSingleDeclaration(declaration, *values);
      break;
    }
    case (2): {
      auto declarations = clause_result.getDeclarations();
      assert(declarations.size() == 2);
      auto d1 = declarations[0];
      auto d2 = declarations[1];
      auto d1_values = clause_result.getValues(d1);
      auto d2_values = clause_result.getValues(d2);
      addPairedDeclarations(d1, d2, *d1_values, *d2_values);
      break;
    }
    default: {
      break;
    }
  }
}

bool IntermediateResultsTable::hasNoResults() const {
  return has_no_results;
}

std::vector<std::string> IntermediateResultsTable::getValuesGivenDeclaration(const PqlDeclaration& declaration) {
  if (has_no_results) {
    return {};  // empty values
  } else {
    return getTableCol(declaration); // TODO
  }
}

void IntermediateResultsTable::addBooleanClauseResult(bool result) {
  if (!result) {
    has_no_results = true;
  } // otherwise, do nothing
}

void IntermediateResultsTable::addSingleDeclaration(const PqlDeclaration & d, const std::vector<std::string> &values) {
  // Declaration doesn't exist
  if (table_mapping.count(d) == 0) {
    addNewTable(d, values);
  } else {
    int table_idx = table_mapping[d];
    tables[table_idx].filter(d, values);
    if (tables[table_idx].is_empty()) {
      has_no_results = true;
      return;
    }
  }
}
//  // Declaration already exists
//  auto existing_values = value_map[d];
//  auto indices_to_keep = ArrayUtility::getIndicesToKeep(existing_values, values);
//  if (indices_to_keep->empty()) {
//    has_no_results = true;
//    return;
//  }
//  // filter this declaration
//  value_map[d] = *(ArrayUtility::getArrSubsetByIndices(value_map[d], *indices_to_keep));
//  // filter all connected declarations, if any
//  if (linked_declarations.count(d) == 1) {
//    for (const auto& linked_d: linked_declarations[d]) {
//      value_map[linked_d] = *(ArrayUtility::getArrSubsetByIndices(value_map[linked_d], *indices_to_keep));
//    }
//  }



//
void IntermediateResultsTable::addPairedDeclarations(
const PqlDeclaration & d1,
const PqlDeclaration & d2,
    const std::vector<std::string> & new_d1_values,
    const std::vector<std::string> & new_d2_values) {
  // Both declarations not in table
  if (table_mapping.count(d1) == 0 && table_mapping.count(d2) == 0) {
    addNewTable(d1, d2, new_d1_values, new_d2_values);

  } else if (table_mapping.count(d1) == 1 && table_mapping.count(d2) == 0) {
    RelationalTable new_table = RelationalTable({d1, d2}, {new_d1_values, new_d2_values});
    int d1_table_idx = table_mapping[d1];
    tables[d1_table_idx] = tables[d1_table_idx].join(new_table, d1);

//    auto existing_d1_values = value_map[d1];
//    // Mirrored
//    auto new_d1_indices_to_keep = ArrayUtility::getIndicesToKeep(*new_d1_values, value_map[d1]);
//    addSingleDeclaration(d1, std::move(new_d1_values));
//    auto filtered_new_d2 = ArrayUtility::getArrSubsetByIndices(*new_d2_values, *new_d1_indices_to_keep);
//    value_map[d2] = *filtered_new_d2;
//    linked_declarations[]

  } else if (table_mapping.count(d1) == 0 && table_mapping.count(d2) == 1) {
    RelationalTable new_table = RelationalTable({d1, d2}, {new_d1_values, new_d2_values});
    int d2_table_idx = table_mapping[d2];
    tables[d2_table_idx] = tables[d2_table_idx].join(new_table, d2);
  } else {
    // case 1: d1 and d2 in same table already
    if (table_mapping[d1] == table_mapping[d2]) {
      RelationalTable new_table = RelationalTable({d1, d2}, {new_d1_values, new_d2_values});
      int table_idx = table_mapping[d1];
      tables[table_idx] = tables[table_idx].join(new_table, d1, d2);
    } else {
      // case 2: d1 and d2 are in different tables (impossible for now? just do select clause last)
      assert(false);
    }
  }
}


std::vector<std::string> IntermediateResultsTable::getTableCol(const PqlDeclaration& d) {
  assert(column_mapping.count(d) == 1);
  int col_idx = column_mapping[d];

  std::vector<std::string> output = {};
  for (auto& row: table) {
    output.push_back(row[col_idx]);
  }
  return output;
}