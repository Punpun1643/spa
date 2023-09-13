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

  bool isRelationTrue(std::string value_1, std::string value_2, RelationType rel_type) override { 
    return true;
  }
  bool isRelationTrueGivenFirstValue(std::string value, RelationType rel_type) override {
    return false;
  }
  bool isRelationTrueGivenSecondValue(std::string value, RelationType rel_type)override {
    return true;
  }
  bool isRelationTrueForAny(RelationType relation_type) override {
    return false;
  }
  
  std::unique_ptr<std::vector<std::string>> getRelationValuesGivenFirstType(EntityType entity_type, RelationType rel_type) override {
    return std::make_unique<std::vector<std::string>>(); // empty
  }
  std::unique_ptr<std::vector<std::string>> getRelationValuesGivenSecondType(EntityType entity_type, RelationType rel_type) override {
    std::vector<std::string> vec = {"1","3","5","7","9"};
    return std::make_unique<std::vector<std::string>>(vec);
  }

  std::unique_ptr<std::vector<std::string>> getRelationValues(EntityType entity_type, std::string value, RelationType rel_type) override {
    std::vector<std::string> vec = {"2","4","6","8","10"};
    return std::make_unique<std::vector<std::string>>(vec);
  }
  std::unique_ptr<std::vector<std::string>> getRelationValues(std::string value, EntityType entity_type, RelationType rel_type) override {
    return std::make_unique<std::vector<std::string>>(); // empty
  }

  std::unique_ptr<std::vector<std::pair<std::string, std::string>>> getRelationValues(EntityType entity_type_1,
                                                                                      EntityType entity_type_2,
                                                                                      RelationType rel_type) override {
    if (entity_type_1 == STMT && entity_type_2 == STMT) {
      auto result = std::make_unique<std::vector<std::pair<std::string, std::string>>>();
      result->push_back(std::make_pair("5", "10"));
      result->push_back(std::make_pair("9", "1"));
      result->push_back(std::make_pair("2", "2"));
      result->push_back(std::make_pair("2", "1"));
      return result;
    } else {
      return std::make_unique<std::vector<std::pair<std::string, std::string>>>();
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
  REQUIRE(result == CONSTANTS);

  select_clause = QeFactoryMethods::getSelectClause("a", EntityType::STMT);
  follows_clause = QeFactoryMethods::getFollowsClause(StmtRef(), StmtRef(1));
  result =
      *qe.evaluateQuery(std::move(select_clause), std::move(follows_clause));
  REQUIRE(result == STATEMENTS);

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
  follows_clause = QeFactoryMethods::getFollowsClause(StmtRef(QeFactoryMethods::getDeclaration("b", EntityType::STMT)), StmtRef());
  result =
      *qe.evaluateQuery(std::move(select_clause), std::move(follows_clause));
  REQUIRE(result == CONSTANTS);

  select_clause = QeFactoryMethods::getSelectClause("a", EntityType::PROCEDURE);
  follows_clause = QeFactoryMethods::getFollowsClause(StmtRef(52), StmtRef(QeFactoryMethods::getDeclaration("b", EntityType::PRINT)));
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

//  // Some overlapping values between declarations
//  select_clause = QeFactoryMethods::getSelectClause("a", EntityType::STMT);
//  follows_clause = QeFactoryMethods::getFollowsClause(
//      StmtRef(QeFactoryMethods::getDeclaration("a", EntityType::STMT)),
//      StmtRef(QeFactoryMethods::getDeclaration("s", EntityType::STMT)));
//  result =
//      *qe.evaluateQuery(std::move(select_clause), std::move(follows_clause));
//  REQUIRE(result == std::vector<std::string>({"2"}));
//
//  select_clause = QeFactoryMethods::getSelectClause("a", EntityType::STMT);
//  follows_clause = QeFactoryMethods::getFollowsClause(
//      StmtRef(QeFactoryMethods::getDeclaration("asd", EntityType::STMT)),
//      StmtRef(QeFactoryMethods::getDeclaration("a", EntityType::STMT)));
//  result =
//      *qe.evaluateQuery(std::move(select_clause), std::move(follows_clause));
//  REQUIRE(result == std::vector<std::string>({"1", "2"}));
//
//  select_clause = QeFactoryMethods::getSelectClause("abc", EntityType::STMT);
//  follows_clause = QeFactoryMethods::getFollowsClause(
//      StmtRef(),
//      StmtRef(QeFactoryMethods::getDeclaration("abc", EntityType::STMT)));
//  result =
//      *qe.evaluateQuery(std::move(select_clause), std::move(follows_clause));
//  REQUIRE(result == std::vector<std::string>({"1", "3"}));
//
//  select_clause = QeFactoryMethods::getSelectClause("abc", EntityType::STMT);
//  follows_clause = QeFactoryMethods::getFollowsClause(
//      StmtRef(QeFactoryMethods::getDeclaration("abc", EntityType::STMT)),
//      StmtRef(1));
//  result =
//      *qe.evaluateQuery(std::move(select_clause), std::move(follows_clause));
//  REQUIRE(result == std::vector<std::string>({"2"}));
//
//  // No overlapping values between declarations
//  select_clause = QeFactoryMethods::getSelectClause("abc", EntityType::STMT);
//  follows_clause = QeFactoryMethods::getFollowsClause(
//      StmtRef(1),
//      StmtRef(QeFactoryMethods::getDeclaration("abc", EntityType::STMT)));
//  result =
//      *qe.evaluateQuery(std::move(select_clause), std::move(follows_clause));
//  REQUIRE(result.empty());
//
//  select_clause = QeFactoryMethods::getSelectClause("abc", EntityType::STMT);
//  follows_clause = QeFactoryMethods::getFollowsClause(
//      StmtRef(QeFactoryMethods::getDeclaration("abc", EntityType::STMT)),
//      StmtRef());
//  result =
//      *qe.evaluateQuery(std::move(select_clause), std::move(follows_clause));
//  REQUIRE(result.empty());

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
