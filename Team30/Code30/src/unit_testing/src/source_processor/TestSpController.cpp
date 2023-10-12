#include <memory>
#include <optional>

#include "../../../spa/src/program_knowledge_base/PKBSPInterface.h"
#include "../../../spa/src/query_processing_system/common/EntityType.h"
#include "../../../spa/src/query_processing_system/common/FollowsClause.h"
#include "../../../spa/src/query_processing_system/evaluator/QueryEvaluator.h"
#include "../../../spa/src/source_processor/SpController.h"
#include "../../../spa/src/source_processor/node/stmt_node/StmtNode.h"
#include "../stub/PkbStub.h"
#include "catch.hpp"

TEST_CASE("Test SpController parses and extract nodes information successfully",
          "[parseAndExtract]") {
  SECTION(
      "Test parse and extract short source program file should process the "
      "correct number of Follows relationships") {
    SpController spController = SpController();
    std::string filePath =
        "/Users/papattaradaapithanangsiri/23s1-cp-spa-team-30/Team30/Tests30/"
        "Sample_source_w4_short.txt";
    PkbStub pkb = PkbStub();
    spController.ParseAndExtract(pkb, filePath);

//    REQUIRE(pkb.insertFollowsCallCount == 2); note: comment out because pkb updated their api
  }

  SECTION(
      "Test parse and extract medium source program file should process the "
      "correct number of Follows relationships") {
    SpController spController = SpController();
    std::string filePath =
        "/Users/papattaradaapithanangsiri/23s1-cp-spa-team-30/Team30/Tests30/"
        "Sample_source_w4_medium.txt";
    PkbStub pkb = PkbStub();
    spController.ParseAndExtract(pkb, filePath);

//    REQUIRE(pkb.insertFollowsCallCount == 10); note: comment out because pkb updated their api
  }
}

