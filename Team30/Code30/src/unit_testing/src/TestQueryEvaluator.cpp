#include <memory>

#include "../../spa/src/program_knowledge_base/PKBQPSInterface.h"
#include "../../spa/src/query_processing_system/common/FollowsClause.h"
#include "../../spa/src/query_processing_system/common/SelectClause.h"
#include "../../spa/src/query_processing_system/evaluator/QueryEvaluator.h"
#include "../../spa/src/query_processing_system/exceptions/InvalidSemanticsException.h"
#include "PkbStub.h"
#include "catch.hpp"

class QeFactoryMethods {
 public:
  static std::unique_ptr<SelectClause> getSelectClause(std::string name,
                                                       EntityType type) {
    return std::make_unique<SelectClause>(PqlDeclaration(name, type));
  }

  static std::unique_ptr<FollowsClause> getFollowsClause(StmtRef ref1,
                                                         StmtRef ref2) {
    return std::make_unique<FollowsClause>(std::make_unique<StmtRef>(ref1),
                                           std::make_unique<StmtRef>(ref2));
  }
};

TEST_CASE("Evaluate query with only select clause") {
  PkbStub pkb = PkbStub();
  QueryEvaluator qe = QueryEvaluator(pkb);

  std::unique_ptr<SelectClause> clause =
      QeFactoryMethods::getSelectClause("a", EntityType::CONSTANT);
  std::vector<std::string> result = qe.evaluateQuery(std::move(clause), {});
  REQUIRE(result == pkb.CONSTANTS);

  clause = QeFactoryMethods::getSelectClause("bc", EntityType::PROCEDURE);
  result = qe.evaluateQuery(std::move(clause), {});
  REQUIRE(result == pkb.PROCEDURES);

  clause = QeFactoryMethods::getSelectClause("ddd", EntityType::VARIABLE);
  result = qe.evaluateQuery(std::move(clause), {});
  REQUIRE(result == pkb.VARIABLES);

  clause = QeFactoryMethods::getSelectClause("Longname", EntityType::STMT);
  result = qe.evaluateQuery(std::move(clause), {});
  REQUIRE(result == pkb.STATEMENTS);
}

TEST_CASE("Select and Follows Clause with boolean result") {
  PkbStub pkb = PkbStub();
  QueryEvaluator qe = QueryEvaluator(pkb);
  std::unique_ptr<SelectClause> select_clause;
  std::unique_ptr<Clause> follows_clause;
  std::vector<std::string> result;

  select_clause = QeFactoryMethods::getSelectClause("a", EntityType::CONSTANT);
  follows_clause = QeFactoryMethods::getFollowsClause(StmtRef(1), StmtRef(1));
  result =
      qe.evaluateQuery(std::move(select_clause), {std::move(follows_clause)});
  REQUIRE(result == pkb.CONSTANTS);

  select_clause = QeFactoryMethods::getSelectClause("a", EntityType::STMT);
  follows_clause = QeFactoryMethods::getFollowsClause(StmtRef(), StmtRef(1));
  result =
      qe.evaluateQuery(std::move(select_clause), {std::move(follows_clause)});
  REQUIRE(result == pkb.STATEMENTS);

  select_clause = QeFactoryMethods::getSelectClause("a", EntityType::STMT);
  follows_clause = QeFactoryMethods::getFollowsClause(StmtRef(1), StmtRef());
  result =
      qe.evaluateQuery(std::move(select_clause), {std::move(follows_clause)});
  REQUIRE(result.empty());

  select_clause = QeFactoryMethods::getSelectClause("a", EntityType::VARIABLE);
  follows_clause = QeFactoryMethods::getFollowsClause(StmtRef(), StmtRef());
  result =
      qe.evaluateQuery(std::move(select_clause), {std::move(follows_clause)});
  REQUIRE(result.empty());
}

