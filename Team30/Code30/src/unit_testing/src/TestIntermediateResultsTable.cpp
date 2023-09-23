#include "catch.hpp"
#include "query_processing_system/evaluator/IntermediateResultsTable.h"
#include "../../spa/src/query_processing_system/common/ClauseResult.h"

namespace IrtTestHelperMethods {
std::vector<std::pair<std::string, std::string>> makePairedVector(std::vector<std::string> v1,
                                                                  std::vector<std::string> v2) {
  assert(v1.size() == v2.size());
  auto list_of_pairs = std::vector<std::pair<std::string, std::string>>();
  for (auto i = 0; i < v1.size(); i++) {
    list_of_pairs.push_back(std::make_pair(v1[i], v2[i]));
  }
  return list_of_pairs;
}
}

TEST_CASE("Intermediate Results Table Tests") {
  // Declarations
  auto a = PqlDeclaration(std::make_shared<std::string>("a"), EntityType::ASSIGN);
  auto b = PqlDeclaration(std::make_shared<std::string>("b"), EntityType::PRINT);
  auto c = PqlDeclaration(std::make_shared<std::string>("constant"), EntityType::CONSTANT);
  auto v = PqlDeclaration(std::make_shared<std::string>("variable"), EntityType::VARIABLE);
  auto s = PqlDeclaration(std::make_shared<std::string>("stmt"), EntityType::STMT);

  auto TRUE_CLAUSE = ClauseResult(true);
  auto FALSE_CLAUSE = ClauseResult(false);

  auto LIST_A = std::vector<std::string>({"1","2","3"});
  auto LIST_B = std::vector<std::string>({"2","3","4"});
  auto LIST_C = std::vector<std::string>({"20","30","40"});
  auto LIST_V = std::vector<std::string>({"a","b","c"});
  auto LIST_S = std::vector<std::string>({"5","6","7"});

  auto SINGLE_CLAUSE_A = ClauseResult(a, LIST_A);
  auto SINGLE_CLAUSE_B = ClauseResult(b, LIST_B);
  auto SINGLE_CLAUSE_V = ClauseResult(v, LIST_V);
  auto PAIRED_CLAUSE_A_B = ClauseResult(a, b, IrtTestHelperMethods::makePairedVector(LIST_A, LIST_B));
  auto PAIRED_CLAUSE_A_C = ClauseResult(a, c, IrtTestHelperMethods::makePairedVector(LIST_A, LIST_C));
  auto PAIRED_CLAUSE_B_C = ClauseResult(b, c, IrtTestHelperMethods::makePairedVector(LIST_B, LIST_C));
  auto PAIRED_CLAUSE_V_S = ClauseResult(v, s, IrtTestHelperMethods::makePairedVector(LIST_V, LIST_S));

  auto irt = IntermediateResultsTable();

  SECTION("Test only boolean clauses") {
    irt.addClauseResult(TRUE_CLAUSE);
    irt.addClauseResult(TRUE_CLAUSE);
    irt.addClauseResult(FALSE_CLAUSE);
    REQUIRE(irt.hasNoResults());
    irt.addClauseResult(FALSE_CLAUSE);
    REQUIRE(irt.hasNoResults());
  }

  SECTION("Test boolean + single clause") {
    irt.addClauseResult(SINGLE_CLAUSE_A);
    REQUIRE_FALSE(irt.hasNoResults());
    irt.addClauseResult(TRUE_CLAUSE);
    REQUIRE_FALSE(irt.hasNoResults());
    REQUIRE(irt.getValuesGivenDeclaration(a) == LIST_A);
    irt.addClauseResult(FALSE_CLAUSE);
    REQUIRE(irt.hasNoResults());
    irt.addClauseResult(SINGLE_CLAUSE_A);
    REQUIRE(irt.hasNoResults());
  }

  SECTION("Test boolean + paired clause") {
    irt.addClauseResult(PAIRED_CLAUSE_A_B);
    REQUIRE_FALSE(irt.hasNoResults());
    irt.addClauseResult(TRUE_CLAUSE);
    REQUIRE_FALSE(irt.hasNoResults());
    REQUIRE(irt.getValuesGivenDeclaration(a) == LIST_A);
    REQUIRE(irt.getValuesGivenDeclaration(b) == LIST_B);
    irt.addClauseResult(FALSE_CLAUSE);
    REQUIRE(irt.hasNoResults());
    irt.addClauseResult(PAIRED_CLAUSE_A_B);
    REQUIRE(irt.hasNoResults());
  }

  SECTION("Test single result clauses") {
    irt.addClauseResult(SINGLE_CLAUSE_B);
    irt.addClauseResult(SINGLE_CLAUSE_A);
    REQUIRE(irt.getValuesGivenDeclaration(a) == LIST_A);
    REQUIRE(irt.getValuesGivenDeclaration(b) == LIST_B);
    // a will have no more values
    irt.addClauseResult(ClauseResult(a, LIST_V));
    REQUIRE(irt.getValuesGivenDeclaration(a).empty());
    REQUIRE(irt.hasNoResults());
    irt.addClauseResult(SINGLE_CLAUSE_V);
    REQUIRE(irt.getValuesGivenDeclaration(v).empty());
  }

  // Full tests with 1 x single and 2 x doubles (for milestone 1)
  SECTION("All synonyms different") {
    irt.addClauseResult(SINGLE_CLAUSE_A);
    irt.addClauseResult(PAIRED_CLAUSE_B_C);
    irt.addClauseResult(PAIRED_CLAUSE_V_S);
    REQUIRE_FALSE(irt.hasNoResults());
    REQUIRE(irt.getValuesGivenDeclaration(a) == LIST_A);
    REQUIRE(irt.getValuesGivenDeclaration(c) == LIST_C);
    REQUIRE(irt.getValuesGivenDeclaration(v) == LIST_V);
  }

  SECTION("Clauses have 1 synonym in common + there are results") {
    irt.addClauseResult(SINGLE_CLAUSE_A);
    irt.addClauseResult(PAIRED_CLAUSE_A_B);
    irt.addClauseResult(PAIRED_CLAUSE_A_C);
    REQUIRE_FALSE(irt.hasNoResults());
    REQUIRE(irt.getValuesGivenDeclaration(a) == LIST_A);
    REQUIRE(irt.getValuesGivenDeclaration(b) == LIST_B);
    REQUIRE(irt.getValuesGivenDeclaration(c) == LIST_C);
  }

  SECTION("Clauses have 1 synonym in common + no results") {
    irt.addClauseResult(SINGLE_CLAUSE_A);
    irt.addClauseResult(PAIRED_CLAUSE_A_C);
    auto CONTRADICTING_CLAUSE = ClauseResult(a, b, IrtTestHelperMethods::makePairedVector(LIST_V, LIST_V));
    irt.addClauseResult(CONTRADICTING_CLAUSE);
    REQUIRE(irt.hasNoResults());
    REQUIRE(irt.getValuesGivenDeclaration(a).empty());
    REQUIRE(irt.getValuesGivenDeclaration(b).empty());
    REQUIRE(irt.getValuesGivenDeclaration(c).empty());
  }

  SECTION("2 common synonyms") {

  }
    // 2 common synonyms (A, (A B), (A B))

    // chained synonyms (A, (A B), (B C))

    // (A, (B C), (A, B) - separate tables later get merged

}