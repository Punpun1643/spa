#include "../../../../spa/src/shared/tokenizer/token/EofToken.h"
#include "../../../../spa/src/shared/tokenizer/token/IntegerToken.h"
#include "../../../../spa/src/shared/tokenizer/token/SpecialCharToken.h"
#include "../../../../spa/src/shared/tokenizer/token/Token.h"
#include "../../../../spa/src/shared/tokenizer/token/WordToken.h"
#include "../../../../spa/src/source_processor/node/stmt_node/StmtType.h"
#include "../../../../spa/src/source_processor/parser/SpParser.h"
#include "catch.hpp"

namespace TestParseStmtLstNode {
class TestableParser : public SpParser {
 public:
  TestableParser(std::vector<std::shared_ptr<Token>> tokens)
      : SpParser(tokens) {}

  using SpParser::ParseStmtLst;

  void Parse() override {}
};
}  // namespace TestParseStmtLstNode

TEST_CASE("Parser parseStmtLst print stmts") {
  std::vector<std::shared_ptr<Token>> tokens;

  SECTION("StmtLst with no statement return empty vector") {
    tokens.push_back(std::static_pointer_cast<Token>(
        std::make_shared<SpecialCharToken>("}")));
    tokens.push_back(
        std::static_pointer_cast<Token>(std::make_shared<EofToken>()));

    TestParseStmtLstNode::TestableParser parser =
        TestParseStmtLstNode::TestableParser(tokens);
    REQUIRE_THROWS(parser.ParseStmtLst());
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

    TestParseStmtLstNode::TestableParser parser =
        TestParseStmtLstNode::TestableParser(tokens);
    auto stmtLstNode = parser.ParseStmtLst();
    auto printNode =
        std::dynamic_pointer_cast<PrintNode>(stmtLstNode->GetChildren().at(0));

    REQUIRE(stmtLstNode != nullptr);
    REQUIRE(stmtLstNode->GetChildren().size() == 1);
    REQUIRE(printNode != nullptr);
    REQUIRE(printNode->GetVarName() == "variable");
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

    TestParseStmtLstNode::TestableParser parser =
        TestParseStmtLstNode::TestableParser(tokens);
    auto stmtLstNode = parser.ParseStmtLst();
    auto printNodeOne =
        std::dynamic_pointer_cast<PrintNode>(stmtLstNode->GetChildren().at(0));
    auto printNodeTwo =
        std::dynamic_pointer_cast<PrintNode>(stmtLstNode->GetChildren().at(1));

    REQUIRE(stmtLstNode != nullptr);
    REQUIRE(stmtLstNode->GetChildren().size() == 2);
    REQUIRE(printNodeOne != nullptr);
    REQUIRE(printNodeOne->GetVarName() == "variable1");
    REQUIRE(printNodeTwo != nullptr);
    REQUIRE(printNodeTwo->GetVarName() == "variable2");
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

    TestParseStmtLstNode::TestableParser parser =
        TestParseStmtLstNode::TestableParser(tokens);
    REQUIRE_THROWS_AS(parser.ParseStmtLst(), std::invalid_argument);
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

    TestParseStmtLstNode::TestableParser parser =
        TestParseStmtLstNode::TestableParser(tokens);
    REQUIRE_THROWS_AS(parser.ParseStmtLst(), std::invalid_argument);
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

    TestParseStmtLstNode::TestableParser parser =
        TestParseStmtLstNode::TestableParser(tokens);
    REQUIRE_THROWS_AS(parser.ParseStmtLst(), std::invalid_argument);
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

    TestParseStmtLstNode::TestableParser parser =
        TestParseStmtLstNode::TestableParser(tokens);
    auto stmtLstNode = parser.ParseStmtLst();
    auto printNode =
        std::dynamic_pointer_cast<ReadNode>(stmtLstNode->GetChildren().at(0));

    REQUIRE(stmtLstNode != nullptr);
    REQUIRE(stmtLstNode->GetChildren().size() == 1);
    REQUIRE(printNode != nullptr);
    REQUIRE(printNode->GetVarName() == "variable");
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

    TestParseStmtLstNode::TestableParser parser =
        TestParseStmtLstNode::TestableParser(tokens);
    auto stmtLstNode = parser.ParseStmtLst();
    auto readNode =
        std::dynamic_pointer_cast<ReadNode>(stmtLstNode->GetChildren().at(0));
    auto printNode =
        std::dynamic_pointer_cast<PrintNode>(stmtLstNode->GetChildren().at(1));

    REQUIRE(stmtLstNode != nullptr);
    REQUIRE(stmtLstNode->GetChildren().size() == 2);
    REQUIRE(readNode != nullptr);
    REQUIRE(readNode->GetStmtType() == StmtType::READ_STMT);
    REQUIRE(readNode->GetVarName() == "variable1");
    REQUIRE(printNode != nullptr);
    REQUIRE(printNode->GetStmtType() == StmtType::PRINT_STMT);
    REQUIRE(printNode->GetVarName() == "variable2");
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

    TestParseStmtLstNode::TestableParser parser =
        TestParseStmtLstNode::TestableParser(tokens);
    REQUIRE_THROWS_AS(parser.ParseStmtLst(), std::invalid_argument);
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

    TestParseStmtLstNode::TestableParser parser =
        TestParseStmtLstNode::TestableParser(tokens);
    REQUIRE_THROWS_AS(parser.ParseStmtLst(), std::invalid_argument);
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

    TestParseStmtLstNode::TestableParser parser =
        TestParseStmtLstNode::TestableParser(tokens);
    REQUIRE_THROWS_AS(parser.ParseStmtLst(), std::invalid_argument);
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

    TestParseStmtLstNode::TestableParser parser =
        TestParseStmtLstNode::TestableParser(tokens);
    auto stmtLstNode = parser.ParseStmtLst();
    auto callNode =
        std::dynamic_pointer_cast<CallNode>(stmtLstNode->GetChildren().at(0));

    REQUIRE(stmtLstNode != nullptr);
    REQUIRE(stmtLstNode->GetChildren().size() == 1);
    REQUIRE(callNode != nullptr);
    REQUIRE(callNode->GetProcName() == "variable");
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

    TestParseStmtLstNode::TestableParser parser =
        TestParseStmtLstNode::TestableParser(tokens);
    auto stmtLstNode = parser.ParseStmtLst();
    auto callNode =
        std::dynamic_pointer_cast<CallNode>(stmtLstNode->GetChildren().at(0));
    auto readNode =
        std::dynamic_pointer_cast<ReadNode>(stmtLstNode->GetChildren().at(1));

    REQUIRE(stmtLstNode != nullptr);
    REQUIRE(stmtLstNode->GetChildren().size() == 2);
    REQUIRE(callNode != nullptr);
    REQUIRE(callNode->GetStmtType() == StmtType::CALL_STMT);
    REQUIRE(callNode->GetProcName() == "variable1");
    REQUIRE(readNode != nullptr);
    REQUIRE(readNode->GetStmtType() == StmtType::READ_STMT);
    REQUIRE(readNode->GetVarName() == "variable2");
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

    TestParseStmtLstNode::TestableParser parser =
        TestParseStmtLstNode::TestableParser(tokens);
    REQUIRE_THROWS_AS(parser.ParseStmtLst(), std::invalid_argument);
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

    TestParseStmtLstNode::TestableParser parser =
        TestParseStmtLstNode::TestableParser(tokens);
    REQUIRE_THROWS_AS(parser.ParseStmtLst(), std::invalid_argument);
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

    TestParseStmtLstNode::TestableParser parser =
        TestParseStmtLstNode::TestableParser(tokens);
    REQUIRE_THROWS_AS(parser.ParseStmtLst(), std::invalid_argument);
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

    TestParseStmtLstNode::TestableParser parser =
        TestParseStmtLstNode::TestableParser(tokens);
    auto stmtLstNode = parser.ParseStmtLst();
    auto printNode =
        std::dynamic_pointer_cast<PrintNode>(stmtLstNode->GetChildren().at(0));

    REQUIRE(stmtLstNode != nullptr);
    REQUIRE(stmtLstNode->GetChildren().size() == 1);
    REQUIRE(printNode != nullptr);
    REQUIRE(printNode->GetStmtIndex() == 1);
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

    TestParseStmtLstNode::TestableParser parser =
        TestParseStmtLstNode::TestableParser(tokens);
    auto stmtLstNode = parser.ParseStmtLst();
    auto readNode =
        std::dynamic_pointer_cast<ReadNode>(stmtLstNode->GetChildren().at(0));
    auto printNode =
        std::dynamic_pointer_cast<PrintNode>(stmtLstNode->GetChildren().at(1));

    REQUIRE(stmtLstNode != nullptr);
    REQUIRE(stmtLstNode->GetChildren().size() == 2);
    REQUIRE(readNode != nullptr);
    REQUIRE(readNode->GetStmtIndex() == 1);
    REQUIRE(printNode != nullptr);
    REQUIRE(printNode->GetStmtIndex() == 2);
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

    TestParseStmtLstNode::TestableParser parser =
        TestParseStmtLstNode::TestableParser(tokens);
    REQUIRE_NOTHROW(parser.ParseStmtLst());
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

    TestParseStmtLstNode::TestableParser parser =
        TestParseStmtLstNode::TestableParser(tokens);
    auto assignNode = std::dynamic_pointer_cast<AssignNode>(
        parser.ParseStmtLst()->GetChildren().at(0));
    REQUIRE(assignNode->GetVarName() == "x");
    REQUIRE(assignNode->GetConstants()->size() == 1);
    REQUIRE(assignNode->GetVariables()->size() == 1);
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

    TestParseStmtLstNode::TestableParser parser =
        TestParseStmtLstNode::TestableParser(tokens);
    auto assignNode = std::dynamic_pointer_cast<AssignNode>(
        parser.ParseStmtLst()->GetChildren().at(0));
    REQUIRE(assignNode->GetConstants()->size() == 0);
    REQUIRE(assignNode->GetVariables()->size() == 2);
    REQUIRE(assignNode->GetVarName() == "normSq");
  }
}