TEST_CASE("Select and Follows Clause with 0 common declarations") {
  PkbStub pkb = PkbStub();
  QueryEvaluator qe = QueryEvaluator(pkb);
  std::unique_ptr<SelectClause> select_clause;
  std::unique_ptr<FollowsClause> follows_clause;
  std::vector<std::string> result;

  select_clause = QeFactoryMethods::getSelectClause("a", EntityType::CONSTANT);
  follows_clause = QeFactoryMethods::getFollowsClause(
      StmtRef(PqlDeclaration("b", EntityType::STMT)),
      StmtRef());
  result =
      qe.evaluateQuery(std::move(select_clause), {std::move(follows_clause)});
  REQUIRE(result.empty());

  select_clause = QeFactoryMethods::getSelectClause("a", EntityType::PROCEDURE);
  follows_clause = QeFactoryMethods::getFollowsClause(
      StmtRef(52),
      StmtRef(PqlDeclaration("b", EntityType::PRINT)));
  result =
      qe.evaluateQuery(std::move(select_clause), {std::move(follows_clause)});
  REQUIRE(result.empty());

  // Case where follows clause has declarations that are different from select
  // clause
  select_clause = QeFactoryMethods::getSelectClause("a", EntityType::STMT);
  follows_clause = QeFactoryMethods::getFollowsClause(
      StmtRef(PqlDeclaration("b", EntityType::STMT)),
      StmtRef(PqlDeclaration("s", EntityType::STMT)));
  result =
      qe.evaluateQuery(std::move(select_clause), {std::move(follows_clause)});
  REQUIRE(result == pkb.STATEMENTS);
}

TEST_CASE("Select and Follows Clause with 1 common declarations") {
  PkbStub pkb = PkbStub();
  QueryEvaluator qe = QueryEvaluator(pkb);
  std::unique_ptr<SelectClause> select_clause;
  std::unique_ptr<Clause> follows_clause;
  std::vector<std::string> result;

  // Some overlapping values between declarations
  select_clause = QeFactoryMethods::getSelectClause("a", EntityType::STMT);
  follows_clause = QeFactoryMethods::getFollowsClause(
      StmtRef(PqlDeclaration("a", EntityType::STMT)),
      StmtRef(PqlDeclaration("s", EntityType::STMT)));
  result =
      qe.evaluateQuery(std::move(select_clause), {std::move(follows_clause)});
  REQUIRE(result == std::vector<std::string>({"2"}));

  select_clause = QeFactoryMethods::getSelectClause("a", EntityType::STMT);
  follows_clause = QeFactoryMethods::getFollowsClause(
      StmtRef(PqlDeclaration("asd", EntityType::STMT)),
      StmtRef(PqlDeclaration("a", EntityType::STMT)));
  result =
      qe.evaluateQuery(std::move(select_clause), {std::move(follows_clause)});
  REQUIRE(result == std::vector<std::string>({"1", "2"}));

  select_clause = QeFactoryMethods::getSelectClause("abc", EntityType::STMT);
  follows_clause = QeFactoryMethods::getFollowsClause(
      StmtRef(),
      StmtRef(PqlDeclaration("abc", EntityType::STMT)));
  result =
      qe.evaluateQuery(std::move(select_clause), {std::move(follows_clause)});
  REQUIRE(result == std::vector<std::string>({"1", "3"}));

  select_clause = QeFactoryMethods::getSelectClause("abc", EntityType::STMT);
  follows_clause = QeFactoryMethods::getFollowsClause(
      StmtRef(PqlDeclaration("abc", EntityType::STMT)),
      StmtRef(1));
  result =
      qe.evaluateQuery(std::move(select_clause), {std::move(follows_clause)});
  REQUIRE(result == std::vector<std::string>({"2"}));

  // No overlapping values between declarations
  select_clause = QeFactoryMethods::getSelectClause("abc", EntityType::STMT);
  follows_clause = QeFactoryMethods::getFollowsClause(
      StmtRef(1),
      StmtRef(PqlDeclaration("abc", EntityType::STMT)));
  result =
      qe.evaluateQuery(std::move(select_clause), {std::move(follows_clause)});
  REQUIRE(result.empty());

  select_clause = QeFactoryMethods::getSelectClause("abc", EntityType::STMT);
  follows_clause = QeFactoryMethods::getFollowsClause(
      StmtRef(PqlDeclaration("abc", EntityType::STMT)),
      StmtRef());
  result =
      qe.evaluateQuery(std::move(select_clause), {std::move(follows_clause)});
  REQUIRE(result.empty());

  select_clause = QeFactoryMethods::getSelectClause("a", EntityType::PRINT);
  follows_clause = QeFactoryMethods::getFollowsClause(
      StmtRef(PqlDeclaration("a", EntityType::PRINT)),
      StmtRef(PqlDeclaration("s", EntityType::PRINT)));
  result =
      qe.evaluateQuery(std::move(select_clause), {std::move(follows_clause)});
  REQUIRE(result.empty());
}

