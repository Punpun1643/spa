#include "../../stub/PkbQpsInterfaceStub.h"
#include "catch.hpp"
#include "query_processing_system/clauses/PatternAssignClause.h"
#include "query_processing_system/clauses/PatternWhileClause.h"
#include "query_processing_system/clauses/PatternIfClause.h"
#include "query_processing_system/exceptions/InvalidSemanticsException.h"

TEST_CASE("Pattern Assign Clauses") {
  PkbQpsInterfaceStub pkb = PkbQpsInterfaceStub();
  // decl, partial
  auto a = PqlDeclaration("a", EntityType::ASSIGN);
  auto v = PqlDeclaration("v", EntityType::VARIABLE);

  // create a simple tree x + y
  auto baseTreeLeft = std::make_shared<TreeNode>("x", nullptr, nullptr);
  auto baseTreeRight = std::make_shared<TreeNode>("y", nullptr, nullptr);
  auto rhs_expr = std::make_shared<TreeNode>("+", baseTreeLeft, baseTreeRight);
  auto empty_rhs_expr = std::make_shared<TreeNode>("", nullptr, nullptr);

  SECTION("decl, wild") {
    auto pattern_clause =
        PatternAssignClause(a, EntRef(v), MatchType::WILD_MATCH, rhs_expr);
    auto result = pattern_clause.Evaluate(pkb);
    REQUIRE(pkb.last_match_type_passed == MatchType::WILD_MATCH);
    REQUIRE(TreeNode::IsSameTree(pkb.last_rhs_expr_passed, rhs_expr));
    REQUIRE(pkb.patternAssignDeclCalls == 1);
    auto values = result->GetValues(a);
    REQUIRE(values == pkb.patternAssignDeclValues1);
    values = result->GetValues(v);
    REQUIRE(values == pkb.patternAssignDeclValues2);
  }

  SECTION("value, partial") {
    auto pattern_clause = PatternAssignClause(a, EntRef("varName"), MatchType::PARTIAL_MATCH, rhs_expr);
    auto result = pattern_clause.Evaluate(pkb);
    REQUIRE(pkb.last_match_type_passed == MatchType::PARTIAL_MATCH);
    REQUIRE(TreeNode::IsSameTree(pkb.last_rhs_expr_passed, rhs_expr));
    REQUIRE(pkb.last_value_passed == "varName");
    REQUIRE(pkb.patternAssignValueCalls == 1);
    auto values = result->GetValues(a);
    REQUIRE(values == pkb.patternAssignValueValues);
  }

  SECTION("wild, exact") {
    auto pattern_clause = PatternAssignClause(a, EntRef(), MatchType::EXACT_MATCH, empty_rhs_expr);
    auto result = pattern_clause.Evaluate(pkb);
    REQUIRE(pkb.last_match_type_passed == MatchType::EXACT_MATCH);
    REQUIRE(TreeNode::IsSameTree(pkb.last_rhs_expr_passed, empty_rhs_expr));
    REQUIRE(pkb.patternAssignWildCalls == 1);
    auto values = result->GetValues(a);
    REQUIRE(values == pkb.patternAssignWildValues);
  }

  SECTION("semantic errors") {
    REQUIRE_THROWS_AS(std::make_shared<PatternAssignClause>(
                          a, EntRef(a), MatchType::PARTIAL_MATCH, rhs_expr),
                      InvalidSemanticsException);
    REQUIRE_THROWS_AS(
        std::make_shared<PatternAssignClause>(v, EntRef(v), MatchType::PARTIAL_MATCH,
                                        empty_rhs_expr),
        InvalidSemanticsException);
  }
}

TEST_CASE("Pattern While/If Clauses") {
  PkbQpsInterfaceStub pkb = PkbQpsInterfaceStub();
  // decl, partial
  auto w = PqlDeclaration("while", EntityType::WHILE);
  auto i = PqlDeclaration("if", EntityType::IF);
  auto v = PqlDeclaration("v", EntityType::VARIABLE);
  auto s = PqlDeclaration("s", EntityType::STMT);

  SECTION("decl ent_ref") {
    auto pattern_while = PatternWhileClause(w, EntRef(v));
    auto result = pattern_while.Evaluate(pkb);
    REQUIRE(pkb.last_entity_type_passed == EntityType::WHILE);
    REQUIRE(result->GetNumDeclarations() == 2);
    REQUIRE(result->GetValues(w) == pkb.patternContainerDeclValues1);
    REQUIRE(result->GetValues(v) == pkb.patternContainerDeclValues2);

    auto pattern_if = PatternIfClause(i, EntRef(v));
    result = pattern_if.Evaluate(pkb);
    REQUIRE(pkb.last_entity_type_passed == EntityType::IF);
    REQUIRE(result->GetNumDeclarations() == 2);
    REQUIRE(result->GetValues(i) == pkb.patternContainerDeclValues1);
    REQUIRE(result->GetValues(v) == pkb.patternContainerDeclValues2);
  }

  SECTION("value entRef") {
    auto pattern_while = PatternWhileClause(w, EntRef("varX"));
    auto result = pattern_while.Evaluate(pkb);
    REQUIRE(pkb.last_entity_type_passed == EntityType::WHILE);
    REQUIRE(pkb.last_value_passed == "varX");
    REQUIRE(result->GetNumDeclarations() == 1);
    REQUIRE(result->GetValues(w) == pkb.patternContainerValueValues);

    auto pattern_if = PatternIfClause(i, EntRef("varY"));
    result = pattern_if.Evaluate(pkb);
    REQUIRE(pkb.last_entity_type_passed == EntityType::IF);
    REQUIRE(pkb.last_value_passed == "varY");
    REQUIRE(result->GetNumDeclarations() == 1);
    REQUIRE(result->GetValues(i) == pkb.patternContainerValueValues);
  }

  SECTION("wild entRef") {
    auto pattern_while = PatternWhileClause(w, EntRef());
    auto result = pattern_while.Evaluate(pkb);
    REQUIRE(pkb.last_entity_type_passed == EntityType::WHILE);
    REQUIRE(result->GetNumDeclarations() == 1);
    REQUIRE(result->GetValues(w) == pkb.patternContainerWildValues);

    auto pattern_if = PatternIfClause(i, EntRef());
    result = pattern_if.Evaluate(pkb);
    REQUIRE(pkb.last_entity_type_passed == EntityType::IF);
    REQUIRE(result->GetNumDeclarations() == 1);
    REQUIRE(result->GetValues(i) == pkb.patternContainerWildValues);
  }

  SECTION("semantic errors") {
    // Invalid declaration type
    REQUIRE_THROWS_AS(PatternWhileClause(i, EntRef()), InvalidSemanticsException);
    REQUIRE_THROWS_AS(PatternWhileClause(v, EntRef("abc")), InvalidSemanticsException);
    REQUIRE_THROWS_AS(PatternIfClause(s, EntRef(v)), InvalidSemanticsException);
    REQUIRE_THROWS_AS(PatternIfClause(w, EntRef()), InvalidSemanticsException);

    // Invalid ent_ref declaration type
    REQUIRE_THROWS_AS(PatternWhileClause(i, EntRef(s)), InvalidSemanticsException);
    REQUIRE_THROWS_AS(PatternIfClause(i, EntRef(w)), InvalidSemanticsException);
  }
}
