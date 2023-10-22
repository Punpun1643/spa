#include "../../../../spa/src/query_processing_system/clauses/ClauseResult.h"
#include "catch.hpp"
#include "query_processing_system/evaluator/ArrayUtility.h"
#include "query_processing_system/evaluator/IntermediateResultsTable.h"

// ASSUMPTIONS: Tests for RelationalTable all pass.

namespace IrtTestHelperMethods {
std::vector<std::pair<std::string, std::string>> makePairedVector(
    std::vector<std::string> v1, std::vector<std::string> v2) {
  assert(v1.size() == v2.size());
  auto list_of_pairs = std::vector<std::pair<std::string, std::string>>();
  for (auto i = 0; i < v1.size(); i++) {
    list_of_pairs.push_back(std::make_pair(v1[i], v2[i]));
  }
  return list_of_pairs;
}

std::vector<std::vector<std::string>> addDim(std::vector<std::string>& vec) {
  std::vector<std::vector<std::string>> output = {};
  for (auto const& value: vec) {
    output.push_back({value});
  }
  return output;
}
}  // namespace IrtTestHelperMethods

TEST_CASE("Intermediate Results Table Tests") {
  // Declarations
  auto a = PqlDeclaration("a", EntityType::ASSIGN);
  auto b = PqlDeclaration("b", EntityType::PRINT);
  auto c = PqlDeclaration("constant", EntityType::CONSTANT);
  auto v = PqlDeclaration("variable", EntityType::VARIABLE);
  auto s = PqlDeclaration("stmt", EntityType::STMT);

  auto TRUE_CLAUSE = ClauseResult(true);
  auto FALSE_CLAUSE = ClauseResult(false);

  auto LIST_A = std::vector<std::string>({"1", "2", "3"});
  std::vector<std::string> LIST_A_PARTIAL = {LIST_A[1]};
  auto LIST_B = std::vector<std::string>({"2", "3", "4"});
  std::vector<std::string> LIST_B_PARTIAL = {LIST_B[1]};
  auto LIST_C = std::vector<std::string>({"20", "30", "40"});
  std::vector<std::string> LIST_C_PARTIAL = {LIST_C[1]};
  auto LIST_V = std::vector<std::string>({"a", "b", "c"});
  std::vector<std::string> LIST_V_PARTIAL = {LIST_V[1]};
  auto LIST_S = std::vector<std::string>({"5", "6", "7"});

  auto LIST_A_OUTPUT = IrtTestHelperMethods::addDim(LIST_A);
  auto LIST_A_PARTIAL_OUTPUT = IrtTestHelperMethods::addDim(LIST_A_PARTIAL);
  auto LIST_B_OUTPUT = IrtTestHelperMethods::addDim(LIST_B);
  auto LIST_B_PARTIAL_OUTPUT = IrtTestHelperMethods::addDim(LIST_B_PARTIAL);
  auto LIST_C_OUTPUT = IrtTestHelperMethods::addDim(LIST_C);
  auto LIST_C_PARTIAL_OUTPUT = IrtTestHelperMethods::addDim(LIST_C_PARTIAL);
  auto LIST_V_OUTPUT = IrtTestHelperMethods::addDim(LIST_V);
  auto LIST_V_PARTIAL_OUTPUT = IrtTestHelperMethods::addDim(LIST_V_PARTIAL);
  auto LIST_S_OUTPUT = IrtTestHelperMethods::addDim(LIST_S);

  auto SINGLE_CLAUSE_A = ClauseResult(a, LIST_A);
  auto SINGLE_CLAUSE_B = ClauseResult(b, LIST_B);
  auto SINGLE_CLAUSE_C = ClauseResult(c, LIST_C);
  auto SINGLE_CLAUSE_V = ClauseResult(v, LIST_V);
  auto CONTRADICTING_CLAUSE_A = ClauseResult(a, LIST_C);
  auto PAIRED_CLAUSE_A_B = ClauseResult(
      a, b, IrtTestHelperMethods::makePairedVector(LIST_A, LIST_B));
  auto PAIRED_CLAUSE_A_C = ClauseResult(
      a, c, IrtTestHelperMethods::makePairedVector(LIST_A, LIST_C));
  auto PAIRED_CLAUSE_B_C = ClauseResult(
      b, c, IrtTestHelperMethods::makePairedVector(LIST_B, LIST_C));
  auto PAIRED_CLAUSE_B_C_PARTIAL = ClauseResult(
      b, c, IrtTestHelperMethods::makePairedVector({LIST_B[1]}, {LIST_C[1]}));
  auto PAIRED_CLAUSE_V_S = ClauseResult(
      v, s, IrtTestHelperMethods::makePairedVector(LIST_V, LIST_S));
  auto CONTRADICTING_CLAUSE_A_B = ClauseResult(
      a, b, IrtTestHelperMethods::makePairedVector(LIST_V, LIST_V));

  auto irt = IntermediateResultsTable();

  // BOOLEAN TESTS
  SECTION("Test only boolean clauses") {
    irt.AddClauseResult(TRUE_CLAUSE);
    irt.AddClauseResult(TRUE_CLAUSE);
    irt.AddClauseResult(FALSE_CLAUSE);
    REQUIRE(irt.HasNoResults());
    irt.AddClauseResult(FALSE_CLAUSE);
    REQUIRE(irt.HasNoResults());
  }

  SECTION("Test boolean + single clause") {
    irt.AddClauseResult(SINGLE_CLAUSE_A);
    REQUIRE_FALSE(irt.HasNoResults());
    irt.AddClauseResult(TRUE_CLAUSE);
    REQUIRE_FALSE(irt.HasNoResults());
    REQUIRE(irt.GetValuesGivenDeclarations({a}) == LIST_A_OUTPUT);
    irt.AddClauseResult(FALSE_CLAUSE);
    REQUIRE(irt.HasNoResults());
    irt.AddClauseResult(SINGLE_CLAUSE_A);
    REQUIRE(irt.HasNoResults());
  }

  SECTION("Test boolean + paired clause") {
    irt.AddClauseResult(PAIRED_CLAUSE_A_B);
    REQUIRE_FALSE(irt.HasNoResults());
    irt.AddClauseResult(TRUE_CLAUSE);
    REQUIRE_FALSE(irt.HasNoResults());
    REQUIRE(irt.GetValuesGivenDeclarations({a}) == LIST_A_OUTPUT);
    REQUIRE(irt.GetValuesGivenDeclarations({b}) == LIST_B_OUTPUT);
    irt.AddClauseResult(FALSE_CLAUSE);
    REQUIRE(irt.HasNoResults());
    irt.AddClauseResult(PAIRED_CLAUSE_A_B);
    REQUIRE(irt.HasNoResults());
  }

  // SINGLE RESULT CLAUSE TESTS
  SECTION("Test single result clauses") {
    irt.AddClauseResult(SINGLE_CLAUSE_B);
    irt.AddClauseResult(SINGLE_CLAUSE_A);
    REQUIRE(irt.GetValuesGivenDeclarations({a}) == LIST_A_OUTPUT);
    REQUIRE(irt.GetValuesGivenDeclarations({b}) == LIST_B_OUTPUT);
    // changes to A should not affect B
    irt.AddClauseResult(ClauseResult(a, LIST_A_PARTIAL));
    REQUIRE(irt.GetValuesGivenDeclarations({a}) == LIST_A_PARTIAL_OUTPUT);
    REQUIRE(irt.GetValuesGivenDeclarations({b}) == LIST_B_OUTPUT);
    // adding in a third unrelated clause should not affect the first two
    irt.AddClauseResult(SINGLE_CLAUSE_V);
    REQUIRE_FALSE(irt.HasNoResults());
    REQUIRE(irt.GetValuesGivenDeclarations({v}) == LIST_V_OUTPUT);
    REQUIRE(irt.GetValuesGivenDeclarations({a}) == LIST_A_PARTIAL_OUTPUT);
    REQUIRE(irt.GetValuesGivenDeclarations({b}) == LIST_B_OUTPUT);
  }

  SECTION("Single result clauses that contradict") {
    irt.AddClauseResult(SINGLE_CLAUSE_B);
    irt.AddClauseResult(SINGLE_CLAUSE_A);
    irt.AddClauseResult(CONTRADICTING_CLAUSE_A);
    REQUIRE(irt.HasNoResults());
    REQUIRE(irt.GetValuesGivenDeclarations({a}).empty());
    REQUIRE(irt.GetValuesGivenDeclarations({b}).empty());
    // adding in a new clause doesn't change things
    irt.AddClauseResult(SINGLE_CLAUSE_V);
    REQUIRE(irt.GetValuesGivenDeclarations({v}).empty());
  }

  // Full tests with at least 1 x single and 2 x paired clauses (for milestone
  // 1)
  SECTION("All synonyms different") {
    irt.AddClauseResult(SINGLE_CLAUSE_A);
    irt.AddClauseResult(PAIRED_CLAUSE_B_C);
    irt.AddClauseResult(PAIRED_CLAUSE_V_S);
    REQUIRE_FALSE(irt.HasNoResults());
    REQUIRE(irt.GetValuesGivenDeclarations({a}) == LIST_A_OUTPUT);
    REQUIRE(irt.GetValuesGivenDeclarations({c}) == LIST_C_OUTPUT);
    REQUIRE(irt.GetValuesGivenDeclarations({v}) == LIST_V_OUTPUT);
    // Check that none of the clauses are linked
    irt.AddClauseResult(PAIRED_CLAUSE_B_C_PARTIAL);
    REQUIRE_FALSE(irt.HasNoResults());
    REQUIRE(irt.GetValuesGivenDeclarations({a}) == LIST_A_OUTPUT);
    REQUIRE(irt.GetValuesGivenDeclarations({v}) == LIST_V_OUTPUT);
    REQUIRE(irt.GetValuesGivenDeclarations({c}) == LIST_C_PARTIAL_OUTPUT);
  }

  SECTION("Paired clauses that contradict should nullify all results") {
    irt.AddClauseResult(PAIRED_CLAUSE_A_B);
    irt.AddClauseResult(PAIRED_CLAUSE_V_S);
    irt.AddClauseResult(SINGLE_CLAUSE_C);
    irt.AddClauseResult(CONTRADICTING_CLAUSE_A_B);
    REQUIRE(irt.HasNoResults());
    REQUIRE(irt.GetValuesGivenDeclarations({a}).empty());
    REQUIRE(irt.GetValuesGivenDeclarations({c}).empty());
    REQUIRE(irt.GetValuesGivenDeclarations({v}).empty());
  }

  SECTION("Clauses have 1 synonym in common + there are results") {
    irt.AddClauseResult(SINGLE_CLAUSE_A);
    irt.AddClauseResult(PAIRED_CLAUSE_A_B);
    irt.AddClauseResult(PAIRED_CLAUSE_A_C);
    REQUIRE_FALSE(irt.HasNoResults());
    REQUIRE(irt.GetValuesGivenDeclarations({a}) == LIST_A_OUTPUT);
    REQUIRE(irt.GetValuesGivenDeclarations({b}) == LIST_B_OUTPUT);
    REQUIRE(irt.GetValuesGivenDeclarations({c}) == LIST_C_OUTPUT);
    // Test that values are linked properly
    irt.AddClauseResult(ClauseResult(c, LIST_C_PARTIAL));
    REQUIRE_FALSE(irt.HasNoResults());
    REQUIRE(irt.GetValuesGivenDeclarations({a}) == LIST_A_PARTIAL_OUTPUT);
    REQUIRE(irt.GetValuesGivenDeclarations({b}) == LIST_B_PARTIAL_OUTPUT);
    REQUIRE(irt.GetValuesGivenDeclarations({c}) == LIST_C_PARTIAL_OUTPUT);
  }

  SECTION("Clauses have 1 synonym in common + no results") {
    irt.AddClauseResult(SINGLE_CLAUSE_A);
    irt.AddClauseResult(PAIRED_CLAUSE_A_C);
    irt.AddClauseResult(CONTRADICTING_CLAUSE_A_B);
    REQUIRE(irt.HasNoResults());
    REQUIRE(irt.GetValuesGivenDeclarations({a}).empty());
    REQUIRE(irt.GetValuesGivenDeclarations({b}).empty());
    REQUIRE(irt.GetValuesGivenDeclarations({c}).empty());
  }

  SECTION("2 common synonyms") {
    irt.AddClauseResult(SINGLE_CLAUSE_A);
    irt.AddClauseResult(PAIRED_CLAUSE_A_B);
    irt.AddClauseResult(PAIRED_CLAUSE_A_B);
    REQUIRE_FALSE(irt.HasNoResults());
    REQUIRE(irt.GetValuesGivenDeclarations({a}) == LIST_A_OUTPUT);
    REQUIRE(irt.GetValuesGivenDeclarations({b}) == LIST_B_OUTPUT);
  }

  SECTION("Chained synonyms") {
    irt.AddClauseResult(SINGLE_CLAUSE_V);
    irt.AddClauseResult(SINGLE_CLAUSE_A);
    irt.AddClauseResult(PAIRED_CLAUSE_A_B);
    irt.AddClauseResult(PAIRED_CLAUSE_B_C);
    REQUIRE_FALSE(irt.HasNoResults());
    REQUIRE(irt.GetValuesGivenDeclarations({a}) == LIST_A_OUTPUT);
    REQUIRE(irt.GetValuesGivenDeclarations({b}) == LIST_B_OUTPUT);
    REQUIRE(irt.GetValuesGivenDeclarations({c}) == LIST_C_OUTPUT);

    // Test that values are linked properly
    irt.AddClauseResult(ClauseResult(v, LIST_V_PARTIAL));
    REQUIRE(irt.GetValuesGivenDeclarations({a}) == LIST_A_OUTPUT);
    REQUIRE(irt.GetValuesGivenDeclarations({c}) == LIST_C_OUTPUT);
    irt.AddClauseResult(ClauseResult(a, LIST_A_PARTIAL));
    REQUIRE(irt.GetValuesGivenDeclarations({a}) == LIST_A_PARTIAL_OUTPUT);
    REQUIRE(irt.GetValuesGivenDeclarations({b}) == LIST_B_PARTIAL_OUTPUT);
    REQUIRE(irt.GetValuesGivenDeclarations({c}) == LIST_C_PARTIAL_OUTPUT);
  }

  SECTION("Separate tables that get later merged") {
    irt.AddClauseResult(SINGLE_CLAUSE_A);
    irt.AddClauseResult(PAIRED_CLAUSE_B_C);
    irt.AddClauseResult(PAIRED_CLAUSE_A_B);
    REQUIRE_FALSE(irt.HasNoResults());
    REQUIRE(irt.GetValuesGivenDeclarations({a}) == LIST_A_OUTPUT);
    REQUIRE(irt.GetValuesGivenDeclarations({b}) == LIST_B_OUTPUT);
    REQUIRE(irt.GetValuesGivenDeclarations({c}) == LIST_C_OUTPUT);

    SECTION("Check linking on a") {
      irt.AddClauseResult(ClauseResult(a, LIST_A_PARTIAL));
      REQUIRE(irt.GetValuesGivenDeclarations({a}) == LIST_A_PARTIAL_OUTPUT);
      REQUIRE(irt.GetValuesGivenDeclarations({b}) == LIST_B_PARTIAL_OUTPUT);
      REQUIRE(irt.GetValuesGivenDeclarations({c}) == LIST_C_PARTIAL_OUTPUT);
    }

    SECTION("Check paired joining") {
      irt.AddClauseResult(PAIRED_CLAUSE_B_C_PARTIAL);
      REQUIRE(irt.GetValuesGivenDeclarations({a}) == LIST_A_PARTIAL_OUTPUT);
      REQUIRE(irt.GetValuesGivenDeclarations({b}) == LIST_B_PARTIAL_OUTPUT);
      REQUIRE(irt.GetValuesGivenDeclarations({c}) == LIST_C_PARTIAL_OUTPUT);
    }
  }

  SECTION("Check results retrieval - unlinked") {
    irt.AddClauseResult(SINGLE_CLAUSE_A);
    irt.AddClauseResult(SINGLE_CLAUSE_C);
    std::vector<std::vector<std::string>> expected_output = {{"1","20"}, {"1","30"}, {"1","40"},
                                                            {"2","20"}, {"2","30"}, {"2","40"},
                                                             {"3","20"}, {"3","30"}, {"3","40"}};
    REQUIRE_THAT(irt.GetValuesGivenDeclarations({a, c}), Catch::UnorderedEquals(expected_output));
  }

  SECTION("Check results retrieval - linked") {
    irt.AddClauseResult(PAIRED_CLAUSE_A_C);
    std::vector<std::vector<std::string>> expected_output = {{"1","20", "1", "20"},
                                                             {"2","30", "2", "30"},
                                                             {"3","40", "3", "40"}};
    REQUIRE_THAT(irt.GetValuesGivenDeclarations({a, c, a, c}), Catch::UnorderedEquals(expected_output));
  }

  SECTION("Check results retrieval - de-duplication") {
    irt.AddClauseResult(PAIRED_CLAUSE_A_B);
    irt.AddClauseResult(ClauseResult(
        a, c, IrtTestHelperMethods::makePairedVector({"2", "2", "2"}, LIST_C)));
    REQUIRE_THAT(irt.GetValuesGivenDeclarations({a}), Catch::UnorderedEquals(LIST_A_PARTIAL_OUTPUT));
  }
}