TEST_CASE("Select and Follows Clause with 2 common declarations") {
  PkbStub pkb = PkbStub();
  QueryEvaluator qe = QueryEvaluator(pkb);
  std::unique_ptr<SelectClause> select_clause;
  std::unique_ptr<Clause> follows_clause;
  std::vector<std::string> result;

  select_clause = QeFactoryMethods::getSelectClause("abc", EntityType::STMT);
  follows_clause = QeFactoryMethods::getFollowsClause(
      StmtRef(PqlDeclaration("abc", EntityType::STMT)),
      StmtRef(PqlDeclaration("abc", EntityType::STMT)));
  result =
      qe.evaluateQuery(std::move(select_clause), {std::move(follows_clause)});
  REQUIRE(result == std::vector<std::string>({"2"}));
}

TEST_CASE("Pattern clauses") {
  PkbStub pkb = PkbStub();
  auto qe = QueryEvaluator(pkb);
  // decl, partial
  auto a = PqlDeclaration("a", EntityType::ASSIGN);
  auto v = PqlDeclaration("v", EntityType::VARIABLE);
  auto SELECT_A = std::make_unique<SelectClause>(a);
  auto SELECT_V = std::make_unique<SelectClause>(v);
  SECTION("decl, wild") {
    auto pattern_clause = std::make_shared<PatternClause>(
        a, EntRef(v), MatchType::WILD_MATCH, "");
    auto result = qe.evaluateQuery(std::move(SELECT_V), {pattern_clause});
    REQUIRE(result == std::vector<std::string>({"varX"}));
  }

  SECTION("decl, partial") {
    auto pattern_clause = std::make_shared<PatternClause>(
        a, EntRef(v), MatchType::PARTIAL_MATCH, "a");
    auto result = qe.evaluateQuery(std::move(SELECT_V), {pattern_clause});
    REQUIRE(result == std::vector<std::string>({"varY"}));
  }

  SECTION("value, wild") {
    auto pattern_clause = std::make_shared<PatternClause>(
        a, EntRef("varName"), MatchType::WILD_MATCH, "");
    auto result = qe.evaluateQuery(std::move(SELECT_A), {pattern_clause});
    REQUIRE(result == std::vector<std::string>({"3"}));
  }

  SECTION("value, partial") {
    auto pattern_clause = std::make_shared<PatternClause>(
        a, EntRef("varName"), MatchType::PARTIAL_MATCH, "a");
    auto result = qe.evaluateQuery(std::move(SELECT_A), {pattern_clause});
    REQUIRE(result.empty());
  }

  SECTION("wild, wild") {
    auto pattern_clause =
        std::make_shared<PatternClause>(a, EntRef(), MatchType::WILD_MATCH, "");
    auto result = qe.evaluateQuery(std::move(SELECT_A), {pattern_clause});
    REQUIRE(result == std::vector<std::string>({"1"}));
  }

  SECTION("wild, partial") {
    auto pattern_clause = std::make_shared<PatternClause>(
        a, EntRef(), MatchType::PARTIAL_MATCH, "blah");
    auto result = qe.evaluateQuery(std::move(SELECT_A), {pattern_clause});
    REQUIRE(result == std::vector<std::string>({"2"}));
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
