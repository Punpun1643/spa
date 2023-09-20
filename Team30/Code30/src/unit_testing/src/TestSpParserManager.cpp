#include <string>

#include "../../spa/src/source_processor/node/StmtLstNode.h"
#include "../../spa/src/source_processor/node/stmt_node/AssignNode.h"
#include "../../spa/src/source_processor/node/stmt_node/CallNode.h"
#include "../../spa/src/source_processor/node/stmt_node/IfNode.h"
#include "../../spa/src/source_processor/node/stmt_node/PrintNode.h"
#include "../../spa/src/source_processor/node/stmt_node/ReadNode.h"
#include "../../spa/src/source_processor/node/stmt_node/StmtNode.h"
#include "../../spa/src/source_processor/node/stmt_node/StmtType.h"
#include "../../spa/src/source_processor/node/stmt_node/WhileNode.h"
#include "../../spa/src/source_processor/parser/SpParserManager.h"
#include "catch.hpp"

TEST_CASE(
    "Test SpParserManager parses short length source program successfully",
    "[parseInputFile, Sample_source_w4_short.txt]") {
  SECTION("Test file with valid path and valid source code") {
    SpParserManager spParserManager = SpParserManager();
    std::string filePath =
        "/Users/papattaradaapithanangsiri/23s1-cp-spa-team-30/Team30/Tests30/"
        "Sample_source_w4_short.txt";
    std::shared_ptr<ProgramNode> programNode =
        spParserManager.parseInputFile(filePath);
    std::shared_ptr<ProcedureNode> procedureNode =
        programNode->getChildren().at(0);
    std::shared_ptr<StmtLstNode> stmtLstNode =
        procedureNode->getChildren().at(0);
    std::vector<std::shared_ptr<StmtNode>> stmtNodes =
        stmtLstNode->getChildren();
    auto callNode = std::dynamic_pointer_cast<CallNode>(stmtNodes.at(0));
    auto readNode = std::dynamic_pointer_cast<ReadNode>(stmtNodes.at(1));
    auto printNode = std::dynamic_pointer_cast<PrintNode>(stmtNodes.at(2));

    REQUIRE(programNode->getChildren().size() == 1);
    REQUIRE(procedureNode->getProcedureName() == "Example");
    REQUIRE(stmtLstNode->getChildren().size() == 3);
    REQUIRE(stmtNodes.at(0)->getStmtType() == StmtType::CALL_STMT);
    REQUIRE(stmtNodes.at(1)->getStmtType() == StmtType::READ_STMT);
    REQUIRE(stmtNodes.at(2)->getStmtType() == StmtType::PRINT_STMT);
    REQUIRE(callNode->getProcName() == "c");
    REQUIRE(readNode->getVarName() == "r");
    REQUIRE(printNode->getVarName() == "p");
  }
}

