#include <source_processor/design_extractor/IExtractionController.h>
#include <source_processor/node/ProcedureNode.h>
#include <source_processor/node/ProgramNode.h>
#include <source_processor/node/StmtLstNode.h>
#include <source_processor/node/stmt_node/PrintNode.h>

#include <iostream>
#include <source_processor/design_extractor/ExtractionController.cpp>

#include "../../spa/src/program_knowledge_base/PKBQPSInterface.h"
#include "ManualASTBuilder.h"
#include "PKBSPStub.h"
#include "catch.hpp"

TEST_CASE("AST 1: Basic SPA, no nesting, while, if") {
  PKBSPStub pkb = PKBSPStub();
  std::shared_ptr<ProgramNode> ast = ManualASTBuilder::getAST_1();
  ExtractionController ec = ExtractionController(pkb);
  ec.ExecuteProgramExtraction(ast);
  SECTION("Follows extraction functionality") {
    REQUIRE(pkb.insertFollowsCallCount == 8);
  }
  SECTION("Parent extraction functionality") {
    REQUIRE(pkb.insertParentCallCount == 6);
  }
  SECTION("Uses extraction functionality") {
    REQUIRE(pkb.insertUsesCallCount == 21);
    // NOTE: this expected value includes all the duplicate calls
    // that may occur (handled by pkb)
  }
  SECTION("Modifies extraction functionality") {
    REQUIRE(pkb.insertModifiesCallCount == 17);
  }
  SECTION("Calls extraction functionality") {
    REQUIRE(pkb.insertCallsCallCount == 1);
  }
  SECTION("Entity extraction functionality") {
    REQUIRE(pkb.insertEntityCallCount == 15);
    REQUIRE(pkb.entitiesSet.size() == 22);
  }
  SECTION("Constant extraction functionality") {
    REQUIRE(pkb.insertConstantCallCount == 3);
  }
  SECTION("Variable extraction functionality") {
    REQUIRE(pkb.insertVariableCallCount == 14);
  }
  SECTION("Pattern extraction functionality") {
    REQUIRE(pkb.insertPatternCallCount == 2);
  }
  SECTION("CFG node insertion functionality") {
    REQUIRE(pkb.insertCFGCallCount == 13);
    REQUIRE(pkb.checkCFGNodeOutgoing("1", {"2"}));
    REQUIRE(pkb.checkCFGNodeIncoming("1", {}));
    REQUIRE(pkb.checkCFGNodeOutgoing("2", {"3"}));
    REQUIRE(pkb.checkCFGNodeIncoming("2", {"1"}));
    REQUIRE(pkb.checkCFGNodeOutgoing("3", {"4"}));
    REQUIRE(pkb.checkCFGNodeIncoming("3", {"2"}));
    REQUIRE(pkb.checkCFGNodeOutgoing("4", {"5"}));
    REQUIRE(pkb.checkCFGNodeIncoming("4", {"3"}));
     REQUIRE(pkb.checkCFGNodeOutgoing("5", {"6", "8"}));
    REQUIRE(pkb.checkCFGNodeIncoming("5", {"4", "7"}));
    REQUIRE(pkb.checkCFGNodeOutgoing("6", {"7"}));
    REQUIRE(pkb.checkCFGNodeIncoming("6", {"5"}));
    REQUIRE(pkb.checkCFGNodeOutgoing("7", {"5", "8"}));
    REQUIRE(pkb.checkCFGNodeIncoming("7", {"6"}));
    REQUIRE(pkb.checkCFGNodeOutgoing("8", {"9", "11"}));
    REQUIRE(pkb.checkCFGNodeIncoming("8", {"5", "7"}));
    REQUIRE(pkb.checkCFGNodeOutgoing("9", {"10"}));
    REQUIRE(pkb.checkCFGNodeIncoming("9", {"8"}));
    REQUIRE(pkb.checkCFGNodeOutgoing("10", {}));
    REQUIRE(pkb.checkCFGNodeIncoming("10", {"9"}));
    REQUIRE(pkb.checkCFGNodeOutgoing("11", {"12"}));
    REQUIRE(pkb.checkCFGNodeIncoming("11", {"8"}));
    REQUIRE(pkb.checkCFGNodeOutgoing("12", {}));
    REQUIRE(pkb.checkCFGNodeIncoming("12", {"11"}));
    REQUIRE(pkb.checkCFGNodeOutgoing("13", {}));
    REQUIRE(pkb.checkCFGNodeIncoming("13", {}));
  }
}

