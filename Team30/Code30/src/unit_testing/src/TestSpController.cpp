#include <memory>
#include <optional>

#include "../../spa/src/program_knowledge_base/PkbApi.h"
#include "../../spa/src/query_processing_system/common/EntityType.h"
#include "../../spa/src/query_processing_system/common/FollowsClause.h"
#include "../../spa/src/query_processing_system/common/SelectClause.h"
#include "../../spa/src/query_processing_system/evaluator/QueryEvaluator.h"
#include "../../spa/src/source_processor/SpController.h"
#include "../../spa/src/source_processor/node/stmt_node/StmtNode.h"
#include "catch.hpp"

class PkbStubSpController : public PkbApi {
 public:
  int insertFollowsCallCount;
  PkbStubSpController() : insertFollowsCallCount(0) {}

  bool insertFollows(std::shared_ptr<StmtNode> stmt1,
                     std::shared_ptr<StmtNode> stmt2) override {
    ++insertFollowsCallCount;
    return true;
  }

  std::unique_ptr<std::vector<std::string>> getEntitiesWithType(
      EntityType type) override {
    std::unique_ptr<std::vector<std::string>> output =
        std::make_unique<std::vector<std::string>>();
    return output;
  }

  std::optional<std::pair<int, int>> getFollows(int s1_line_num,
                                                EntityType s2_type) override {
    return std::nullopt;
  }

  std::optional<std::pair<int, int>> getFollows(EntityType s1_type,
                                                int s2_line_num) override {
    return std::nullopt;
  }

  std::unique_ptr<std::vector<std::pair<int, int>>> getFollows(
      EntityType s1_type, EntityType s2_type) override {
    return std::make_unique<std::vector<std::pair<int, int>>>();
  }
};

TEST_CASE("Test SpController parses and extract nodes information successfully",
          "[parseAndExtract, Sample_source_w4_short.txt]") {
  SECTION(
      "Test parse and extract short source program file should process the "
      "correct number of Follows relationships") {
    SpController spController = SpController();
    std::string filePath =
        "/Users/papattaradaapithanangsiri/23s1-cp-spa-team-30/Team30/Tests30/"
        "Sample_source_w4_short.txt";
    PkbStubSpController pkb = PkbStubSpController();
    spController.parseAndExtract(pkb, filePath);

    REQUIRE(pkb.insertFollowsCallCount == 2);
  }
}
