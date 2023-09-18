#include <memory>

#include "../../spa/src/program_knowledge_base/PkbApi.h"
#include "../../spa/src/query_processing_system/common/FollowsClause.h"
#include "../../spa/src/query_processing_system/common/SelectClause.h"
#include "../../spa/src/query_processing_system/evaluator/QueryEvaluator.h"
#include "PkbStub.h"
#include "catch.hpp"

class QeFactoryMethods {
 public:
  static std::shared_ptr<PqlDeclaration> getDeclaration(std::string name,
                                                        EntityType type) {
    return std::make_shared<PqlDeclaration>(std::make_shared<std::string>(name),
                                            type);
  }

  static std::unique_ptr<SelectClause> getSelectClause(std::string name,
                                                       EntityType type) {
    return std::make_unique<SelectClause>(getDeclaration(name, type));
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
  std::vector<std::string> result = *qe.evaluateQuery(std::move(clause));
  REQUIRE(result == pkb.CONSTANTS);

  clause = QeFactoryMethods::getSelectClause("bc", EntityType::PROCEDURE);
  result = *qe.evaluateQuery(std::move(clause));
  REQUIRE(result == pkb.PROCEDURES);

  clause = QeFactoryMethods::getSelectClause("ddd", EntityType::VARIABLE);
  result = *qe.evaluateQuery(std::move(clause));
  REQUIRE(result == pkb.VARIABLES);

  clause = QeFactoryMethods::getSelectClause("Longname", EntityType::STMT);
  result = *qe.evaluateQuery(std::move(clause));
  REQUIRE(result == pkb.STATEMENTS);
}

TEST_CASE("Select and Follows Clause with boolean result") {
  PkbStub pkb = PkbStub();
  QueryEvaluator qe = QueryEvaluator(pkb);
  std::unique_ptr<SelectClause> select_clause;
  std::unique_ptr<FollowsClause> follows_clause;
  std::vector<std::string> result;

  select_clause = QeFactoryMethods::getSelectClause("a", EntityType::CONSTANT);
  follows_clause = QeFactoryMethods::getFollowsClause(StmtRef(1), StmtRef(1));
  result =
      *qe.evaluateQuery(std::move(select_clause), std::move(follows_clause));
  REQUIRE(result == pkb.CONSTANTS);

  select_clause = QeFactoryMethods::getSelectClause("a", EntityType::STMT);
  follows_clause = QeFactoryMethods::getFollowsClause(StmtRef(), StmtRef(1));
  result =
      *qe.evaluateQuery(std::move(select_clause), std::move(follows_clause));
  REQUIRE(result == pkb.STATEMENTS);

  select_clause = QeFactoryMethods::getSelectClause("a", EntityType::STMT);
  follows_clause = QeFactoryMethods::getFollowsClause(StmtRef(1), StmtRef());
  result =
      *qe.evaluateQuery(std::move(select_clause), std::move(follows_clause));
  REQUIRE(result.empty());

  select_clause = QeFactoryMethods::getSelectClause("a", EntityType::VARIABLE);
  follows_clause = QeFactoryMethods::getFollowsClause(StmtRef(), StmtRef());
  result =
      *qe.evaluateQuery(std::move(select_clause), std::move(follows_clause));
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
      StmtRef(QeFactoryMethods::getDeclaration("b", EntityType::STMT)),
      StmtRef());
  result =
      *qe.evaluateQuery(std::move(select_clause), std::move(follows_clause));
  REQUIRE(result == pkb.CONSTANTS);

  select_clause = QeFactoryMethods::getSelectClause("a", EntityType::PROCEDURE);
  follows_clause = QeFactoryMethods::getFollowsClause(
      StmtRef(52),
      StmtRef(QeFactoryMethods::getDeclaration("b", EntityType::PRINT)));
  result =
      *qe.evaluateQuery(std::move(select_clause), std::move(follows_clause));
  REQUIRE(result == pkb.PROCEDURES);

  // Case where follows clause has declarations that are different from select
  // clause
  select_clause = QeFactoryMethods::getSelectClause("a", EntityType::STMT);
  follows_clause = QeFactoryMethods::getFollowsClause(
      StmtRef(QeFactoryMethods::getDeclaration("b", EntityType::STMT)),
      StmtRef(QeFactoryMethods::getDeclaration("s", EntityType::STMT)));
  result =
      *qe.evaluateQuery(std::move(select_clause), std::move(follows_clause));
  REQUIRE(result == pkb.STATEMENTS);
}