TEST_CASE(
    "Test SpParserManager parses medium length source program successfully",
    "[parseInputFile, Sample_source_w4_medium.txt]") {
  SECTION(
      "Test file with valid path and valid source code for Example 1 and 2") {
    SpParserManager spParserManager = SpParserManager();
    std::string filePath =
        "/Users/papattaradaapithanangsiri/23s1-cp-spa-team-30/Team30/Tests30/"
        "Sample_source_w4_medium.txt";
    auto programNode = spParserManager.parseInputFile(filePath);

    REQUIRE(programNode->getChildren().size() == 3);

    // Test details for procedure Example 1
    {
      auto procedureNode = programNode->getChildren().at(0);
      auto stmtLstNode = procedureNode->getChildren().at(0);
      auto stmtNodes = stmtLstNode->getChildren();

      auto callNode = std::dynamic_pointer_cast<CallNode>(stmtNodes.at(0));
      auto readNode = std::dynamic_pointer_cast<ReadNode>(stmtNodes.at(1));
      auto printNode = std::dynamic_pointer_cast<PrintNode>(stmtNodes.at(2));

      REQUIRE(procedureNode->getProcedureName() == "Example1");
      REQUIRE(stmtLstNode->getChildren().size() == 3);
      REQUIRE(callNode->getProcName() == "Function1");
      REQUIRE(readNode->getVarName() == "userInput1");
      REQUIRE(printNode->getVarName() == "output1");
    }

    // Test details for procedure Example 2
    {
      auto procedureNode = programNode->getChildren().at(1);
      auto stmtLstNode = procedureNode->getChildren().at(0);
      auto stmtNodes = stmtLstNode->getChildren();

      auto callNode1 = std::dynamic_pointer_cast<CallNode>(stmtNodes.at(0));
      auto readNode = std::dynamic_pointer_cast<ReadNode>(stmtNodes.at(1));
      auto printNode = std::dynamic_pointer_cast<PrintNode>(stmtNodes.at(2));
      auto callNode2 = std::dynamic_pointer_cast<CallNode>(stmtNodes.at(3));

      REQUIRE(procedureNode->getProcedureName() == "Example2");
      REQUIRE(stmtLstNode->getChildren().size() == 4);
      REQUIRE(callNode1->getProcName() == "ExternalProc");
      REQUIRE(readNode->getVarName() == "configValue");
      REQUIRE(printNode->getVarName() == "statusMessage");
      REQUIRE(callNode2->getProcName() == "Cleanup");
    }

    // Test details for procedure Example 3
    {
      auto procedureNode = programNode->getChildren().at(2);
      auto stmtLstNode = procedureNode->getChildren().at(0);
      auto stmtNodes = stmtLstNode->getChildren();

      auto callNode1 = std::dynamic_pointer_cast<CallNode>(stmtNodes.at(0));
      auto callNode2 = std::dynamic_pointer_cast<CallNode>(stmtNodes.at(1));
      auto readNode1 = std::dynamic_pointer_cast<ReadNode>(stmtNodes.at(2));
      auto readNode2 = std::dynamic_pointer_cast<ReadNode>(stmtNodes.at(3));
      auto printNode1 = std::dynamic_pointer_cast<PrintNode>(stmtNodes.at(4));
      auto printNode2 = std::dynamic_pointer_cast<PrintNode>(stmtNodes.at(5));

      REQUIRE(procedureNode->getProcedureName() == "Example3");
      REQUIRE(stmtLstNode->getChildren().size() == 6);
      REQUIRE(callNode1->getProcName() == "Initialize");
      REQUIRE(callNode2->getProcName() == "Configure");
      REQUIRE(readNode1->getVarName() == "configA");
      REQUIRE(readNode2->getVarName() == "configB");
      REQUIRE(printNode1->getVarName() == "reportA");
      REQUIRE(printNode2->getVarName() == "reportB");
    }
  }
}

TEST_CASE("Test SpManager parses complete source file successfully",
          "[parseInputFile, Sample_source.txt]") {
  SECTION("Test file with valid path and valid source code") {
    SpParserManager spParserManager = SpParserManager();
    std::string filePath =
        "/Users/papattaradaapithanangsiri/23s1-cp-spa-team-30/Team30/Tests30/"
        "Sample_source.txt";
    auto programNode = spParserManager.parseInputFile(filePath);

    REQUIRE(programNode->getChildren().size() == 3);

    // procedure
    auto procedureNode1 = programNode->getChildren().at(0);
    auto procedureNode2 = programNode->getChildren().at(1);
    auto procedureNode3 = programNode->getChildren().at(2);

    REQUIRE(procedureNode1->getProcedureName() == "Example");
    REQUIRE(procedureNode2->getProcedureName() == "p");
    REQUIRE(procedureNode3->getProcedureName() == "q");

    // stmtLst
    auto stmtLstNode1 = procedureNode1->getChildren().at(0);
    auto stmtLstNode2 = procedureNode2->getChildren().at(0);
    auto stmtLstNode3 = procedureNode3->getChildren().at(0);

    // stmt: procedure Example
    REQUIRE(stmtLstNode1->getChildren().size() == 5);

    auto assignNode1 = std::dynamic_pointer_cast<AssignNode>(
        stmtLstNode1->getChildren().at(0));
    auto assignNode2 = std::dynamic_pointer_cast<AssignNode>(
        stmtLstNode1->getChildren().at(1));
    auto assignNode3 = std::dynamic_pointer_cast<AssignNode>(
        stmtLstNode1->getChildren().at(2));
    auto whileNode =
        std::dynamic_pointer_cast<WhileNode>(stmtLstNode1->getChildren().at(3));
    auto callNode =
        std::dynamic_pointer_cast<CallNode>(stmtLstNode1->getChildren().at(4));

    REQUIRE(assignNode1->getStmtType() == StmtType::ASSIGN_STMT);
    REQUIRE(assignNode1->getStmtIndex() == 1);
    REQUIRE(assignNode1->getVarName() == "x");
    REQUIRE(assignNode1->getVariables().size() == 0);
    REQUIRE(assignNode1->getConstants().size() == 1);
    auto variables = assignNode1->getVariables();
    auto constants = assignNode1->getConstants();
    REQUIRE(constants.find(2) != constants.end());

    // stmt: procedure p
    REQUIRE(stmtLstNode2->getChildren().size() == 2);

    // stmt: procedure q
    REQUIRE(stmtLstNode3->getChildren().size() == 1);
  }
}