TEST_CASE("AST 2: Basic SPA, doubly nested while") {
  PKBSPStub pkb = PKBSPStub();
  std::shared_ptr<ProgramNode> ast = ManualASTBuilder::getAST_2();
  ExtractionController ec = ExtractionController(pkb);
  ec.ExecuteProgramExtraction(ast);
  SECTION("Follows extraction functionality") {
    REQUIRE(pkb.insertFollowsCallCount == 1);
  }
  SECTION("Parent extraction functionality") {
    REQUIRE(pkb.insertParentCallCount == 3);
  }
  SECTION("Uses extraction functionality") {
    REQUIRE(pkb.insertUsesCallCount == 9);
    // NOTE: the expected value 15 includes all the duplicate calls
    // that may occur (handled by pkb)
  }
  SECTION("Modifies extraction functionality") {
    REQUIRE(pkb.insertModifiesCallCount == 4);
  }
  SECTION("Calls extraction functionality") {
    REQUIRE(pkb.insertCallsCallCount == 0);
  }
  SECTION("Entity extraction functionality") {
    REQUIRE(pkb.insertEntityCallCount == 5);
  }
  SECTION("Constant extraction functionality") {
    REQUIRE(pkb.insertConstantCallCount == 2);
  }
  SECTION("Variable extraction functionality") {
    REQUIRE(pkb.insertVariableCallCount == 4);
  }
  SECTION("Pattern extraction functionality") {
    REQUIRE(pkb.insertPatternCallCount == 0);
  }
  SECTION("CFG node insertion functionality") {
    REQUIRE(pkb.insertCFGCallCount == 4);
    REQUIRE(pkb.checkCFGNodeIncoming("1", {"2", "4"}));
    REQUIRE(pkb.checkCFGNodeOutgoing("1", {"2"}));
    REQUIRE(pkb.checkCFGNodeIncoming("2", {"1", "4"}));
    REQUIRE(pkb.checkCFGNodeOutgoing("2", {"3", "1"}));
    REQUIRE(pkb.checkCFGNodeIncoming("3", {"2"}));
    REQUIRE(pkb.checkCFGNodeOutgoing("3", {"4"}));
    REQUIRE(pkb.checkCFGNodeIncoming("4", {"3"}));
    REQUIRE(pkb.checkCFGNodeOutgoing("4", {"2", "1"}));
  }
}

TEST_CASE("AST 3: Basic SPA, 2 procedures") {
  PKBSPStub pkb = PKBSPStub();
  std::shared_ptr<ProgramNode> ast = ManualASTBuilder::getAST_3();
  ExtractionController ec = ExtractionController(pkb);
  ec.ExecuteProgramExtraction(ast);
  SECTION("Follows extraction functionality") {
    REQUIRE(pkb.insertFollowsCallCount == 2);
  }
  SECTION("Parent extraction functionality") {
    REQUIRE(pkb.insertParentCallCount == 0);
  }
  SECTION("Uses extraction functionality") {
    REQUIRE(pkb.insertUsesCallCount == 4);
    // NOTE: the expected value 15 includes all the duplicate calls
    // that may occur (handled by pkb)
  }
  SECTION("Modifies extraction functionality") {
    REQUIRE(pkb.insertModifiesCallCount == 4);
  }
  SECTION("Calls extraction functionality") {
    REQUIRE(pkb.insertCallsCallCount == 0);
  }
  SECTION("Entity extraction functionality") {
    REQUIRE(pkb.insertEntityCallCount == 6);
  }
  SECTION("Constant extraction functionality") {
    REQUIRE(pkb.insertConstantCallCount == 0);
  }
  SECTION("Variable extraction functionality") {
    REQUIRE(pkb.insertVariableCallCount == 4);
  }
  SECTION("Pattern extraction functionality") {
    REQUIRE(pkb.insertPatternCallCount == 0);
  }
  SECTION("CFG node insertion functionality") {
    REQUIRE(pkb.insertCFGCallCount == 4);
    REQUIRE(pkb.checkCFGNodeIncoming("1", {}));
    REQUIRE(pkb.checkCFGNodeOutgoing("1", {"2"}));
    REQUIRE(pkb.checkCFGNodeIncoming("2", {"1"}));
    REQUIRE(pkb.checkCFGNodeOutgoing("2", {}));
    REQUIRE(pkb.checkCFGNodeIncoming("3", {}));
    REQUIRE(pkb.checkCFGNodeOutgoing("3", {"4"}));
    REQUIRE(pkb.checkCFGNodeIncoming("4", {"3"}));
    REQUIRE(pkb.checkCFGNodeOutgoing("4", {}));

  }
}

