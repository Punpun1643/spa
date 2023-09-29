#include "../../spa/src/shared/tokenizer/token/EofToken.h"
#include "../../spa/src/shared/tokenizer/token/IntegerToken.h"
#include "../../spa/src/shared/tokenizer/token/SpecialCharToken.h"
#include "../../spa/src/shared/tokenizer/token/Token.h"
#include "../../spa/src/shared/tokenizer/token/WordToken.h"
#include "../../spa/src/source_processor/node/stmt_node/StmtType.h"
#include "../../spa/src/source_processor/parser/SpParser.h"
#include "catch.hpp"

TEST_CASE("Parser parseStmtLst print stmts") {
  std::vector<std::shared_ptr<Token>> tokens;

  SECTION("StmtLst with no statement return empty vector") {
    tokens.push_back(std::static_pointer_cast<Token>(
        std::make_shared<SpecialCharToken>("}")));
    tokens.push_back(
        std::static_pointer_cast<Token>(std::make_shared<EofToken>()));

    SpParser parser = SpParser(tokens);
    REQUIRE_THROWS(parser.parseStmtLst());
  }

  SECTION("StmtLst with one statement return one statement node") {
    tokens.push_back(
        std::static_pointer_cast<Token>(std::make_shared<WordToken>("print")));
    tokens.push_back(std::static_pointer_cast<Token>(
        std::make_shared<WordToken>("variable")));
    tokens.push_back(std::static_pointer_cast<Token>(
        std::make_shared<SpecialCharToken>(";")));
    tokens.push_back(std::static_pointer_cast<Token>(
        std::make_shared<SpecialCharToken>("}")));
    tokens.push_back(
        std::static_pointer_cast<Token>(std::make_shared<EofToken>()));

    SpParser parser = SpParser(tokens);
    auto stmtLstNode = parser.parseStmtLst();
    auto printNode =
        std::dynamic_pointer_cast<PrintNode>(stmtLstNode->getChildren().at(0));

    REQUIRE(stmtLstNode != nullptr);
    REQUIRE(stmtLstNode->getChildren().size() == 1);
    REQUIRE(printNode != nullptr);
    REQUIRE(printNode->getVarName() == "variable");
  }

  SECTION("StmtLst with two statement return two statement nodes") {
    tokens.push_back(
        std::static_pointer_cast<Token>(std::make_shared<WordToken>("print")));
    tokens.push_back(std::static_pointer_cast<Token>(
        std::make_shared<WordToken>("variable1")));
    tokens.push_back(std::static_pointer_cast<Token>(
        std::make_shared<SpecialCharToken>(";")));
    tokens.push_back(
        std::static_pointer_cast<Token>(std::make_shared<WordToken>("print")));
    tokens.push_back(std::static_pointer_cast<Token>(
        std::make_shared<WordToken>("variable2")));
    tokens.push_back(std::static_pointer_cast<Token>(
        std::make_shared<SpecialCharToken>(";")));
    tokens.push_back(std::static_pointer_cast<Token>(
        std::make_shared<SpecialCharToken>("}")));
    tokens.push_back(
        std::static_pointer_cast<Token>(std::make_shared<EofToken>()));

    SpParser parser = SpParser(tokens);
    auto stmtLstNode = parser.parseStmtLst();
    auto printNodeOne =
        std::dynamic_pointer_cast<PrintNode>(stmtLstNode->getChildren().at(0));
    auto printNodeTwo =
        std::dynamic_pointer_cast<PrintNode>(stmtLstNode->getChildren().at(1));

    REQUIRE(stmtLstNode != nullptr);
    REQUIRE(stmtLstNode->getChildren().size() == 2);
    REQUIRE(printNodeOne != nullptr);
    REQUIRE(printNodeOne->getVarName() == "variable1");
    REQUIRE(printNodeTwo != nullptr);
    REQUIRE(printNodeTwo->getVarName() == "variable2");
  }

  SECTION("StmtLst with invalid stmt missing ;") {
    tokens.push_back(
        std::static_pointer_cast<Token>(std::make_shared<WordToken>("print")));
    tokens.push_back(std::static_pointer_cast<Token>(
        std::make_shared<WordToken>("variable1")));
    tokens.push_back(std::static_pointer_cast<Token>(
        std::make_shared<SpecialCharToken>("}")));
    tokens.push_back(
        std::static_pointer_cast<Token>(std::make_shared<EofToken>()));

    SpParser parser = SpParser(tokens);
    REQUIRE_THROWS_AS(parser.parseStmtLst(), std::invalid_argument);
  }

  SECTION("StmtLst with invalid stmt missing variable name") {
    tokens.push_back(
        std::static_pointer_cast<Token>(std::make_shared<WordToken>("print")));
    tokens.push_back(std::static_pointer_cast<Token>(
        std::make_shared<SpecialCharToken>(";")));
    tokens.push_back(std::static_pointer_cast<Token>(
        std::make_shared<SpecialCharToken>("}")));
    tokens.push_back(
        std::static_pointer_cast<Token>(std::make_shared<EofToken>()));

    SpParser parser = SpParser(tokens);
    REQUIRE_THROWS_AS(parser.parseStmtLst(), std::invalid_argument);
  }

  SECTION(
      "StmtLst with ine invalid stmt missing variable name and one valid "
      "stmt") {
    tokens.push_back(
        std::static_pointer_cast<Token>(std::make_shared<WordToken>("print")));
    tokens.push_back(std::static_pointer_cast<Token>(
        std::make_shared<WordToken>("variable1")));
    tokens.push_back(std::static_pointer_cast<Token>(
        std::make_shared<SpecialCharToken>(";")));
    tokens.push_back(
        std::static_pointer_cast<Token>(std::make_shared<WordToken>("print")));
    tokens.push_back(std::static_pointer_cast<Token>(
        std::make_shared<SpecialCharToken>(";")));
    tokens.push_back(std::static_pointer_cast<Token>(
        std::make_shared<SpecialCharToken>("}")));
    tokens.push_back(
        std::static_pointer_cast<Token>(std::make_shared<EofToken>()));

    SpParser parser = SpParser(tokens);
    REQUIRE_THROWS_AS(parser.parseStmtLst(), std::invalid_argument);
  }
}

