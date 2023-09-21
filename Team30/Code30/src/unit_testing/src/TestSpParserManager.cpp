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
    auto constants1 = assignNode1->getConstants();
    REQUIRE(constants1.find(2) != constants1.end());

    REQUIRE(assignNode2->getStmtType() == StmtType::ASSIGN_STMT);
    REQUIRE(assignNode2->getStmtIndex() == 2);
    REQUIRE(assignNode2->getVarName() == "z");
    REQUIRE(assignNode2->getVariables().size() == 0);
    REQUIRE(assignNode2->getConstants().size() == 1);
    auto constants2 = assignNode2->getConstants();
    REQUIRE(constants2.find(3) != constants2.end());

    REQUIRE(assignNode3->getStmtType() == StmtType::ASSIGN_STMT);
    REQUIRE(assignNode3->getStmtIndex() == 3);
    REQUIRE(assignNode3->getVarName() == "i");
    REQUIRE(assignNode3->getVariables().size() == 0);
    REQUIRE(assignNode3->getConstants().size() == 1);
    auto constants3 = assignNode3->getConstants();
    REQUIRE(constants3.find(5) != constants3.end());

    REQUIRE(whileNode->getStmtType() == StmtType::WHILE_STMT);
    REQUIRE(whileNode->getStmtIndex() == 4);

    auto whileCondExpr = whileNode->getCondExpr();
    auto whileStmtLst = whileNode->getStmtLst();
    auto whileCondExprVariables = whileCondExpr->getVariables();
    auto whileCondExprConstants = whileCondExpr->getConstants();
    REQUIRE(whileCondExprVariables.size() == 1);
    REQUIRE(whileCondExprConstants.size() == 1);

    auto whileStmtAssign1 = std::dynamic_pointer_cast<AssignNode>(
        whileStmtLst->getChildren().at(0));
    auto whileIfStmt =
        std::dynamic_pointer_cast<IfNode>(whileStmtLst->getChildren().at(1));
    auto whileStmtAssign2 = std::dynamic_pointer_cast<AssignNode>(
        whileStmtLst->getChildren().at(2));
    auto whileCallStmt =
        std::dynamic_pointer_cast<CallNode>(whileStmtLst->getChildren().at(3));
    auto whileStmtAssign3 = std::dynamic_pointer_cast<AssignNode>(
        whileStmtLst->getChildren().at(4));

    REQUIRE(whileCondExprVariables.find("i") != whileCondExprVariables.end());
    REQUIRE(whileCondExprConstants.find(0) != whileCondExprConstants.end());

    REQUIRE(whileStmtAssign1->getStmtType() == StmtType::ASSIGN_STMT);
    REQUIRE(whileStmtAssign1->getStmtIndex() == 5);
    REQUIRE(whileStmtAssign1->getVarName() == "x");
    REQUIRE(whileStmtAssign1->getVariables().size() == 1);
    REQUIRE(whileStmtAssign1->getConstants().size() == 1);

    auto whileStmtAssign1ExprTreeRoot = whileStmtAssign1->getRootOfTree();
    REQUIRE(whileStmtAssign1ExprTreeRoot->getVal() == "-");
    REQUIRE(whileStmtAssign1ExprTreeRoot->getLeftSubTree()->getVal() == "x");
    REQUIRE(whileStmtAssign1ExprTreeRoot->getRightSubTree()->getVal() == "1");

    REQUIRE(whileIfStmt->getStmtType() == StmtType::IF_STMT);
    REQUIRE(whileIfStmt->getStmtIndex() == 6);

    auto whileIfStmtCondExpr = whileIfStmt->getCondExpr();

    REQUIRE(whileIfStmtCondExpr->getVariables().size() == 1);
    REQUIRE(whileIfStmtCondExpr->getConstants().size() == 1);
    REQUIRE(whileIfStmtCondExpr->getVariables().find("x") !=
            whileIfStmtCondExpr->getVariables().end());
    REQUIRE(whileIfStmtCondExpr->getConstants().find(1) !=
            whileIfStmtCondExpr->getConstants().end());

    auto whileIfStmtThenStmtLst = whileIfStmt->getThenStmtLst();
    auto whileIfStmtElseStmtLst = whileIfStmt->getElseStmtLst();

    REQUIRE(whileIfStmtThenStmtLst->getChildren().size() == 1);
    REQUIRE(whileIfStmtElseStmtLst->getChildren().size() == 1);

    auto whileIfStmtThenStmtAssign = std::dynamic_pointer_cast<AssignNode>(
        whileIfStmtThenStmtLst->getChildren().at(0));
    auto whileIfStmtElseStmtAssign = std::dynamic_pointer_cast<AssignNode>(
        whileIfStmtElseStmtLst->getChildren().at(0));

    REQUIRE(whileIfStmtThenStmtAssign->getStmtType() == StmtType::ASSIGN_STMT);
    REQUIRE(whileIfStmtThenStmtAssign->getStmtIndex() == 7);
    REQUIRE(whileIfStmtThenStmtAssign->getVarName() == "z");
    REQUIRE(whileIfStmtThenStmtAssign->getVariables().size() == 1);
    REQUIRE(whileIfStmtThenStmtAssign->getConstants().size() == 1);
    REQUIRE(whileIfStmtThenStmtAssign->getVariables().find("x") !=
            whileIfStmtThenStmtAssign->getVariables().end());
    REQUIRE(whileIfStmtThenStmtAssign->getConstants().find(1) !=
            whileIfStmtThenStmtAssign->getConstants().end());

    REQUIRE(whileIfStmtElseStmtAssign->getStmtType() == StmtType::ASSIGN_STMT);
    REQUIRE(whileIfStmtElseStmtAssign->getStmtIndex() == 8);
    REQUIRE(whileIfStmtElseStmtAssign->getVarName() == "y");
    REQUIRE(whileIfStmtElseStmtAssign->getVariables().size() == 2);
    REQUIRE(whileIfStmtElseStmtAssign->getConstants().size() == 0);
    REQUIRE(whileIfStmtElseStmtAssign->getVariables().find("z") !=
            whileIfStmtElseStmtAssign->getVariables().end());
    REQUIRE(whileIfStmtElseStmtAssign->getVariables().find("x") !=
            whileIfStmtElseStmtAssign->getVariables().end());

    REQUIRE(whileStmtAssign2->getStmtType() == StmtType::ASSIGN_STMT);
    REQUIRE(whileStmtAssign2->getStmtIndex() == 9);
    REQUIRE(whileStmtAssign2->getVarName() == "z");
    REQUIRE(whileStmtAssign2->getVariables().size() == 3);
    REQUIRE(whileStmtAssign2->getConstants().size() == 0);
    REQUIRE(whileStmtAssign2->getVariables().find("z") !=
            whileStmtAssign2->getVariables().end());
    REQUIRE(whileStmtAssign2->getVariables().find("x") !=
            whileStmtAssign2->getVariables().end());
    REQUIRE(whileStmtAssign2->getVariables().find("i") !=
            whileStmtAssign2->getVariables().end());

    REQUIRE(whileCallStmt->getStmtType() == StmtType::CALL_STMT);
    REQUIRE(whileCallStmt->getStmtIndex() == 10);
    REQUIRE(whileCallStmt->getProcName() == "q");

    REQUIRE(whileStmtAssign3->getStmtType() == StmtType::ASSIGN_STMT);
    REQUIRE(whileStmtAssign3->getStmtIndex() == 11);
    REQUIRE(whileStmtAssign3->getVarName() == "i");
    REQUIRE(whileStmtAssign3->getVariables().size() == 1);
    REQUIRE(whileStmtAssign3->getConstants().size() == 1);
    REQUIRE(whileStmtAssign3->getVariables().find("i") !=
            whileStmtAssign3->getVariables().end());
    REQUIRE(whileStmtAssign3->getConstants().find(1) !=
            whileStmtAssign3->getConstants().end());

    REQUIRE(callNode->getStmtType() == StmtType::CALL_STMT);
    REQUIRE(callNode->getStmtIndex() == 12);
    REQUIRE(callNode->getProcName() == "p");

    // stmt: procedure p
    REQUIRE(stmtLstNode2->getChildren().size() == 2);
    auto pIfNode1 =
        std::dynamic_pointer_cast<IfNode>(stmtLstNode2->getChildren().at(0));
    auto pAssignNode2 = std::dynamic_pointer_cast<AssignNode>(
        stmtLstNode2->getChildren().at(1));

    REQUIRE(pIfNode1->getStmtType() == StmtType::IF_STMT);
    REQUIRE(pIfNode1->getStmtIndex() == 13);

    auto pIfNode1CondExpr = pIfNode1->getCondExpr();
    auto pIfNode1ThenStmtLst = pIfNode1->getThenStmtLst();
    auto pIfNode1ElseStmtLst = pIfNode1->getElseStmtLst();

    REQUIRE(pIfNode1CondExpr->getVariables().size() == 1);
    REQUIRE(pIfNode1CondExpr->getConstants().size() == 1);
    REQUIRE(pIfNode1CondExpr->getVariables().find("x") !=
            pIfNode1CondExpr->getVariables().end());
    REQUIRE(pIfNode1CondExpr->getConstants().find(0) !=
            pIfNode1CondExpr->getConstants().end());

    // stmt: procedure q
    REQUIRE(stmtLstNode3->getChildren().size() == 1);
  }
}