TEST_CASE(
    "AST 4: Basic SPA, doubly nested if (if-if) and triple nested while stmt "
    "(if-if-while)") {
  PKBSPStub pkb = PKBSPStub();
  std::shared_ptr<ProgramNode> ast = ManualASTBuilder::getAST_4();
  ExtractionController ec = ExtractionController(pkb);
  ec.ExecuteProgramExtraction(ast);
  SECTION("Follows extraction functionality") {
    REQUIRE(pkb.insertFollowsCallCount == 1);
  }
  SECTION("Parent extraction functionality") {
    REQUIRE(pkb.insertParentCallCount == 6);
  }
  SECTION("Uses extraction functionality") {
    REQUIRE(pkb.insertUsesCallCount == 23);
    // NOTE: the expected value 15 includes all the duplicate calls
    // that may occur (handled by pkb)
  }
  SECTION("Modifies extraction functionality") {
    REQUIRE(pkb.insertModifiesCallCount == 8);
  }
  SECTION("Calls extraction functionality") {
    REQUIRE(pkb.insertCallsCallCount == 1);
  }
  SECTION("Entity extraction functionality") {
    REQUIRE(pkb.insertEntityCallCount == 10);
  }
  SECTION("Constant extraction functionality") {
    REQUIRE(pkb.insertConstantCallCount == 3);
  }
  SECTION("Variable extraction functionality") {
    REQUIRE(pkb.insertVariableCallCount == 8);
  }
  SECTION("Pattern extraction functionality") {
    REQUIRE(pkb.insertPatternCallCount == 1);
  }
  SECTION("CFG node insertion functionality") {
    REQUIRE(pkb.insertCFGCallCount == 8);
    REQUIRE(pkb.checkCFGNodeIncoming("1", {}));
    REQUIRE(pkb.checkCFGNodeOutgoing("1", {"2", "7"}));
    REQUIRE(pkb.checkCFGNodeIncoming("2", {"1"}));
    REQUIRE(pkb.checkCFGNodeOutgoing("2", {"3"}));
    REQUIRE(pkb.checkCFGNodeIncoming("3", {"2"}));
    REQUIRE(pkb.checkCFGNodeOutgoing("3", {"4", "5"}));
    REQUIRE(pkb.checkCFGNodeIncoming("4", {"3"}));
    REQUIRE(pkb.checkCFGNodeOutgoing("4", {}));
    REQUIRE(pkb.checkCFGNodeIncoming("5", {"3", "6"}));
    REQUIRE(pkb.checkCFGNodeOutgoing("5", {"6"}));
    REQUIRE(pkb.checkCFGNodeIncoming("6", {"5"}));
    REQUIRE(pkb.checkCFGNodeOutgoing("6", {"5"}));
    REQUIRE(pkb.checkCFGNodeIncoming("7", {"1"}));
    REQUIRE(pkb.checkCFGNodeOutgoing("7", {}));
    REQUIRE(pkb.checkCFGNodeIncoming("8", {}));
    REQUIRE(pkb.checkCFGNodeOutgoing("8", {}));
  }
}

// Hardcoded pkb stub returns not really working for this
TEST_CASE("AST 5: Three procedures, nested calls") {
  PKBSPStub pkb = PKBSPStub();
  std::shared_ptr<ProgramNode> ast = ManualASTBuilder::getAST_5();
  ExtractionController ec = ExtractionController(pkb);
  ec.ExecuteProgramExtraction(ast);
  SECTION("Follows extraction functionality") {
    REQUIRE(pkb.insertFollowsCallCount == 0);
  }
  SECTION("Parent extraction functionality") {
    REQUIRE(pkb.insertParentCallCount == 1);
  }
  // Hardcoded pkb stub returns not really working for this
  SECTION("Uses extraction functionality") {
    REQUIRE(pkb.insertUsesCallCount == 4);
    // NOTE: the expected value 15 includes all the duplicate calls
    // that may occur (handled by pkb)
  }
  // Hardcoded pkb stub returns not really working for this
  SECTION("Modifies extraction functionality") {
    REQUIRE(pkb.insertModifiesCallCount == 2);
  }
  SECTION("Calls extraction functionality") {
    REQUIRE(pkb.insertCallsCallCount == 2);
  }
  SECTION("Entity extraction functionality") {
    REQUIRE(pkb.insertEntityCallCount == 7);
  }
  SECTION("Constant extraction functionality") {
    REQUIRE(pkb.insertConstantCallCount == 1);
  }
  SECTION("Variable extraction functionality") {
    REQUIRE(pkb.insertVariableCallCount == 3);
  }
  SECTION("Pattern extraction functionality") {
    REQUIRE(pkb.insertPatternCallCount == 1);
  }
  SECTION("CFG node insertion functionality") {
    REQUIRE(pkb.insertCFGCallCount == 4);
    REQUIRE(pkb.checkCFGNodeIncoming("1", {}));
    REQUIRE(pkb.checkCFGNodeOutgoing("1", {}));
    REQUIRE(pkb.checkCFGNodeIncoming("2", {"3"}));
    REQUIRE(pkb.checkCFGNodeOutgoing("2", {"3"}));
    REQUIRE(pkb.checkCFGNodeIncoming("3", {"2"}));
    REQUIRE(pkb.checkCFGNodeOutgoing("3", {"2"}));
    REQUIRE(pkb.checkCFGNodeIncoming("4", {}));
    REQUIRE(pkb.checkCFGNodeOutgoing("4", {}));
  }
}