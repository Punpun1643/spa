#include "../../../../spa/src/query_processing_system/clauses/ClauseResult.h"
#include "catch.hpp"
#include "query_processing_system/evaluator/IntermediateResultsTable.h"
#include "shared/ArrayUtility.h"

// ASSUMPTIONS: Tests for RelationalTable all pass.

namespace IrtTestHelperMethods {
std::vector<std::pair<std::string, std::string>> MakePairedVector(
    std::vector<std::string> v1, std::vector<std::string> v2) {
  assert(v1.size() == v2.size());
  auto list_of_pairs = std::vector<std::pair<std::string, std::string>>();
  for (auto i = 0; i < v1.size(); i++) {
    list_of_pairs.emplace_back(v1[i], v2[i]);
  }
  return list_of_pairs;
}

std::vector<std::vector<std::string>> AddDim(std::vector<std::string>& vec) {
  std::vector<std::vector<std::string>> output = {};
  for (auto const& value : vec) {
    output.push_back({value});
  }
  return output;
}

std::vector<std::vector<std::string>> MergeVectorsElementWise(
    std::vector<std::vector<std::string>> input_vectors) {
  // assume input vectors have same length
  for (int i = 0; i < input_vectors.size() - 1; i++) {
    assert(input_vectors[i].size() == input_vectors[i + 1].size());
  }
  std::vector<std::vector<std::string>> output;
  for (auto i = 0; i < input_vectors[0].size(); i++) {
    std::vector<std::string> new_row;
    for (auto& v : input_vectors) {
      new_row.push_back(v[i]);
    }
    output.push_back(new_row);
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

  auto LIST_A_OUTPUT = IrtTestHelperMethods::AddDim(LIST_A);
  auto LIST_A_PARTIAL_OUTPUT = IrtTestHelperMethods::AddDim(LIST_A_PARTIAL);
  auto LIST_B_OUTPUT = IrtTestHelperMethods::AddDim(LIST_B);
  auto LIST_B_PARTIAL_OUTPUT = IrtTestHelperMethods::AddDim(LIST_B_PARTIAL);
  auto LIST_C_OUTPUT = IrtTestHelperMethods::AddDim(LIST_C);
  auto LIST_C_PARTIAL_OUTPUT = IrtTestHelperMethods::AddDim(LIST_C_PARTIAL);
  auto LIST_V_OUTPUT = IrtTestHelperMethods::AddDim(LIST_V);
  auto LIST_V_PARTIAL_OUTPUT = IrtTestHelperMethods::AddDim(LIST_V_PARTIAL);
  auto LIST_S_OUTPUT = IrtTestHelperMethods::AddDim(LIST_S);

  auto SINGLE_CLAUSE_A = ClauseResult(a, LIST_A);
  auto SINGLE_CLAUSE_B = ClauseResult(b, LIST_B);
  auto SINGLE_CLAUSE_C = ClauseResult(c, LIST_C);
  auto SINGLE_CLAUSE_V = ClauseResult(v, LIST_V);
  auto CONTRADICTING_CLAUSE_A = ClauseResult(a, LIST_C);
  auto PAIRED_CLAUSE_A_B = ClauseResult(
      a, b, IrtTestHelperMethods::MakePairedVector(LIST_A, LIST_B));
  auto PAIRED_CLAUSE_A_C = ClauseResult(
      a, c, IrtTestHelperMethods::MakePairedVector(LIST_A, LIST_C));
  auto PAIRED_CLAUSE_B_C = ClauseResult(
      b, c, IrtTestHelperMethods::MakePairedVector(LIST_B, LIST_C));
  auto PAIRED_CLAUSE_B_C_PARTIAL = ClauseResult(
      b, c, IrtTestHelperMethods::MakePairedVector({LIST_B[1]}, {LIST_C[1]}));
  auto PAIRED_CLAUSE_V_S = ClauseResult(
      v, s, IrtTestHelperMethods::MakePairedVector(LIST_V, LIST_S));
  auto CONTRADICTING_CLAUSE_A_B = ClauseResult(
      a, b, IrtTestHelperMethods::MakePairedVector(LIST_V, LIST_V));

  auto irt = IntermediateResultsTable();

  // BOOLEAN TESTS
  SECTION("Test only boolean clauses") {
    irt.AddClauseResult(TRUE_CLAUSE, false);
    irt.AddClauseResult(TRUE_CLAUSE, false);
    irt.AddClauseResult(FALSE_CLAUSE, false);
    REQUIRE(irt.HasNoResults());
    irt.AddClauseResult(FALSE_CLAUSE, false);
    REQUIRE(irt.HasNoResults());
  }

  SECTION("Test negated boolean clauses") {
    irt.AddClauseResult(TRUE_CLAUSE, false);
    irt.AddClauseResult(FALSE_CLAUSE, true);
    REQUIRE_FALSE(irt.HasNoResults());

    irt.AddClauseResult(TRUE_CLAUSE, true);
    REQUIRE(irt.HasNoResults());
  }

  SECTION("Test boolean + single clause") {
    irt.AddClauseResult(SINGLE_CLAUSE_A, false);
    REQUIRE_FALSE(irt.HasNoResults());
    irt.AddClauseResult(FALSE_CLAUSE, true);
    REQUIRE_FALSE(irt.HasNoResults());
    REQUIRE(irt.GetValuesGivenDeclarations({a}) == LIST_A_OUTPUT);
    irt.AddClauseResult(FALSE_CLAUSE, false);
    REQUIRE(irt.HasNoResults());
    irt.AddClauseResult(SINGLE_CLAUSE_A, false);
    REQUIRE(irt.HasNoResults());
  }

  SECTION("Test boolean + paired clause") {
    irt.AddClauseResult(PAIRED_CLAUSE_A_B, false);
    REQUIRE_FALSE(irt.HasNoResults());
    irt.AddClauseResult(TRUE_CLAUSE, false);
    REQUIRE_FALSE(irt.HasNoResults());
    REQUIRE(irt.GetValuesGivenDeclarations({a, b}) ==
            IrtTestHelperMethods::MergeVectorsElementWise({LIST_A, LIST_B}));
    irt.AddClauseResult(TRUE_CLAUSE, true);
    REQUIRE(irt.HasNoResults());
    irt.AddClauseResult(PAIRED_CLAUSE_A_B, false);
    REQUIRE(irt.HasNoResults());
  }

  // SINGLE RESULT CLAUSE TESTS
  SECTION("Test single result clauses") {
    irt.AddClauseResult(SINGLE_CLAUSE_B, false);
    irt.AddClauseResult(SINGLE_CLAUSE_A, false);
    REQUIRE(irt.GetValuesGivenDeclarations({a}) == LIST_A_OUTPUT);
    REQUIRE(irt.GetValuesGivenDeclarations({b}) == LIST_B_OUTPUT);
    // changes to A should not affect B
    auto clause_result = ClauseResult(a, LIST_A_PARTIAL);
    irt.AddClauseResult(clause_result, false);
    REQUIRE(irt.GetValuesGivenDeclarations({a}) == LIST_A_PARTIAL_OUTPUT);
    REQUIRE(irt.GetValuesGivenDeclarations({b}) == LIST_B_OUTPUT);
    // adding in a third unrelated clause should not affect the first two
    irt.AddClauseResult(SINGLE_CLAUSE_V, false);
    REQUIRE_FALSE(irt.HasNoResults());
    REQUIRE(irt.GetValuesGivenDeclarations({v}) == LIST_V_OUTPUT);
    REQUIRE(irt.GetValuesGivenDeclarations({a}) == LIST_A_PARTIAL_OUTPUT);
    REQUIRE(irt.GetValuesGivenDeclarations({b}) == LIST_B_OUTPUT);
  }

  SECTION("Single result clauses that contradict") {
    irt.AddClauseResult(SINGLE_CLAUSE_B, false);
    irt.AddClauseResult(SINGLE_CLAUSE_A, false);
    irt.AddClauseResult(CONTRADICTING_CLAUSE_A, false);
    REQUIRE(irt.HasNoResults());
    REQUIRE(irt.GetValuesGivenDeclarations({a}).empty());
    REQUIRE(irt.GetValuesGivenDeclarations({b}).empty());
    // adding in a new clause doesn't change things
    irt.AddClauseResult(SINGLE_CLAUSE_V, false);
    REQUIRE(irt.GetValuesGivenDeclarations({v}).empty());
  }

  SECTION("Test negated single declaration clauses") {
    irt.AddClauseResult(PAIRED_CLAUSE_A_B, false);
    auto clause_result = ClauseResult(a, {"1", "3", "4"});
    irt.AddClauseResult(clause_result, true);
    REQUIRE(irt.GetValuesGivenDeclarations({b}) == LIST_B_PARTIAL_OUTPUT);

    irt.AddClauseResult(SINGLE_CLAUSE_B, true);
    REQUIRE(irt.HasNoResults());
    REQUIRE(irt.GetValuesGivenDeclarations({b}).empty());
  }

  // Full tests with at least 1 x single and 2 x paired clauses (for milestone
  // 1)
  SECTION("All synonyms different") {
    irt.AddClauseResult(SINGLE_CLAUSE_A, false);
    irt.AddClauseResult(PAIRED_CLAUSE_B_C, false);
    irt.AddClauseResult(PAIRED_CLAUSE_V_S, false);
    REQUIRE_FALSE(irt.HasNoResults());
    REQUIRE(irt.GetValuesGivenDeclarations({a}) == LIST_A_OUTPUT);
    REQUIRE(irt.GetValuesGivenDeclarations({c}) == LIST_C_OUTPUT);
    REQUIRE(irt.GetValuesGivenDeclarations({v}) == LIST_V_OUTPUT);
    // Check that none of the clauses are linked
    irt.AddClauseResult(PAIRED_CLAUSE_B_C_PARTIAL, false);
    REQUIRE_FALSE(irt.HasNoResults());
    REQUIRE(irt.GetValuesGivenDeclarations({a}) == LIST_A_OUTPUT);
    REQUIRE(irt.GetValuesGivenDeclarations({v}) == LIST_V_OUTPUT);
    REQUIRE(irt.GetValuesGivenDeclarations({c}) == LIST_C_PARTIAL_OUTPUT);
  }

  SECTION("Paired clauses that contradict should nullify all results") {
    irt.AddClauseResult(PAIRED_CLAUSE_A_B, false);
    irt.AddClauseResult(PAIRED_CLAUSE_V_S, false);
    irt.AddClauseResult(SINGLE_CLAUSE_C, false);
    irt.AddClauseResult(CONTRADICTING_CLAUSE_A_B, false);
    REQUIRE(irt.HasNoResults());
    REQUIRE(irt.GetValuesGivenDeclarations({a}).empty());
    REQUIRE(irt.GetValuesGivenDeclarations({c}).empty());
    REQUIRE(irt.GetValuesGivenDeclarations({v}).empty());
  }

  SECTION("Clauses have 1 synonym in common + there are results") {
    irt.AddClauseResult(SINGLE_CLAUSE_A, false);
    irt.AddClauseResult(PAIRED_CLAUSE_A_B, false);
    irt.AddClauseResult(PAIRED_CLAUSE_A_C, false);
    REQUIRE_FALSE(irt.HasNoResults());
    REQUIRE(irt.GetValuesGivenDeclarations({a, b, c}) ==
            IrtTestHelperMethods::MergeVectorsElementWise(
                {LIST_A, LIST_B, LIST_C}));
    // Test that values are linked properly
    auto clause_result = ClauseResult(c, LIST_C_PARTIAL);
    irt.AddClauseResult(clause_result, false);
    REQUIRE_FALSE(irt.HasNoResults());
    REQUIRE(irt.GetValuesGivenDeclarations({a, b, c}) ==
            IrtTestHelperMethods::MergeVectorsElementWise(
                {LIST_A_PARTIAL, LIST_B_PARTIAL, LIST_C_PARTIAL}));
  }

  SECTION("Clauses have 1 synonym in common + no results") {
    irt.AddClauseResult(SINGLE_CLAUSE_A, false);
    irt.AddClauseResult(PAIRED_CLAUSE_A_C, false);
    irt.AddClauseResult(CONTRADICTING_CLAUSE_A_B, false);
    REQUIRE(irt.HasNoResults());
    REQUIRE(irt.GetValuesGivenDeclarations({a}).empty());
    REQUIRE(irt.GetValuesGivenDeclarations({b}).empty());
    REQUIRE(irt.GetValuesGivenDeclarations({c}).empty());
  }

  SECTION("2 common synonyms") {
    irt.AddClauseResult(SINGLE_CLAUSE_A, false);
    irt.AddClauseResult(PAIRED_CLAUSE_A_B, false);
    irt.AddClauseResult(PAIRED_CLAUSE_A_B, false);
    REQUIRE_FALSE(irt.HasNoResults());
    REQUIRE(irt.GetValuesGivenDeclarations({a, b}) ==
            IrtTestHelperMethods::MergeVectorsElementWise({LIST_A, LIST_B}));
  }

  SECTION("Chained synonyms") {
    irt.AddClauseResult(SINGLE_CLAUSE_V, false);
    irt.AddClauseResult(SINGLE_CLAUSE_A, false);
    irt.AddClauseResult(PAIRED_CLAUSE_A_B, false);
    irt.AddClauseResult(PAIRED_CLAUSE_B_C, false);
    REQUIRE_FALSE(irt.HasNoResults());
    REQUIRE(irt.GetValuesGivenDeclarations({a, b, c}) ==
            IrtTestHelperMethods::MergeVectorsElementWise(
                {LIST_A, LIST_B, LIST_C}));

    // Test that values are linked properly
    auto clause_result = ClauseResult(v, LIST_V_PARTIAL);
    irt.AddClauseResult(clause_result, false);
    REQUIRE(irt.GetValuesGivenDeclarations({a, b, c}) ==
            IrtTestHelperMethods::MergeVectorsElementWise(
                {LIST_A, LIST_B, LIST_C}));
    clause_result = ClauseResult(a, LIST_A_PARTIAL);
    irt.AddClauseResult(clause_result, false);
    REQUIRE(irt.GetValuesGivenDeclarations({a, b, c}) ==
            IrtTestHelperMethods::MergeVectorsElementWise(
                {LIST_A_PARTIAL, LIST_B_PARTIAL, LIST_C_PARTIAL}));
  }

  SECTION("Separate tables that get later merged") {
    irt.AddClauseResult(SINGLE_CLAUSE_A, false);
    irt.AddClauseResult(PAIRED_CLAUSE_B_C, false);
    irt.AddClauseResult(PAIRED_CLAUSE_A_B, false);
    REQUIRE_FALSE(irt.HasNoResults());
    REQUIRE(irt.GetValuesGivenDeclarations({a, b, c}) ==
            IrtTestHelperMethods::MergeVectorsElementWise(
                {LIST_A, LIST_B, LIST_C}));

    SECTION("Check linking on a") {
      auto clause_result = ClauseResult(a, LIST_A_PARTIAL);
      irt.AddClauseResult(clause_result, false);
      REQUIRE(irt.GetValuesGivenDeclarations({a, b, c}) ==
              IrtTestHelperMethods::MergeVectorsElementWise(
                  {LIST_A_PARTIAL, LIST_B_PARTIAL, LIST_C_PARTIAL}));
    }

    SECTION("Check paired joining") {
      irt.AddClauseResult(PAIRED_CLAUSE_B_C_PARTIAL, false);
      REQUIRE(irt.GetValuesGivenDeclarations({a, b, c}) ==
              IrtTestHelperMethods::MergeVectorsElementWise(
                  {LIST_A_PARTIAL, LIST_B_PARTIAL, LIST_C_PARTIAL}));
    }
  }

  SECTION("Test negated clauses with two synonyms - same table") {
    irt.AddClauseResult(PAIRED_CLAUSE_A_B, false);
    irt.AddClauseResult(PAIRED_CLAUSE_B_C, false);
    auto CLAUSE_B_C_PARTIAL_INVERSE =
        ClauseResult(b, c,
                     {std::make_pair("2", "20"), std::make_pair("4", "40"),
                      std::make_pair("30", "3")});

    irt.AddClauseResult(CLAUSE_B_C_PARTIAL_INVERSE, true);
    REQUIRE_FALSE(irt.HasNoResults());
    REQUIRE(irt.GetValuesGivenDeclarations({a, b, c}) ==
            IrtTestHelperMethods::MergeVectorsElementWise(
                {LIST_A_PARTIAL, LIST_B_PARTIAL, LIST_C_PARTIAL}));

    irt.AddClauseResult(PAIRED_CLAUSE_A_C, true);
    REQUIRE(irt.HasNoResults());
  }

  SECTION("Test negated clauses with two synonyms - diff table") {
    irt.AddClauseResult(SINGLE_CLAUSE_V, false);
    auto clause_result = ClauseResult(a, {"1", "2"});
    irt.AddClauseResult(clause_result, false);
    clause_result = ClauseResult(b, c, {std::make_pair("20", "30")});
    irt.AddClauseResult(clause_result, false);

    auto CLAUSE_A_B_PARTIAL_INVERSE =
        ClauseResult(a, b,
                     {std::make_pair("1", "20"), std::make_pair("20", "2"),
                      std::make_pair("1", "21")});

    irt.AddClauseResult(CLAUSE_A_B_PARTIAL_INVERSE, true);
    REQUIRE_FALSE(irt.HasNoResults());
    std::vector<std::string> OUTPUT_A = {"2"};
    std::vector<std::string> OUTPUT_C = {"30"};

    REQUIRE(
        irt.GetValuesGivenDeclarations({a, c}) ==
        IrtTestHelperMethods::MergeVectorsElementWise({OUTPUT_A, OUTPUT_C}));

    irt.AddClauseResult(PAIRED_CLAUSE_A_C, true);
    REQUIRE(irt.HasNoResults());
  }

  SECTION("Check results retrieval - unlinked") {
    irt.AddClauseResult(SINGLE_CLAUSE_A, false);
    irt.AddClauseResult(SINGLE_CLAUSE_C, false);
    std::vector<std::vector<std::string>> expected_output = {
        {"1", "20"}, {"1", "30"}, {"1", "40"}, {"2", "20"}, {"2", "30"},
        {"2", "40"}, {"3", "20"}, {"3", "30"}, {"3", "40"}};
    REQUIRE_THAT(irt.GetValuesGivenDeclarations({a, c}),
                 Catch::UnorderedEquals(expected_output));
  }

  SECTION("Check results retrieval - linked") {
    irt.AddClauseResult(PAIRED_CLAUSE_A_C, false);
    std::vector<std::vector<std::string>> expected_output = {
        {"1", "20", "1", "20"}, {"2", "30", "2", "30"}, {"3", "40", "3", "40"}};
    REQUIRE_THAT(irt.GetValuesGivenDeclarations({a, c, a, c}),
                 Catch::UnorderedEquals(expected_output));
  }

  SECTION("Check results retrieval - de-duplication") {
    irt.AddClauseResult(PAIRED_CLAUSE_A_B, false);
    auto clause_result = ClauseResult(
        a, c, IrtTestHelperMethods::MakePairedVector({"2", "2", "2"}, LIST_C));
    irt.AddClauseResult(clause_result, false);
    REQUIRE_THAT(irt.GetValuesGivenDeclarations({a}),
                 Catch::UnorderedEquals(LIST_A_PARTIAL_OUTPUT));
  }
}
