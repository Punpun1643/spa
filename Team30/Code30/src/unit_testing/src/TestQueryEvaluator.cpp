#include <memory>
#include <optional>

#include "../../spa/src/program_knowledge_base/PkbApi.h"
#include "../../spa/src/query_processing_system/common/EntityType.h"
#include "../../spa/src/query_processing_system/common/FollowsClause.h"
#include "../../spa/src/query_processing_system/common/SelectClause.h"
#include "../../spa/src/query_processing_system/evaluator/QueryEvaluator.h"
#include "catch.hpp"

std::vector<std::string> PROCEDURES = {"procedure1", "procedure2",
                                       "procedure3"};
std::vector<std::string> CONSTANTS = {"12", "13", "14", "15"};
std::vector<std::string> VARIABLES = {"varX"};
std::vector<std::string> STATEMENTS = {"1", "2", "3"};

class PkbStub : public PkbApi {
  bool insertFollows(std::shared_ptr<StmtNode> stmt1,
                     std::shared_ptr<StmtNode> stmt2) override {
    return true;
  }

  std::unique_ptr<std::vector<std::string>> getEntitiesWithType(
      EntityType type) override {
    std::unique_ptr<std::vector<std::string>> output =
        std::make_unique<std::vector<std::string>>();

    if (type == PROCEDURE) {
      *output = PROCEDURES;
    } else if (type == CONSTANT) {
      *output = CONSTANTS;
    } else if (type == VARIABLE) {
      *output = VARIABLES;
    } else {  // statement type
      *output = STATEMENTS;
    }
    return output;
  }

  std::optional<std::pair<int, int>> getFollows(int s1_line_num,
                                                EntityType s2_type) override {
    if (s1_line_num == 1) {
      return std::optional(std::make_pair(1, 2));
    } else if (s1_line_num == 2) {
      return std::nullopt;  // empty
    }
  }

  std::optional<std::pair<int, int>> getFollows(EntityType s1_type,
                                                int s2_line_num) override {
    if (s2_line_num == 1) {
      return std::optional(std::make_pair(3, 1));
    } else if (s2_line_num == 2) {
      return std::nullopt;  // empty
    }
  }

  std::unique_ptr<std::vector<std::pair<int, int>>> getFollows(
      EntityType s1_type, EntityType s2_type) override {
    if (s1_type == STMT && s2_type == STMT) {
      std::unique_ptr<std::vector<std::pair<int, int>>> result =
          std::make_unique<std::vector<std::pair<int, int>>>();
      result->push_back(std::make_pair(5, 10));
      result->push_back(std::make_pair(9, 1));
      result->push_back(std::make_pair(2, 2));
      result->push_back(std::make_pair(2, 1));
      return result;
    } else {
      return std::make_unique<std::vector<std::pair<int, int>>>();
    }
  }
};

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
  REQUIRE(result == CONSTANTS);

  clause = QeFactoryMethods::getSelectClause("bc", EntityType::PROCEDURE);
  result = *qe.evaluateQuery(std::move(clause));
  REQUIRE(result == PROCEDURES);

  clause = QeFactoryMethods::getSelectClause("ddd", EntityType::VARIABLE);
  result = *qe.evaluateQuery(std::move(clause));
  REQUIRE(result == VARIABLES);

  clause = QeFactoryMethods::getSelectClause("fawfnaawf", EntityType::STMT);
  result = *qe.evaluateQuery(std::move(clause));
  REQUIRE(result == STATEMENTS);
}

TEST_CASE("Select and Follows Clause with 0 common declarations") {
  PkbStub pkb = PkbStub();
  QueryEvaluator qe = QueryEvaluator(pkb);
  std::unique_ptr<SelectClause> select_clause;
  std::unique_ptr<FollowsClause> follows_clause;
  std::vector<std::string> result;

  select_clause = QeFactoryMethods::getSelectClause("a", EntityType::CONSTANT);
  follows_clause = QeFactoryMethods::getFollowsClause(StmtRef(), StmtRef());
  result =
      *qe.evaluateQuery(std::move(select_clause), std::move(follows_clause));
  REQUIRE(result == CONSTANTS);

  select_clause = QeFactoryMethods::getSelectClause("a", EntityType::PROCEDURE);
  follows_clause = QeFactoryMethods::getFollowsClause(StmtRef(52), StmtRef(12));
  result =
      *qe.evaluateQuery(std::move(select_clause), std::move(follows_clause));
  REQUIRE(result == PROCEDURES);

  // Case where follows clause has declarations that are different from select
  // clause
  select_clause = QeFactoryMethods::getSelectClause("a", EntityType::STMT);
  follows_clause = QeFactoryMethods::getFollowsClause(
      StmtRef(QeFactoryMethods::getDeclaration("b", EntityType::STMT)),
      StmtRef(QeFactoryMethods::getDeclaration("s", EntityType::STMT)));
  result =
      *qe.evaluateQuery(std::move(select_clause), std::move(follows_clause));
  REQUIRE(result == STATEMENTS);
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

  select_clause = QeFactoryMethods::getSelectClause("abc", EntityType::STMT);
  follows_clause = QeFactoryMethods::getFollowsClause(
      StmtRef(),
      StmtRef(QeFactoryMethods::getDeclaration("abc", EntityType::STMT)));
  result =
      *qe.evaluateQuery(std::move(select_clause), std::move(follows_clause));
  REQUIRE(result == std::vector<std::string>({"1", "2"}));

  select_clause = QeFactoryMethods::getSelectClause("abc", EntityType::STMT);
  follows_clause = QeFactoryMethods::getFollowsClause(
      StmtRef(1),
      StmtRef(QeFactoryMethods::getDeclaration("abc", EntityType::STMT)));
  result =
      *qe.evaluateQuery(std::move(select_clause), std::move(follows_clause));
  REQUIRE(result == std::vector<std::string>({"2"}));

  select_clause = QeFactoryMethods::getSelectClause("abc", EntityType::STMT);
  follows_clause = QeFactoryMethods::getFollowsClause(
      StmtRef(QeFactoryMethods::getDeclaration("abc", EntityType::STMT)),
      StmtRef(1));
  result =
      *qe.evaluateQuery(std::move(select_clause), std::move(follows_clause));
  REQUIRE(result == std::vector<std::string>({"3"}));

  // No overlapping values between declarations
  select_clause = QeFactoryMethods::getSelectClause("a", EntityType::STMT);
  follows_clause = QeFactoryMethods::getFollowsClause(
      StmtRef(QeFactoryMethods::getDeclaration("a", EntityType::STMT)),
      StmtRef(2));
  result =
      *qe.evaluateQuery(std::move(select_clause), std::move(follows_clause));
  REQUIRE(result.empty());

  select_clause = QeFactoryMethods::getSelectClause("a", EntityType::STMT);
  follows_clause = QeFactoryMethods::getFollowsClause(
      StmtRef(2),
      StmtRef(QeFactoryMethods::getDeclaration("a", EntityType::STMT)));
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