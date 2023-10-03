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

class ExtractorBuilder {
 public:
  std::shared_ptr<EntityExtractor> eExtractor;
  std::shared_ptr<FollowsExtractor> fExtractor;
  std::shared_ptr<ParentExtractor> pExtractor;
  std::shared_ptr<ModifiesExtractor> mExtractor;
  std::shared_ptr<UsesExtractor> uExtractor;

  PkbStub pkb;
  std::shared_ptr<CallsManager> callsManager;
  ExtractorBuilder(PkbStub& pkb, std::shared_ptr<CallsManager> callsManager) : pkb(pkb), callsManager(callsManager) {
    eExtractor = std::make_shared<EntityExtractor>(pkb);
    fExtractor = std::make_shared<FollowsExtractor>(pkb);
    pExtractor = std::make_shared<ParentExtractor>(pkb);
    mExtractor = std::make_shared<ModifiesExtractor>(pkb, callsManager);
    uExtractor = std::make_shared<UsesExtractor>(pkb, callsManager);
  }
};

TEST_CASE("AST 1: Basic SPA, no nesting, while, if") {
  PkbStub pkb = PkbStub();
  std::shared_ptr<CallsManager> callsManager = std::make_shared<CallsManager>();
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
    REQUIRE(pkb.insertUsesCallCount == 17);
    // NOTE: this expected value includes all the duplicate calls
    // that may occur (handled by pkb)
  }
  SECTION("Modifies extraction functionality") {
    REQUIRE(pkb.insertModifiesCallCount == 13);
  }
  SECTION("Entity extraction functionality") {
    REQUIRE(pkb.insertEntityCallCount == 13);
  }
  SECTION("Constant extraction functionality") {
    REQUIRE(pkb.insertConstantCallCount == 3);
  }
  SECTION("Variable extraction functionality") {
    REQUIRE(pkb.insertVariableCallCount == 12);
  }
  SECTION("Pattern extraction functionality") {
    REQUIRE(pkb.insertPatternCallCount == 1);
  }
}

TEST_CASE("AST 2: Basic SPA, doubly nested while") {
  PkbStub pkb = PkbStub();
  std::shared_ptr<CallsManager> callsManager = std::make_shared<CallsManager>();
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
  std::shared_ptr<CallsManager> callsManager = std::make_shared<CallsManager>();
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
  std::shared_ptr<CallsManager> callsManager = std::make_shared<CallsManager>();
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
    REQUIRE(pkb.insertUsesCallCount == 18);
    // NOTE: the expected value 15 includes all the duplicate calls
    // that may occur (handled by pkb)
  }
  SECTION("Modifies extraction functionality") {
    REQUIRE(pkb.insertModifiesCallCount == 3);
  }
  SECTION("Entity extraction functionality") {
    REQUIRE(pkb.insertEntityCallCount == 8);
  }
  SECTION("Constant extraction functionality") {
    REQUIRE(pkb.insertConstantCallCount == 3);
  }
  SECTION("Variable extraction functionality") {
    REQUIRE(pkb.insertVariableCallCount == 6);
  }
  SECTION("Pattern extraction functionality") {
    REQUIRE(pkb.insertPatternCallCount == 0);
  }
}