TEST_CASE("Select and Follows Clause with 1 common declarations") {
  PkbStub pkb = PkbStub();
  QueryEvaluator qe = QueryEvaluator(pkb);
  std::unique_ptr<SelectClause> select_clause;
  std::unique_ptr<FollowsClause> follows_clause;
  std::vector<std::string> result;

  // Some overlapping values between declarations
  select_clause = QeFactoryMethods::getSelectClause("a", EntityType::STMT);
  follows_clause = QeFactoryMethods::getFollowsClause(
      StmtRef(QeFactoryMethods::getDeclaration("a", EntityType::STMT)),
      StmtRef(QeFactoryMethods::getDeclaration("s", EntityType::STMT)));
  result =
      *qe.evaluateQuery(std::move(select_clause), std::move(follows_clause));
  REQUIRE(result == std::vector<std::string>({"2"}));

  select_clause = QeFactoryMethods::getSelectClause("a", EntityType::STMT);
  follows_clause = QeFactoryMethods::getFollowsClause(
      StmtRef(QeFactoryMethods::getDeclaration("asd", EntityType::STMT)),
      StmtRef(QeFactoryMethods::getDeclaration("a", EntityType::STMT)));
  result =
      *qe.evaluateQuery(std::move(select_clause), std::move(follows_clause));
  REQUIRE(result == std::vector<std::string>({"1", "2"}));

  select_clause = QeFactoryMethods::getSelectClause("abc", EntityType::STMT);
  follows_clause = QeFactoryMethods::getFollowsClause(
      StmtRef(),
      StmtRef(QeFactoryMethods::getDeclaration("abc", EntityType::STMT)));
  result =
      *qe.evaluateQuery(std::move(select_clause), std::move(follows_clause));
  REQUIRE(result == std::vector<std::string>({"1", "3"}));

  select_clause = QeFactoryMethods::getSelectClause("abc", EntityType::STMT);
  follows_clause = QeFactoryMethods::getFollowsClause(
      StmtRef(QeFactoryMethods::getDeclaration("abc", EntityType::STMT)),
      StmtRef(1));
  result =
      *qe.evaluateQuery(std::move(select_clause), std::move(follows_clause));
  REQUIRE(result == std::vector<std::string>({"2"}));

  // No overlapping values between declarations
  select_clause = QeFactoryMethods::getSelectClause("abc", EntityType::STMT);
  follows_clause = QeFactoryMethods::getFollowsClause(
      StmtRef(1),
      StmtRef(QeFactoryMethods::getDeclaration("abc", EntityType::STMT)));
  result =
      *qe.evaluateQuery(std::move(select_clause), std::move(follows_clause));
  REQUIRE(result.empty());

  select_clause = QeFactoryMethods::getSelectClause("abc", EntityType::STMT);
  follows_clause = QeFactoryMethods::getFollowsClause(
      StmtRef(QeFactoryMethods::getDeclaration("abc", EntityType::STMT)),
      StmtRef());
  result =
      *qe.evaluateQuery(std::move(select_clause), std::move(follows_clause));
  REQUIRE(result.empty());

  select_clause = QeFactoryMethods::getSelectClause("a", EntityType::PRINT);
  follows_clause = QeFactoryMethods::getFollowsClause(
      StmtRef(QeFactoryMethods::getDeclaration("a", EntityType::PRINT)),
      StmtRef(QeFactoryMethods::getDeclaration("s", EntityType::PRINT)));
  result =
      *qe.evaluateQuery(std::move(select_clause), std::move(follows_clause));
  REQUIRE(result.empty());
}

TEST_CASE("Select and Follows Clause with 2 common declarations") {
  PkbStub pkb = PkbStub();
  QueryEvaluator qe = QueryEvaluator(pkb);
  std::unique_ptr<SelectClause> select_clause;
  std::unique_ptr<FollowsClause> follows_clause;
  std::vector<std::string> result;

  select_clause = QeFactoryMethods::getSelectClause("abc", EntityType::STMT);
  follows_clause = QeFactoryMethods::getFollowsClause(
      StmtRef(QeFactoryMethods::getDeclaration("abc", EntityType::STMT)),
      StmtRef(QeFactoryMethods::getDeclaration("abc", EntityType::STMT)));
  result =
      *qe.evaluateQuery(std::move(select_clause), std::move(follows_clause));
  REQUIRE(result == std::vector<std::string>({"2"}));
}

// TODO: Future, separate clause tests from query evaluator tests.
// Clean up the testing code.