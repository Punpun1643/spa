#include "catch.hpp"
#include "query_processing_system/evaluator/IntermediateResultsTable.h"

TEST_CASE("Test getIndicesToKeep") {
  // Edge cases
  auto result = IntermediateResultsTable::getIndicesToKeep({}, {"blah","de"});
  REQUIRE(*result == std::vector<int>());

  result = IntermediateResultsTable::getIndicesToKeep({"blah","de"}, {});
  REQUIRE(*result == std::vector<int>());

  std::vector<std::string> arr = {"65","20","40", "40", "20","10"};
  std::vector<std::string> overlap_arr = {"20","20", "40", "10"};

  result = IntermediateResultsTable::getIndicesToKeep(arr, overlap_arr);
  REQUIRE(*result == std::vector<int>({1,2,4,5}));

  // try with longer strings
  arr = {"varX","varY","count", "idx", "idx", "idx"};
  overlap_arr = {"idx","idx", "count", "nonsense", "otherNonsense"};

  result = IntermediateResultsTable::getIndicesToKeep(arr, overlap_arr);
  REQUIRE(*result == std::vector<int>({2,3,4}));
}
