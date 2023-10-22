#include "query_processing_system/clauses/PatternClause.h"
#include "../../stub/PkbQpsInterfaceStub.h"
#include "query_processing_system/exceptions/InvalidSemanticsException.h"
#include "catch.hpp"


TEST_CASE("Pattern clauses") {
  PkbQpsInterfaceStub pkb = PkbQpsInterfaceStub();
  // decl, partial
  auto a = PqlDeclaration("a", EntityType::ASSIGN);
  auto v = PqlDeclaration("v", EntityType::VARIABLE);

  // create a simple tree x + y
  auto baseTreeLeft =
      std::make_shared<TreeNode>("x", nullptr, nullptr);
  auto baseTreeRight =
      std::make_shared<TreeNode>("y", nullptr, nullptr);
  auto rhs_expr =
      std::make_shared<TreeNode>("+", baseTreeLeft, baseTreeRight);
  auto empty_rhs_expr =
      std::make_shared<TreeNode>("", nullptr, nullptr);

  SECTION("decl, wild") {
    auto pattern_clause = PatternClause(
        a, EntRef(v), MatchType::WILD_MATCH, rhs_expr);
    auto result = pattern_clause.Evaluate(pkb);
    REQUIRE(pkb.last_match_type_passed == MatchType::WILD_MATCH);
    REQUIRE(TreeNode::IsSameTree(pkb.last_rhs_expr_passed, rhs_expr));
    REQUIRE(pkb.patternDeclCalls == 1);
    auto values = result->GetValues(a);
    REQUIRE(values == pkb.patternDeclValues1);
    values = result->GetValues(v);
    REQUIRE(values == pkb.patternDeclValues2);
  }

  SECTION("value, partial") {
    auto pattern_clause = PatternClause(
        a, EntRef("varName"), MatchType::PARTIAL_MATCH, rhs_expr);
    auto result = pattern_clause.Evaluate(pkb);
    REQUIRE(pkb.last_match_type_passed == MatchType::PARTIAL_MATCH);
    REQUIRE(TreeNode::IsSameTree(pkb.last_rhs_expr_passed, rhs_expr));
    REQUIRE(pkb.last_value_passed == "varName");
    REQUIRE(pkb.patternValueCalls == 1);
    auto values = result->GetValues(a);
    REQUIRE(values == pkb.patternValueValues);
  }

  SECTION("wild, exact") {
    auto pattern_clause = PatternClause(
        a, EntRef(), MatchType::EXACT_MATCH, empty_rhs_expr);
    auto result = pattern_clause.Evaluate(pkb);
    REQUIRE(pkb.last_match_type_passed == MatchType::EXACT_MATCH);
    REQUIRE(TreeNode::IsSameTree(pkb.last_rhs_expr_passed, empty_rhs_expr));
    REQUIRE(pkb.patternWildCalls == 1);
    auto values = result->GetValues(a);
    REQUIRE(values == pkb.patternWildValues);
  }

  SECTION("semantic errors") {
    REQUIRE_THROWS_AS(std::make_shared<PatternClause>(
                          a, EntRef(a), MatchType::PARTIAL_MATCH, rhs_expr),
                      InvalidSemanticsException);
    REQUIRE_THROWS_AS(std::make_shared<PatternClause>(
                          v, EntRef(v), MatchType::PARTIAL_MATCH, empty_rhs_expr),
                      InvalidSemanticsException);
  }
}
