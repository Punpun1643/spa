#include <source_processor/design_extractor/IExtractionController.h>
#include <source_processor/node/ProcedureNode.h>
#include <source_processor/node/ProgramNode.h>
#include <source_processor/node/StmtLstNode.h>
#include <source_processor/node/stmt_node/PrintNode.h>

#include <iostream>
#include <source_processor/design_extractor/ExtractionController.cpp>

#include "../../spa/src/program_knowledge_base/PkbApi.h"
#include "ManualASTBuilder.h"
#include "PkbStub.h"
#include "catch.hpp"

TEST_CASE("AST 1: Basic SPA, no nesting, while, if") {
  PkbStub pkb = PkbStub();
  std::shared_ptr<ProgramNode> ast = ManualASTBuilder::getAST_1();
  ExtractionController ec = ExtractionController(pkb);
  ec.executeProgramExtraction(ast);
  SECTION("Follows extraction functionality") {
    REQUIRE(pkb.insertFollowsCallCount == 8);
  }
  SECTION("Parent extraction functionality") {
    REQUIRE(pkb.insertParentCallCount == 6);
  }
  SECTION("Uses extraction functionality") {
    REQUIRE(pkb.insertUsesCallCount == 23);
    // NOTE: this expected value includes all the duplicate calls
    // that may occur (handled by pkb)
  }
  SECTION("Modifies extraction functionality") {
    REQUIRE(pkb.insertModifiesCallCount == 19);
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
}

TEST_CASE("AST 2: Basic SPA, doubly nested while") {
  PkbStub pkb = PkbStub();
  std::shared_ptr<ProgramNode> ast = ManualASTBuilder::getAST_2();
  ExtractionController ec = ExtractionController(pkb);
  ec.executeProgramExtraction(ast);
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
}

TEST_CASE("AST 3: Basic SPA, 2 procedures") {
  PkbStub pkb = PkbStub();
  std::shared_ptr<ProgramNode> ast = ManualASTBuilder::getAST_3();
  ExtractionController ec = ExtractionController(pkb);
  ec.executeProgramExtraction(ast);
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
}

TEST_CASE(
    "AST 4: Basic SPA, doubly nested if (if-if) and triple nested while stmt "
    "(if-if-while)") {
  PkbStub pkb = PkbStub();
  std::shared_ptr<ProgramNode> ast = ManualASTBuilder::getAST_4();
  ExtractionController ec = ExtractionController(pkb);
  ec.executeProgramExtraction(ast);
  SECTION("Follows extraction functionality") {
    REQUIRE(pkb.insertFollowsCallCount == 1);
  }
  SECTION("Parent extraction functionality") {
    REQUIRE(pkb.insertParentCallCount == 6);
  }
  SECTION("Uses extraction functionality") {
    REQUIRE(pkb.insertUsesCallCount == 26);
    // NOTE: the expected value 15 includes all the duplicate calls
    // that may occur (handled by pkb)
  }
  SECTION("Modifies extraction functionality") {
    REQUIRE(pkb.insertModifiesCallCount == 11);
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
}

// Hardcoded pkb stub returns not really working for this
TEST_CASE(
    "AST 5: Three procedures, nested calls") {
  PkbStub pkb = PkbStub();
  std::shared_ptr<ProgramNode> ast = ManualASTBuilder::getAST_5();
  ExtractionController ec = ExtractionController(pkb);
  ec.executeProgramExtraction(ast);
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
}