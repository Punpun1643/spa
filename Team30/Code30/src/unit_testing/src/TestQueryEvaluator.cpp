#include <memory>

#include "../../spa/src/program_knowledge_base/PKBQPSInterface.h"
#include "../../spa/src/query_processing_system/common/FollowsClause.h"
#include "../../spa/src/query_processing_system/common/SelectClause.h"
#include "../../spa/src/query_processing_system/evaluator/QueryEvaluator.h"
#include "../../spa/src/query_processing_system/evaluator/ArrayUtility.h"
#include "../../spa/src/query_processing_system/exceptions/InvalidSemanticsException.h"
#include "PkbStub.h"
#include "catch.hpp"

class QeFactoryMethods {
 public:
  static std::shared_ptr<FollowsClause> getFollowsClause(StmtRef ref1,
                                                         StmtRef ref2) {
    return std::make_shared<FollowsClause>(std::make_unique<StmtRef>(ref1),
                                           std::make_unique<StmtRef>(ref2), false);
  }
};

TEST_CASE("Test Query Evaluator") {
  PkbStub pkb = PkbStub();
  QueryEvaluator qe = QueryEvaluator(pkb);
  PqlDeclaration decl = PqlDeclaration("dummy", EntityType::STMT);
  std::shared_ptr<Clause> follows_clause;
  std::vector<std::vector<std::string>> result;

  SECTION("Evaluate boolean query") {
    // bool query with no clauses is true
    REQUIRE(qe.evaluateQuery({}));

    // bool query with a result
    follows_clause =
        QeFactoryMethods::getFollowsClause(StmtRef(), StmtRef(1));
    REQUIRE(qe.evaluateQuery({follows_clause}));

    // bool query with no result
    follows_clause = QeFactoryMethods::getFollowsClause(StmtRef(1), StmtRef());
    REQUIRE_FALSE(qe.evaluateQuery({follows_clause}));
  }

  SECTION("Evaluate query with single result") {
    decl = PqlDeclaration("a", EntityType::CONSTANT);
    result = qe.evaluateQuery({decl}, {});
    REQUIRE(ArrayUtility::flattenVector(result) == pkb.CONSTANTS);

    decl = PqlDeclaration("Longname", EntityType::STMT);
    result = qe.evaluateQuery({decl}, {});
    REQUIRE(ArrayUtility::flattenVector(result) == pkb.STATEMENTS);
  }

  SECTION("Select and Follows Clause with boolean result") {
    decl = PqlDeclaration("a", EntityType::CONSTANT);
    follows_clause = QeFactoryMethods::getFollowsClause(StmtRef(1), StmtRef(1));
    result =
        qe.evaluateQuery({decl}, {follows_clause});
    REQUIRE(ArrayUtility::flattenVector(result) == pkb.CONSTANTS);

    decl = PqlDeclaration("a", EntityType::STMT);
    follows_clause = QeFactoryMethods::getFollowsClause(StmtRef(), StmtRef(1));
    result =
        qe.evaluateQuery({decl}, {follows_clause});
    REQUIRE(ArrayUtility::flattenVector(result) == pkb.STATEMENTS);

    decl = PqlDeclaration("a", EntityType::STMT);
    follows_clause = QeFactoryMethods::getFollowsClause(StmtRef(1), StmtRef());
    result =
        qe.evaluateQuery({decl}, {follows_clause});
    REQUIRE(result.empty());

    decl = PqlDeclaration("a", EntityType::VARIABLE);
    follows_clause = QeFactoryMethods::getFollowsClause(StmtRef(), StmtRef());
    result =
        qe.evaluateQuery({decl}, {follows_clause});
    REQUIRE(result.empty());
  }

  SECTION("Select and Follows Clause with 0 common declarations") {
    decl = PqlDeclaration("a", EntityType::CONSTANT);
    follows_clause = QeFactoryMethods::getFollowsClause(
        StmtRef(PqlDeclaration("b", EntityType::STMT)),
        StmtRef());
    result =
        qe.evaluateQuery({decl}, {follows_clause});
    REQUIRE(result.empty());

    decl = PqlDeclaration("a", EntityType::PROCEDURE);
    follows_clause = QeFactoryMethods::getFollowsClause(
        StmtRef(52),
        StmtRef(PqlDeclaration("b", EntityType::PRINT)));
    result =
        qe.evaluateQuery({decl}, {follows_clause});
    REQUIRE(result.empty());

    // Case where follows clause has declarations that are different from select
    // clause
    decl = PqlDeclaration("a", EntityType::STMT);
    follows_clause = QeFactoryMethods::getFollowsClause(
        StmtRef(PqlDeclaration("b", EntityType::STMT)),
        StmtRef(PqlDeclaration("s", EntityType::STMT)));
    result =
        qe.evaluateQuery({decl}, {follows_clause});
    REQUIRE(ArrayUtility::flattenVector(result) == pkb.STATEMENTS);
  }
}


