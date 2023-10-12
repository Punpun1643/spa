#include <memory>

#include "../../../../spa/src/shared/tokenizer/token/EofToken.h"
#include "../../../../spa/src/shared/tokenizer/token/IntegerToken.h"
#include "../../../../spa/src/shared/tokenizer/token/SpecialCharToken.h"
#include "../../../../spa/src/shared/tokenizer/token/Token.h"
#include "../../../../spa/src/shared/tokenizer/token/WordToken.h"
#include "../../../../spa/src/source_processor/node/stmt_node/AssignNode.h"
#include "../../../../spa/src/source_processor/node/util_node/CondExprNode.h"
#include "../../../../spa/src/source_processor/parser/SpParser.h"
#include "catch.hpp"

namespace TestParseAssign {
class TestableParser : public SpParser {
 public:
  TestableParser(std::vector<std::shared_ptr<Token>> tokens)
      : SpParser(tokens) {}

  using SpParser::ParseAssign;
  using SpParser::ParseStmtLst;

  void parse() override {}
};
}  // namespace TestParseAssign

TEST_CASE("Test parse assign", "[parseAssign]") {
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

    TestParseAssign::TestableParser parser =
        TestParseAssign::TestableParser(tokens);
    REQUIRE_NOTHROW(parser.ParseStmtLst());
  }

  SECTION("Test valid assign stmt return the correct expression tree") {
    /*
     * x = 1 + y;
     */
    std::vector<std::shared_ptr<Token>> tokens;

    tokens.push_back(std::make_shared<IntegerToken>("1"));
    tokens.push_back(std::make_shared<SpecialCharToken>("+"));
    tokens.push_back(std::make_shared<WordToken>("y"));
    tokens.push_back(std::make_shared<SpecialCharToken>(";"));
    tokens.push_back(std::make_shared<EofToken>());

    TestParseAssign::TestableParser parser =
        TestParseAssign::TestableParser(tokens);
    std::shared_ptr<AssignNode> assignNode = parser.ParseAssign("x");
    std::shared_ptr<TreeNode> exprTreeRoot = assignNode->GetRootOfTree();

    REQUIRE(exprTreeRoot->GetVal() == "+");
    REQUIRE(exprTreeRoot->GetLeftSubTree()->GetVal() == "1");
    REQUIRE(exprTreeRoot->GetRightSubTree()->GetVal() == "y");
  }

  SECTION(
      "Test valid assign stmt with many variables and operators return the "
      "correct constants and variables") {
    /*
     * normSq = cenX * cenX + cenY * cenY;
     */
    std::vector<std::shared_ptr<Token>> tokens;

    tokens.push_back(std::make_shared<WordToken>("cenX"));
    tokens.push_back(std::make_shared<SpecialCharToken>("*"));
    tokens.push_back(std::make_shared<WordToken>("cenX"));
    tokens.push_back(std::make_shared<SpecialCharToken>("+"));
    tokens.push_back(std::make_shared<WordToken>("cenY"));
    tokens.push_back(std::make_shared<SpecialCharToken>("*"));
    tokens.push_back(std::make_shared<WordToken>("cenY"));
    tokens.push_back(std::make_shared<SpecialCharToken>(";"));
    tokens.push_back(std::make_shared<EofToken>());

    TestParseAssign::TestableParser parser =
        TestParseAssign::TestableParser(tokens);
    auto assignNode = parser.ParseAssign("normSq");
    REQUIRE(assignNode->GetConstants()->size() == 0);
    REQUIRE(assignNode->GetVariables()->size() == 2);
    REQUIRE(assignNode->GetVarName() == "normSq");
  }

  SECTION(
      "Test valid assign stmt with many variables and operators return the "
      "correct expression tree") {
    /*
     * normSq = cenX * cenX + cenY * cenY;
     */
    std::vector<std::shared_ptr<Token>> tokens;

    tokens.push_back(std::make_shared<WordToken>("cenX"));
    tokens.push_back(std::make_shared<SpecialCharToken>("*"));
    tokens.push_back(std::make_shared<WordToken>("cenX"));
    tokens.push_back(std::make_shared<SpecialCharToken>("+"));
    tokens.push_back(std::make_shared<WordToken>("cenY"));
    tokens.push_back(std::make_shared<SpecialCharToken>("*"));
    tokens.push_back(std::make_shared<WordToken>("cenY"));
    tokens.push_back(std::make_shared<SpecialCharToken>(";"));
    tokens.push_back(std::make_shared<EofToken>());

    TestParseAssign::TestableParser parser =
        TestParseAssign::TestableParser(tokens);
    std::shared_ptr<AssignNode> assignNode = parser.ParseAssign("normSq");
    std::shared_ptr<TreeNode> exprTreeRoot = assignNode->GetRootOfTree();

    REQUIRE(exprTreeRoot->GetVal() == "+");
    REQUIRE(exprTreeRoot->GetLeftSubTree()->GetVal() == "*");
    REQUIRE(exprTreeRoot->GetRightSubTree()->GetVal() == "*");
    REQUIRE(exprTreeRoot->GetLeftSubTree()->GetLeftSubTree()->GetVal() ==
            "cenX");
    REQUIRE(exprTreeRoot->GetLeftSubTree()->GetRightSubTree()->GetVal() ==
            "cenX");
    REQUIRE(exprTreeRoot->GetRightSubTree()->GetLeftSubTree()->GetVal() ==
            "cenY");
    REQUIRE(exprTreeRoot->GetRightSubTree()->GetRightSubTree()->GetVal() ==
            "cenY");
  }

  SECTION(
      "Test valid assign with brackets should return the correct constants and "
      "variables") {
    /*
     * x = (1 + y) * z;
     */
    std::vector<std::shared_ptr<Token>> tokens;
    tokens.push_back(std::make_shared<SpecialCharToken>("("));
    tokens.push_back(std::make_shared<IntegerToken>("1"));
    tokens.push_back(std::make_shared<SpecialCharToken>("+"));
    tokens.push_back(std::make_shared<WordToken>("y"));
    tokens.push_back(std::make_shared<SpecialCharToken>(")"));
    tokens.push_back(std::make_shared<SpecialCharToken>("*"));
    tokens.push_back(std::make_shared<WordToken>("z"));
    tokens.push_back(std::make_shared<SpecialCharToken>(";"));
    tokens.push_back(std::make_shared<EofToken>());

    TestParseAssign::TestableParser parser =
        TestParseAssign::TestableParser(tokens);
    auto assignNode = parser.ParseAssign("x");
    REQUIRE(assignNode->GetConstants()->size() == 1);
    REQUIRE(assignNode->GetVariables()->size() == 2);
    REQUIRE(assignNode->GetVarName() == "x");
  }

  SECTION(
      "Test valid assign with brackets should return the correct expression "
      "tree") {
    /*
     * x = (1 + y) * z;
     */
    std::vector<std::shared_ptr<Token>> tokens;
    tokens.push_back(std::make_shared<SpecialCharToken>("("));
    tokens.push_back(std::make_shared<IntegerToken>("1"));
    tokens.push_back(std::make_shared<SpecialCharToken>("+"));
    tokens.push_back(std::make_shared<WordToken>("y"));
    tokens.push_back(std::make_shared<SpecialCharToken>(")"));
    tokens.push_back(std::make_shared<SpecialCharToken>("*"));
    tokens.push_back(std::make_shared<WordToken>("z"));
    tokens.push_back(std::make_shared<SpecialCharToken>(";"));
    tokens.push_back(std::make_shared<EofToken>());

    TestParseAssign::TestableParser parser =
        TestParseAssign::TestableParser(tokens);
    auto assignNode = parser.ParseAssign("x");
    std::shared_ptr<TreeNode> exprTreeRoot = assignNode->GetRootOfTree();

    REQUIRE(exprTreeRoot->GetVal() == "*");
    REQUIRE(exprTreeRoot->GetLeftSubTree()->GetVal() == "+");
    REQUIRE(exprTreeRoot->GetRightSubTree()->GetVal() == "z");
    REQUIRE(exprTreeRoot->GetLeftSubTree()->GetLeftSubTree()->GetVal() == "1");
    REQUIRE(exprTreeRoot->GetLeftSubTree()->GetRightSubTree()->GetVal() == "y");
  }

  SECTION(
      "Test valid assign with 2 brackets should return correct constants and "
      "variables") {
    /*
     * x = (1 + y) * (z + 2);
     */
    std::vector<std::shared_ptr<Token>> tokens;

    tokens.push_back(std::make_shared<SpecialCharToken>("("));
    tokens.push_back(std::make_shared<IntegerToken>("1"));
    tokens.push_back(std::make_shared<SpecialCharToken>("+"));
    tokens.push_back(std::make_shared<WordToken>("y"));
    tokens.push_back(std::make_shared<SpecialCharToken>(")"));
    tokens.push_back(std::make_shared<SpecialCharToken>("*"));
    tokens.push_back(std::make_shared<SpecialCharToken>("("));
    tokens.push_back(std::make_shared<WordToken>("z"));
    tokens.push_back(std::make_shared<SpecialCharToken>("+"));
    tokens.push_back(std::make_shared<IntegerToken>("2"));
    tokens.push_back(std::make_shared<SpecialCharToken>(")"));
    tokens.push_back(std::make_shared<SpecialCharToken>(";"));
    tokens.push_back(std::make_shared<EofToken>());

    TestParseAssign::TestableParser parser =
        TestParseAssign::TestableParser(tokens);
    auto assignNode = parser.ParseAssign("x");
    REQUIRE(assignNode->GetConstants()->size() == 2);
    REQUIRE(assignNode->GetVariables()->size() == 2);
    REQUIRE(assignNode->GetVarName() == "x");
  }

  SECTION(
      "Test valid assign with 2 brackets should return correct expression "
      "tree") {
    /*
     * x = (1 + y) * (z + 2);
     */
    std::vector<std::shared_ptr<Token>> tokens;

    tokens.push_back(std::make_shared<SpecialCharToken>("("));
    tokens.push_back(std::make_shared<IntegerToken>("1"));
    tokens.push_back(std::make_shared<SpecialCharToken>("+"));
    tokens.push_back(std::make_shared<WordToken>("y"));
    tokens.push_back(std::make_shared<SpecialCharToken>(")"));
    tokens.push_back(std::make_shared<SpecialCharToken>("*"));
    tokens.push_back(std::make_shared<SpecialCharToken>("("));
    tokens.push_back(std::make_shared<WordToken>("z"));
    tokens.push_back(std::make_shared<SpecialCharToken>("+"));
    tokens.push_back(std::make_shared<IntegerToken>("2"));
    tokens.push_back(std::make_shared<SpecialCharToken>(")"));
    tokens.push_back(std::make_shared<SpecialCharToken>(";"));
    tokens.push_back(std::make_shared<EofToken>());

    TestParseAssign::TestableParser parser =
        TestParseAssign::TestableParser(tokens);
    auto assignNode = parser.ParseAssign("x");
    std::shared_ptr<TreeNode> exprTreeRoot = assignNode->GetRootOfTree();

    REQUIRE(exprTreeRoot->GetVal() == "*");
    REQUIRE(exprTreeRoot->GetLeftSubTree()->GetVal() == "+");
    REQUIRE(exprTreeRoot->GetRightSubTree()->GetVal() == "+");
    REQUIRE(exprTreeRoot->GetLeftSubTree()->GetLeftSubTree()->GetVal() == "1");
    REQUIRE(exprTreeRoot->GetLeftSubTree()->GetRightSubTree()->GetVal() == "y");
    REQUIRE(exprTreeRoot->GetRightSubTree()->GetLeftSubTree()->GetVal() == "z");
    REQUIRE(exprTreeRoot->GetRightSubTree()->GetRightSubTree()->GetVal() ==
            "2");
  }

  SECTION(
      "Test valid assign stmt with division should return correct constants "
      "and variables") {
    /*
     * x = 1 / y;
     */
    std::vector<std::shared_ptr<Token>> tokens;

    tokens.push_back(std::make_shared<IntegerToken>("1"));
    tokens.push_back(std::make_shared<SpecialCharToken>("/"));
    tokens.push_back(std::make_shared<WordToken>("y"));
    tokens.push_back(std::make_shared<SpecialCharToken>(";"));
    tokens.push_back(std::make_shared<EofToken>());

    TestParseAssign::TestableParser parser =
        TestParseAssign::TestableParser(tokens);
    auto assignNode = parser.ParseAssign("x");
    REQUIRE(assignNode->GetConstants()->size() == 1);
    REQUIRE(assignNode->GetVariables()->size() == 1);
    REQUIRE(assignNode->GetVarName() == "x");
  }

  SECTION(
      "Test valid assign stmt with division should return correct expression "
      "tree") {
    /*
     * x = 1 / y;
     */
    std::vector<std::shared_ptr<Token>> tokens;

    tokens.push_back(std::make_shared<IntegerToken>("1"));
    tokens.push_back(std::make_shared<SpecialCharToken>("/"));
    tokens.push_back(std::make_shared<WordToken>("y"));
    tokens.push_back(std::make_shared<SpecialCharToken>(";"));
    tokens.push_back(std::make_shared<EofToken>());

    TestParseAssign::TestableParser parser =
        TestParseAssign::TestableParser(tokens);
    auto assignNode = parser.ParseAssign("x");
    std::shared_ptr<TreeNode> exprTreeRoot = assignNode->GetRootOfTree();

    REQUIRE(exprTreeRoot->GetVal() == "/");
    REQUIRE(exprTreeRoot->GetLeftSubTree()->GetVal() == "1");
    REQUIRE(exprTreeRoot->GetRightSubTree()->GetVal() == "y");
  }

  SECTION(
      "Test valid assign stmt with multiplication and division should return "
      "the correct constants and variables") {
    /*
     * x = 1 / y * z;
     */
    std::vector<std::shared_ptr<Token>> tokens;

    tokens.push_back(std::make_shared<IntegerToken>("1"));
    tokens.push_back(std::make_shared<SpecialCharToken>("/"));
    tokens.push_back(std::make_shared<WordToken>("y"));
    tokens.push_back(std::make_shared<SpecialCharToken>("*"));
    tokens.push_back(std::make_shared<WordToken>("z"));
    tokens.push_back(std::make_shared<SpecialCharToken>(";"));
    tokens.push_back(std::make_shared<EofToken>());

    TestParseAssign::TestableParser parser =
        TestParseAssign::TestableParser(tokens);
    auto assignNode = parser.ParseAssign("x");
    REQUIRE(assignNode->GetConstants()->size() == 1);
    REQUIRE(assignNode->GetVariables()->size() == 2);
    REQUIRE(assignNode->GetVarName() == "x");
  }

  SECTION(
      "Test valid assign stmt with multiplication and division should return "
      "the correct expression tree") {
    /*
     * x = 1 / y * z;
     */
    std::vector<std::shared_ptr<Token>> tokens;

    tokens.push_back(std::make_shared<IntegerToken>("1"));
    tokens.push_back(std::make_shared<SpecialCharToken>("/"));
    tokens.push_back(std::make_shared<WordToken>("y"));
    tokens.push_back(std::make_shared<SpecialCharToken>("*"));
    tokens.push_back(std::make_shared<WordToken>("z"));
    tokens.push_back(std::make_shared<SpecialCharToken>(";"));
    tokens.push_back(std::make_shared<EofToken>());

    TestParseAssign::TestableParser parser =
        TestParseAssign::TestableParser(tokens);
    auto assignNode = parser.ParseAssign("x");
    std::shared_ptr<TreeNode> exprTreeRoot = assignNode->GetRootOfTree();

    REQUIRE(exprTreeRoot->GetVal() == "*");
    REQUIRE(exprTreeRoot->GetLeftSubTree()->GetVal() == "/");
    REQUIRE(exprTreeRoot->GetRightSubTree()->GetVal() == "z");
    REQUIRE(exprTreeRoot->GetLeftSubTree()->GetLeftSubTree()->GetVal() == "1");
    REQUIRE(exprTreeRoot->GetLeftSubTree()->GetRightSubTree()->GetVal() == "y");
  }

  SECTION(
      "Test valid assign stmt with 3 operands should return the correct "
      "constants and variables") {
    /*
     * x = 1 + y + z;
     */
    std::vector<std::shared_ptr<Token>> tokens;

    tokens.push_back(std::make_shared<IntegerToken>("1"));
    tokens.push_back(std::make_shared<SpecialCharToken>("+"));
    tokens.push_back(std::make_shared<WordToken>("y"));
    tokens.push_back(std::make_shared<SpecialCharToken>("+"));
    tokens.push_back(std::make_shared<WordToken>("z"));
    tokens.push_back(std::make_shared<SpecialCharToken>(";"));
    tokens.push_back(std::make_shared<EofToken>());

    TestParseAssign::TestableParser parser =
        TestParseAssign::TestableParser(tokens);
    auto assignNode = parser.ParseAssign("x");
    REQUIRE(assignNode->GetConstants()->size() == 1);
    REQUIRE(assignNode->GetVariables()->size() == 2);
    REQUIRE(assignNode->GetVarName() == "x");
  }

  SECTION(
      "Test valid assign stmt with 3 operands should return the correct "
      "expression tree") {
    /*
     * x = 1 + y + z;
     */
    std::vector<std::shared_ptr<Token>> tokens;

    tokens.push_back(std::make_shared<IntegerToken>("1"));
    tokens.push_back(std::make_shared<SpecialCharToken>("+"));
    tokens.push_back(std::make_shared<WordToken>("y"));
    tokens.push_back(std::make_shared<SpecialCharToken>("+"));
    tokens.push_back(std::make_shared<WordToken>("z"));
    tokens.push_back(std::make_shared<SpecialCharToken>(";"));
    tokens.push_back(std::make_shared<EofToken>());

    TestParseAssign::TestableParser parser =
        TestParseAssign::TestableParser(tokens);
    auto assignNode = parser.ParseAssign("x");
    std::shared_ptr<TreeNode> exprTreeRoot = assignNode->GetRootOfTree();

    REQUIRE(exprTreeRoot->GetVal() == "+");
    REQUIRE(exprTreeRoot->GetLeftSubTree()->GetVal() == "+");
    REQUIRE(exprTreeRoot->GetRightSubTree()->GetVal() == "z");
    REQUIRE(exprTreeRoot->GetLeftSubTree()->GetLeftSubTree()->GetVal() == "1");
    REQUIRE(exprTreeRoot->GetLeftSubTree()->GetRightSubTree()->GetVal() == "y");
  }

  SECTION("Test invalid assign stmt with extra operand should throw an error") {
    /*
     * x = 2 + + y;
     */
    std::vector<std::shared_ptr<Token>> tokens;

    tokens.push_back(std::make_shared<IntegerToken>("2"));
    tokens.push_back(std::make_shared<SpecialCharToken>("+"));
    tokens.push_back(std::make_shared<SpecialCharToken>("+"));
    tokens.push_back(std::make_shared<WordToken>("y"));
    tokens.push_back(std::make_shared<SpecialCharToken>(";"));
    tokens.push_back(std::make_shared<EofToken>());

    TestParseAssign::TestableParser parser =
        TestParseAssign::TestableParser(tokens);
    REQUIRE_THROWS(parser.ParseAssign("x"));
  }

  SECTION("Test invalid assign stmt with extra bracket should throw an error") {
    /*
     * x = (1 + y) * (z + 2));
     */
    std::vector<std::shared_ptr<Token>> tokens;

    tokens.push_back(std::make_shared<SpecialCharToken>("("));
    tokens.push_back(std::make_shared<IntegerToken>("1"));
    tokens.push_back(std::make_shared<SpecialCharToken>("+"));
    tokens.push_back(std::make_shared<WordToken>("y"));
    tokens.push_back(std::make_shared<SpecialCharToken>(")"));
    tokens.push_back(std::make_shared<SpecialCharToken>("*"));
    tokens.push_back(std::make_shared<SpecialCharToken>("("));
    tokens.push_back(std::make_shared<WordToken>("z"));
    tokens.push_back(std::make_shared<SpecialCharToken>("+"));
    tokens.push_back(std::make_shared<IntegerToken>("2"));
    tokens.push_back(std::make_shared<SpecialCharToken>(")"));
    tokens.push_back(std::make_shared<SpecialCharToken>(")"));
    tokens.push_back(std::make_shared<SpecialCharToken>(";"));
    tokens.push_back(std::make_shared<EofToken>());

    TestParseAssign::TestableParser parser =
        TestParseAssign::TestableParser(tokens);
    REQUIRE_THROWS(parser.ParseAssign("x"));
  }

  SECTION(
      "Test invalid assign stmt with non closing bracket should throw an "
      "error") {
    /*
     * x = (1 + y) * z + 2);
     */
    std::vector<std::shared_ptr<Token>> tokens;

    tokens.push_back(std::make_shared<SpecialCharToken>("("));
    tokens.push_back(std::make_shared<IntegerToken>("1"));
    tokens.push_back(std::make_shared<SpecialCharToken>("+"));
    tokens.push_back(std::make_shared<WordToken>("y"));
    tokens.push_back(std::make_shared<SpecialCharToken>(")"));
    tokens.push_back(std::make_shared<SpecialCharToken>("*"));
    tokens.push_back(std::make_shared<WordToken>("z"));
    tokens.push_back(std::make_shared<SpecialCharToken>("+"));
    tokens.push_back(std::make_shared<IntegerToken>("2"));
    tokens.push_back(std::make_shared<SpecialCharToken>(")"));
    tokens.push_back(std::make_shared<SpecialCharToken>(";"));
    tokens.push_back(std::make_shared<EofToken>());

    TestParseAssign::TestableParser parser =
        TestParseAssign::TestableParser(tokens);
    REQUIRE_THROWS(parser.ParseAssign("x"));
  }

  SECTION(
      "Test invalid assign stmt with invalid operand should throw an error") {
    /*
     * x = 1 + y && 2;
     */
    std::vector<std::shared_ptr<Token>> tokens;

    tokens.push_back(std::make_shared<IntegerToken>("1"));
    tokens.push_back(std::make_shared<SpecialCharToken>("+"));
    tokens.push_back(std::make_shared<WordToken>("y"));
    tokens.push_back(std::make_shared<SpecialCharToken>("&&"));
    tokens.push_back(std::make_shared<IntegerToken>("2"));
    tokens.push_back(std::make_shared<SpecialCharToken>(";"));
    tokens.push_back(std::make_shared<EofToken>());

    TestParseAssign::TestableParser parser =
        TestParseAssign::TestableParser(tokens);
    REQUIRE_THROWS(parser.ParseAssign("x"));
  }
}