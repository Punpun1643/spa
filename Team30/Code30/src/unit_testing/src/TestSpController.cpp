#include <memory>
#include <optional>

#include "../../spa/src/source_processor/SpController.h"
#include "../../spa/src/source_processor/node/stmt_node/StmtNode.h"
#include "../../spa/src/program_knowledge_base/PkbApi.h"
#include "../../spa/src/query_processing_system/common/EntityType.h"
#include "../../spa/src/query_processing_system/common/FollowsClause.h"
#include "../../spa/src/query_processing_system/common/SelectClause.h"
#include "../../spa/src/query_processing_system/evaluator/QueryEvaluator.h"
#include <iostream>
#include "catch.hpp"

namespace TestSpController {
  std::vector<std::string> PROCEDURES = {"procedure1", "procedure2",
                                         "procedure3"};
  std::vector<std::string> CONSTANTS = {"12", "13", "14", "15"};
  std::vector<std::string> VARIABLES = {"varX"};
  std::vector<std::string> STATEMENTS = {"1", "2", "3"};
}

class PkbStub : public PkbApi {

 public:
  int insertFollowsCallCount;
  PkbStub() : insertFollowsCallCount(0) {}

  bool insertFollows(std::shared_ptr<StmtNode> stmt1,
                   std::shared_ptr<StmtNode> stmt2) override {
    std::cout << insertFollowsCallCount << std::endl;
    ++insertFollowsCallCount;
    return true;
  }

  std::unique_ptr<std::vector<std::string>> getEntitiesWithType(
      EntityType type) override {
    std::unique_ptr<std::vector<std::string>> output =
        std::make_unique<std::vector<std::string>>();

    if (type == PROCEDURE) {
      *output = TestSpController::PROCEDURES;
    } else if (type == CONSTANT) {
      *output = TestSpController::CONSTANTS;
    } else if (type == VARIABLE) {
      *output = TestSpController::VARIABLES;
    } else {  // statement type
      *output = TestSpController::STATEMENTS;
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

TEST_CASE("Test SpController parses and extract nodes information successfully",
          "[]") {
  SECTION("Test parse and extract short source program file") {
    SpController spController = SpController();
    std::string filePath =
        "/Users/papattaradaapithanangsiri/23s1-cp-spa-team-30/Team30/Tests30/"
        "Sample_source_w4_short.txt";
    PkbStub pkb = PkbStub();
    spController.parseAndExtract(pkb, filePath);

    REQUIRE(pkb.insertFollowsCallCount == 2);
  }
}