TEST_CASE("Pattern clauses") {
  PkbStub pkb = PkbStub();
  auto qe = QueryEvaluator(pkb);
  // decl, partial
  auto a = PqlDeclaration("a", EntityType::ASSIGN);
  auto v = PqlDeclaration("v", EntityType::VARIABLE);
  SECTION("decl, wild") {
    auto pattern_clause = std::make_shared<PatternClause>(
        a, EntRef(v), MatchType::WILD_MATCH, "");
    auto result = qe.evaluateQuery({v}, {pattern_clause});
    REQUIRE(ArrayUtility::flattenVector(result) == std::vector<std::string>({"varX"}));
  }

  SECTION("decl, partial") {
    auto pattern_clause = std::make_shared<PatternClause>(
        a, EntRef(v), MatchType::PARTIAL_MATCH, "a");
    auto result = qe.evaluateQuery({v}, {pattern_clause});
    REQUIRE(ArrayUtility::flattenVector(result) == std::vector<std::string>({"varY"}));
  }

  SECTION("value, wild") {
    auto pattern_clause = std::make_shared<PatternClause>(
        a, EntRef("varName"), MatchType::WILD_MATCH, "");
    auto result = qe.evaluateQuery({a}, {pattern_clause});
    REQUIRE(ArrayUtility::flattenVector(result) == std::vector<std::string>({"3"}));
  }

  SECTION("value, partial") {
    auto pattern_clause = std::make_shared<PatternClause>(
        a, EntRef("varName"), MatchType::PARTIAL_MATCH, "a");
    auto result = qe.evaluateQuery({a}, {pattern_clause});
    REQUIRE(ArrayUtility::flattenVector(result) == std::vector<std::string>({"4"}));
  }

  SECTION("wild, wild") {
    auto pattern_clause =
        std::make_shared<PatternClause>(a, EntRef(), MatchType::WILD_MATCH, "");
    auto result = qe.evaluateQuery({a}, {pattern_clause});
    REQUIRE(ArrayUtility::flattenVector(result) == std::vector<std::string>({"1"}));
  }

  SECTION("wild, partial") {
    auto pattern_clause = std::make_shared<PatternClause>(
        a, EntRef(), MatchType::PARTIAL_MATCH, "blah");
    auto result = qe.evaluateQuery({a}, {pattern_clause});
    REQUIRE(ArrayUtility::flattenVector(result) == std::vector<std::string>({"2"}));
  }

  SECTION("semantic errors") {
    REQUIRE_THROWS_AS(std::make_shared<PatternClause>(
                          a, EntRef(a), MatchType::PARTIAL_MATCH, "a"),
                      InvalidSemanticsException);
    REQUIRE_THROWS_AS(std::make_shared<PatternClause>(
                          v, EntRef(v), MatchType::PARTIAL_MATCH, "a"),
                      InvalidSemanticsException);
  }
}

// TODO: Future, separate clause tests from query evaluator tests.
// Clean up the testing code.
// add 3 clause evaluation testing