TEST_CASE("Parser parseStmtLst read stmts") {
  std::vector<std::shared_ptr<Token>> tokens;

  SECTION("StmtLst with one statement return one statement node") {
    tokens.push_back(
        std::static_pointer_cast<Token>(std::make_shared<WordToken>("read")));
    tokens.push_back(std::static_pointer_cast<Token>(
        std::make_shared<WordToken>("variable")));
    tokens.push_back(std::static_pointer_cast<Token>(
        std::make_shared<SpecialCharToken>(";")));
    tokens.push_back(std::static_pointer_cast<Token>(
        std::make_shared<SpecialCharToken>("}")));
    tokens.push_back(
        std::static_pointer_cast<Token>(std::make_shared<EofToken>()));

    SpParser parser = SpParser(tokens);
    auto stmtLstNode = parser.parseStmtLst();
    auto printNode =
        std::dynamic_pointer_cast<ReadNode>(stmtLstNode->getChildren().at(0));

    REQUIRE(stmtLstNode != nullptr);
    REQUIRE(stmtLstNode->getChildren().size() == 1);
    REQUIRE(printNode != nullptr);
    REQUIRE(printNode->getVarName() == "variable");
  }

  SECTION(
      "StmtLst with one read stmt and one print stmt return two statement "
      "nodes") {
    tokens.push_back(
        std::static_pointer_cast<Token>(std::make_shared<WordToken>("read")));
    tokens.push_back(std::static_pointer_cast<Token>(
        std::make_shared<WordToken>("variable1")));
    tokens.push_back(std::static_pointer_cast<Token>(
        std::make_shared<SpecialCharToken>(";")));
    tokens.push_back(
        std::static_pointer_cast<Token>(std::make_shared<WordToken>("print")));
    tokens.push_back(std::static_pointer_cast<Token>(
        std::make_shared<WordToken>("variable2")));
    tokens.push_back(std::static_pointer_cast<Token>(
        std::make_shared<SpecialCharToken>(";")));
    tokens.push_back(std::static_pointer_cast<Token>(
        std::make_shared<SpecialCharToken>("}")));
    tokens.push_back(
        std::static_pointer_cast<Token>(std::make_shared<EofToken>()));

    SpParser parser = SpParser(tokens);
    auto stmtLstNode = parser.parseStmtLst();
    auto readNode =
        std::dynamic_pointer_cast<ReadNode>(stmtLstNode->getChildren().at(0));
    auto printNode =
        std::dynamic_pointer_cast<PrintNode>(stmtLstNode->getChildren().at(1));

    REQUIRE(stmtLstNode != nullptr);
    REQUIRE(stmtLstNode->getChildren().size() == 2);
    REQUIRE(readNode != nullptr);
    REQUIRE(readNode->getStmtType() == StmtType::READ_STMT);
    REQUIRE(readNode->getVarName() == "variable1");
    REQUIRE(printNode != nullptr);
    REQUIRE(printNode->getStmtType() == StmtType::PRINT_STMT);
    REQUIRE(printNode->getVarName() == "variable2");
  }

  SECTION("StmtLst with invalid stmt missing ;") {
    tokens.push_back(
        std::static_pointer_cast<Token>(std::make_shared<WordToken>("read")));
    tokens.push_back(std::static_pointer_cast<Token>(
        std::make_shared<WordToken>("variable1")));
    tokens.push_back(std::static_pointer_cast<Token>(
        std::make_shared<SpecialCharToken>("}")));
    tokens.push_back(
        std::static_pointer_cast<Token>(std::make_shared<EofToken>()));

    SpParser parser = SpParser(tokens);
    REQUIRE_THROWS_AS(parser.parseStmtLst(), std::invalid_argument);
  }

  SECTION("StmtLst with invalid stmt missing variable name") {
    tokens.push_back(
        std::static_pointer_cast<Token>(std::make_shared<WordToken>("read")));
    tokens.push_back(std::static_pointer_cast<Token>(
        std::make_shared<SpecialCharToken>(";")));
    tokens.push_back(std::static_pointer_cast<Token>(
        std::make_shared<SpecialCharToken>("}")));
    tokens.push_back(
        std::static_pointer_cast<Token>(std::make_shared<EofToken>()));

    SpParser parser = SpParser(tokens);
    REQUIRE_THROWS_AS(parser.parseStmtLst(), std::invalid_argument);
  }

  SECTION(
      "StmtLst with ine invalid stmt missing variable name and one valid "
      "stmt") {
    tokens.push_back(
        std::static_pointer_cast<Token>(std::make_shared<WordToken>("read")));
    tokens.push_back(std::static_pointer_cast<Token>(
        std::make_shared<WordToken>("variable1")));
    tokens.push_back(std::static_pointer_cast<Token>(
        std::make_shared<SpecialCharToken>(";")));
    tokens.push_back(
        std::static_pointer_cast<Token>(std::make_shared<WordToken>("read")));
    tokens.push_back(std::static_pointer_cast<Token>(
        std::make_shared<SpecialCharToken>(";")));
    tokens.push_back(std::static_pointer_cast<Token>(
        std::make_shared<SpecialCharToken>("}")));
    tokens.push_back(
        std::static_pointer_cast<Token>(std::make_shared<EofToken>()));

    SpParser parser = SpParser(tokens);
    REQUIRE_THROWS_AS(parser.parseStmtLst(), std::invalid_argument);
  }
}

