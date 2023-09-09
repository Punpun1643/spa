#include "catch.hpp"
#include "../../spa/src/program_knowledge_base/PkbApi.h"
#include "../../spa/src/query_processing_system/common/EntityType.h"
#include "../../spa/src/query_processing_system/common/SelectClause.h"
#include "../../spa/src/query_processing_system/evaluator/QueryEvaluator.h"
#include <memory>

std::vector<std::string> PROCEDURES = {"procedure1", "procedure2", "procedure3"};
std::vector<std::string> CONSTANTS = {"12", "13", "14", "15"};
std::vector<std::string> VARIABLES = {"varX"};
std::vector<std::string> STATEMENTS = {"1", "2", "3"};

class PkbStub : public PkbApi {
  std::unique_ptr<std::vector<std::string>> getEntitiesWithType(EntityType type) override {
    std::unique_ptr<std::vector<std::string>> output =
        std::make_unique<std::vector<std::string>>();

    if (type == PROCEDURE) {
      *output = PROCEDURES;
    } else if (type == CONSTANT) {
      *output = CONSTANTS;
    } else if (type == VARIABLE) {
      *output = VARIABLES;
    } else { // statement type
      *output = STATEMENTS;
    }
    return output;
  }
};

class SelectClauseFactory {
 public:
  static std::unique_ptr<SelectClause> getClause(std::string name, EntityType type) {
    PqlDeclaration declaration = PqlDeclaration(
        std::make_shared<std::string>(name),
        type);
    return std::make_unique<SelectClause>(
        std::make_shared<PqlDeclaration>(declaration));
  }
};

TEST_CASE("Evaluate query with only select clause") {
  PkbStub pkb = PkbStub();
  QueryEvaluator qe = QueryEvaluator(pkb);

  std::unique_ptr<SelectClause> clause =
      SelectClauseFactory::getClause("a", EntityType::CONSTANT);
  std::vector<std::string> result = *qe.evaluateQuery(std::move(clause));
  REQUIRE(result == CONSTANTS);

  clause = SelectClauseFactory::getClause("bc", EntityType::PROCEDURE);
  result = *qe.evaluateQuery(std::move(clause));
  REQUIRE(result == PROCEDURES);

  clause = SelectClauseFactory::getClause("ddd", EntityType::VARIABLE);
  result = *qe.evaluateQuery(std::move(clause));
  REQUIRE(result == VARIABLES);

  clause = SelectClauseFactory::getClause("fawfnaawf", EntityType::STMT);
  result = *qe.evaluateQuery(std::move(clause));
  REQUIRE(result == STATEMENTS);
}