TEST_CASE("Parser parseStmtLst call stmts") {
  std::vector<std::shared_ptr<Token>> tokens;

  SECTION("StmtLst with one statement return one statement node") {
    tokens.push_back(
        std::static_pointer_cast<Token>(std::make_shared<WordToken>("call")));
    tokens.push_back(std::static_pointer_cast<Token>(
        std::make_shared<WordToken>("variable")));
    tokens.push_back(std::static_pointer_cast<Token>(
        std::make_shared<SpecialCharToken>(";")));
    tokens.push_back(std::static_pointer_cast<Token>(
        std::make_shared<SpecialCharToken>("}")));
    tokens.push_back(
        std::static_pointer_cast<Token>(std::make_shared<EofToken>()));

    SpParser parser = SpParser(tokens);
    auto stmtLstNode = parser.parseStmtLst();
    auto callNode =
        std::dynamic_pointer_cast<CallNode>(stmtLstNode->getChildren().at(0));

    REQUIRE(stmtLstNode != nullptr);
    REQUIRE(stmtLstNode->getChildren().size() == 1);
    REQUIRE(callNode != nullptr);
    REQUIRE(callNode->getProcName() == "variable");
  }

  SECTION(
      "StmtLst with one call stmt and one read stmt return two statement "
      "nodes") {
    tokens.push_back(
        std::static_pointer_cast<Token>(std::make_shared<WordToken>("call")));
    tokens.push_back(std::static_pointer_cast<Token>(
        std::make_shared<WordToken>("variable1")));
    tokens.push_back(std::static_pointer_cast<Token>(
        std::make_shared<SpecialCharToken>(";")));
    tokens.push_back(
        std::static_pointer_cast<Token>(std::make_shared<WordToken>("read")));
    tokens.push_back(std::static_pointer_cast<Token>(
        std::make_shared<WordToken>("variable2")));
    tokens.push_back(std::static_pointer_cast<Token>(
        std::make_shared<SpecialCharToken>(";")));
    tokens.push_back(std::static_pointer_cast<Token>(
        std::make_shared<SpecialCharToken>("}")));
    tokens.push_back(
        std::static_pointer_cast<Token>(std::make_shared<EofToken>()));

    SpParser parser = SpParser(tokens);
    auto stmtLstNode = parser.parseStmtLst();
    auto callNode =
        std::dynamic_pointer_cast<CallNode>(stmtLstNode->getChildren().at(0));
    auto readNode =
        std::dynamic_pointer_cast<ReadNode>(stmtLstNode->getChildren().at(1));

    REQUIRE(stmtLstNode != nullptr);
    REQUIRE(stmtLstNode->getChildren().size() == 2);
    REQUIRE(callNode != nullptr);
    REQUIRE(callNode->getStmtType() == StmtType::CALL_STMT);
    REQUIRE(callNode->getProcName() == "variable1");
    REQUIRE(readNode != nullptr);
    REQUIRE(readNode->getStmtType() == StmtType::READ_STMT);
    REQUIRE(readNode->getVarName() == "variable2");
  }

  SECTION("StmtLst with invalid stmt missing ;") {
    tokens.push_back(
        std::static_pointer_cast<Token>(std::make_shared<WordToken>("call")));
    tokens.push_back(std::static_pointer_cast<Token>(
        std::make_shared<WordToken>("variable1")));
    tokens.push_back(std::static_pointer_cast<Token>(
        std::make_shared<SpecialCharToken>("}")));
    tokens.push_back(
        std::static_pointer_cast<Token>(std::make_shared<EofToken>()));

    SpParser parser = SpParser(tokens);
    REQUIRE_THROWS_AS(parser.parseStmtLst(), std::invalid_argument);
  }

  SECTION("StmtLst with invalid stmt missing variable name") {
    tokens.push_back(
        std::static_pointer_cast<Token>(std::make_shared<WordToken>("call")));
    tokens.push_back(std::static_pointer_cast<Token>(
        std::make_shared<SpecialCharToken>(";")));
    tokens.push_back(std::static_pointer_cast<Token>(
        std::make_shared<SpecialCharToken>("}")));
    tokens.push_back(
        std::static_pointer_cast<Token>(std::make_shared<EofToken>()));

    SpParser parser = SpParser(tokens);
    REQUIRE_THROWS_AS(parser.parseStmtLst(), std::invalid_argument);
  }

  SECTION(
      "StmtLst with ine invalid stmt missing variable name and one valid "
      "stmt") {
    tokens.push_back(
        std::static_pointer_cast<Token>(std::make_shared<WordToken>("call")));
    tokens.push_back(std::static_pointer_cast<Token>(
        std::make_shared<WordToken>("variable1")));
    tokens.push_back(std::static_pointer_cast<Token>(
        std::make_shared<SpecialCharToken>(";")));
    tokens.push_back(
        std::static_pointer_cast<Token>(std::make_shared<WordToken>("call")));
    tokens.push_back(std::static_pointer_cast<Token>(
        std::make_shared<SpecialCharToken>(";")));
    tokens.push_back(std::static_pointer_cast<Token>(
        std::make_shared<SpecialCharToken>("}")));
    tokens.push_back(
        std::static_pointer_cast<Token>(std::make_shared<EofToken>()));

    SpParser parser = SpParser(tokens);
    REQUIRE_THROWS_AS(parser.parseStmtLst(), std::invalid_argument);
  }
}

TEST_CASE("Parser parseStmtLst stmtIndexing") {
  std::vector<std::shared_ptr<Token>> tokens;

  SECTION("StmtLst with one statement the stmt should have stmtIndex 1") {
    tokens.push_back(
        std::static_pointer_cast<Token>(std::make_shared<WordToken>("print")));
    tokens.push_back(std::static_pointer_cast<Token>(
        std::make_shared<WordToken>("variable")));
    tokens.push_back(std::static_pointer_cast<Token>(
        std::make_shared<SpecialCharToken>(";")));
    tokens.push_back(std::static_pointer_cast<Token>(
        std::make_shared<SpecialCharToken>("}")));
    tokens.push_back(
        std::static_pointer_cast<Token>(std::make_shared<EofToken>()));

    SpParser parser = SpParser(tokens);
    auto stmtLstNode = parser.parseStmtLst();
    auto printNode =
        std::dynamic_pointer_cast<PrintNode>(stmtLstNode->getChildren().at(0));

    REQUIRE(stmtLstNode != nullptr);
    REQUIRE(stmtLstNode->getChildren().size() == 1);
    REQUIRE(printNode != nullptr);
    REQUIRE(printNode->getStmtIndex() == 1);
  }

  SECTION(
      "StmtLst with one read stmt and one print stmt each should have index 1 "
      "and 2 respectively") {
    tokens.push_back(
        std::static_pointer_cast<Token>(std::make_shared<WordToken>("read")));
    tokens.push_back(std::static_pointer_cast<Token>(
        std::make_shared<WordToken>("variable1")));
    tokens.push_back(std::static_pointer_cast<Token>(
        std::make_shared<SpecialCharToken>(";")));
    tokens.push_back(
        std::static_pointer_cast<Token>(std::make_shared<WordToken>("print")));
    tokens.push_back(std::static_pointer_cast<Token>(
        std::make_shared<WordToken>("variable2")));
    tokens.push_back(std::static_pointer_cast<Token>(
        std::make_shared<SpecialCharToken>(";")));
    tokens.push_back(std::static_pointer_cast<Token>(
        std::make_shared<SpecialCharToken>("}")));
    tokens.push_back(
        std::static_pointer_cast<Token>(std::make_shared<EofToken>()));

    SpParser parser = SpParser(tokens);
    auto stmtLstNode = parser.parseStmtLst();
    auto readNode =
        std::dynamic_pointer_cast<ReadNode>(stmtLstNode->getChildren().at(0));
    auto printNode =
        std::dynamic_pointer_cast<PrintNode>(stmtLstNode->getChildren().at(1));

    REQUIRE(stmtLstNode != nullptr);
    REQUIRE(stmtLstNode->getChildren().size() == 2);
    REQUIRE(readNode != nullptr);
    REQUIRE(readNode->getStmtIndex() == 1);
    REQUIRE(printNode != nullptr);
    REQUIRE(printNode->getStmtIndex() == 2);
  }

  SECTION(
      "Test valid assign stmt is able to be parsed correctly by parseStmtLst") {
    /*
     * x = 1 + y;
     */
    std::vector<std::shared_ptr<Token>> tokens;

    tokens.push_back(std::make_shared<WordToken>("x"));
    tokens.push_back(std::make_shared<SpecialCharToken>("="));
    tokens.push_back(std::make_shared<IntegerToken>("1"));
    tokens.push_back(std::make_shared<SpecialCharToken>("+"));
    tokens.push_back(std::make_shared<WordToken>("y"));
    tokens.push_back(std::make_shared<SpecialCharToken>(";"));
    tokens.push_back(std::static_pointer_cast<Token>(
        std::make_shared<SpecialCharToken>("}")));
    tokens.push_back(std::make_shared<EofToken>());

    SpParser parser = SpParser(tokens);
    REQUIRE_NOTHROW(parser.parseStmtLst());
  }

  SECTION(
      "Test valid assign stmt is able to return the correct varName, "
      "constants, and variables") {
    /*
     * x = 1 + y;
     */
    std::vector<std::shared_ptr<Token>> tokens;

    tokens.push_back(std::make_shared<WordToken>("x"));
    tokens.push_back(std::make_shared<SpecialCharToken>("="));
    tokens.push_back(std::make_shared<IntegerToken>("1"));
    tokens.push_back(std::make_shared<SpecialCharToken>("+"));
    tokens.push_back(std::make_shared<WordToken>("y"));
    tokens.push_back(std::make_shared<SpecialCharToken>(";"));
    tokens.push_back(std::static_pointer_cast<Token>(
        std::make_shared<SpecialCharToken>("}")));
    tokens.push_back(std::make_shared<EofToken>());

    SpParser parser = SpParser(tokens);
    auto assignNode = std::dynamic_pointer_cast<AssignNode>(
        parser.parseStmtLst()->getChildren().at(0));
    REQUIRE(assignNode->getVarName() == "x");
    REQUIRE(assignNode->getConstants()->size() == 1);
    REQUIRE(assignNode->getVariables()->size() == 1);
  }

  SECTION(
      "Test valid assign stmt with many variables and operators return the "
      "correct constants and variables") {
    /*
     * normSq = cenX * cenX + cenY * cenY;
     */
    std::vector<std::shared_ptr<Token>> tokens;

    tokens.push_back(std::make_shared<WordToken>("normSq"));
    tokens.push_back(std::make_shared<SpecialCharToken>("="));
    tokens.push_back(std::make_shared<WordToken>("cenX"));
    tokens.push_back(std::make_shared<SpecialCharToken>("*"));
    tokens.push_back(std::make_shared<WordToken>("cenX"));
    tokens.push_back(std::make_shared<SpecialCharToken>("+"));
    tokens.push_back(std::make_shared<WordToken>("cenY"));
    tokens.push_back(std::make_shared<SpecialCharToken>("*"));
    tokens.push_back(std::make_shared<WordToken>("cenY"));
    tokens.push_back(std::make_shared<SpecialCharToken>(";"));
    tokens.push_back(std::static_pointer_cast<Token>(
        std::make_shared<SpecialCharToken>("}")));
    tokens.push_back(std::make_shared<EofToken>());

    SpParser parser = SpParser(tokens);
    auto assignNode = std::dynamic_pointer_cast<AssignNode>(
        parser.parseStmtLst()->getChildren().at(0));
    REQUIRE(assignNode->getConstants()->size() == 0);
    REQUIRE(assignNode->getVariables()->size() == 2);
    REQUIRE(assignNode->getVarName() == "normSq");
  }
}
