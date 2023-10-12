#include "../../spa/src/shared/tokenizer/token/EofToken.h"
#include "../../spa/src/shared/tokenizer/token/IntegerToken.h"
#include "../../spa/src/shared/tokenizer/token/SpecialCharToken.h"
#include "../../spa/src/shared/tokenizer/token/Token.h"
#include "../../spa/src/shared/tokenizer/token/WordToken.h"
#include "../../spa/src/source_processor/node/stmt_node/StmtType.h"
#include "../../spa/src/source_processor/parser/SpParser.h"
#include "catch.hpp"

using Catch::Matchers::Contains;

namespace TestParseWhileNode {
class TestableParser : public SpParser {
 public:
  TestableParser(std::vector<std::shared_ptr<Token>> tokens)
      : SpParser(tokens) {}

  using SpParser::parseWhile;

  void parse() override {}
};
}  // namespace TestParseWhileNode

TEST_CASE("Test parseWhile", "[parseWhile]") {
  SECTION(
      "Test missing open bracket after while keyword throws invalid while as "
      "the error message") {
    std::vector<std::shared_ptr<Token>> tokens;
    tokens.push_back(
        std::static_pointer_cast<Token>(std::make_shared<WordToken>("i")));
    tokens.push_back(std::static_pointer_cast<Token>(
        std::make_shared<SpecialCharToken>("!=")));
    tokens.push_back(
        std::static_pointer_cast<Token>(std::make_shared<IntegerToken>("0")));
    tokens.push_back(std::static_pointer_cast<Token>(
        std::make_shared<SpecialCharToken>(")")));
    tokens.push_back(std::static_pointer_cast<Token>(
        std::make_shared<SpecialCharToken>("{")));
    tokens.push_back(
        std::static_pointer_cast<Token>(std::make_shared<WordToken>("read")));
    tokens.push_back(
        std::static_pointer_cast<Token>(std::make_shared<WordToken>("i")));
    tokens.push_back(std::static_pointer_cast<Token>(
        std::make_shared<SpecialCharToken>(";")));
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

    TestParseWhileNode::TestableParser parser =
        TestParseWhileNode::TestableParser(tokens);

    REQUIRE_THROWS_WITH(parser.parseWhile(), Contains("Invalid while"));
  }

  SECTION(
      "Test other special character at the open bracket position after while "
      "keyword throws invalid while as the error message") {
    std::vector<std::shared_ptr<Token>> tokens;
    tokens.push_back(std::static_pointer_cast<Token>(
        std::make_shared<SpecialCharToken>(")")));
    tokens.push_back(
        std::static_pointer_cast<Token>(std::make_shared<WordToken>("i")));
    tokens.push_back(std::static_pointer_cast<Token>(
        std::make_shared<SpecialCharToken>("!=")));
    tokens.push_back(
        std::static_pointer_cast<Token>(std::make_shared<IntegerToken>("0")));
    tokens.push_back(std::static_pointer_cast<Token>(
        std::make_shared<SpecialCharToken>(")")));
    tokens.push_back(std::static_pointer_cast<Token>(
        std::make_shared<SpecialCharToken>("{")));
    tokens.push_back(
        std::static_pointer_cast<Token>(std::make_shared<WordToken>("read")));
    tokens.push_back(
        std::static_pointer_cast<Token>(std::make_shared<WordToken>("i")));
    tokens.push_back(std::static_pointer_cast<Token>(
        std::make_shared<SpecialCharToken>(";")));
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

    TestParseWhileNode::TestableParser parser =
        TestParseWhileNode::TestableParser(tokens);

    REQUIRE_THROWS_WITH(parser.parseWhile(), Contains("Invalid while"));
  }

  SECTION(
      "Test missing close bracket throws invalid while as the error message") {
    std::vector<std::shared_ptr<Token>> tokens;
    tokens.push_back(std::static_pointer_cast<Token>(
        std::make_shared<SpecialCharToken>("(")));
    tokens.push_back(
        std::static_pointer_cast<Token>(std::make_shared<WordToken>("i")));
    tokens.push_back(std::static_pointer_cast<Token>(
        std::make_shared<SpecialCharToken>("!=")));
    tokens.push_back(
        std::static_pointer_cast<Token>(std::make_shared<IntegerToken>("0")));
    tokens.push_back(std::static_pointer_cast<Token>(
        std::make_shared<SpecialCharToken>("{")));
    tokens.push_back(
        std::static_pointer_cast<Token>(std::make_shared<WordToken>("read")));
    tokens.push_back(
        std::static_pointer_cast<Token>(std::make_shared<WordToken>("i")));
    tokens.push_back(std::static_pointer_cast<Token>(
        std::make_shared<SpecialCharToken>(";")));
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

    TestParseWhileNode::TestableParser parser =
        TestParseWhileNode::TestableParser(tokens);

    REQUIRE_THROWS_WITH(parser.parseWhile(),
                        Contains("Invalid condition expression!"));
  }

  SECTION(
      "Test double close bracket throws invalid while as the error message") {
    std::vector<std::shared_ptr<Token>> tokens;
    tokens.push_back(std::static_pointer_cast<Token>(
        std::make_shared<SpecialCharToken>("(")));
    tokens.push_back(
        std::static_pointer_cast<Token>(std::make_shared<WordToken>("i")));
    tokens.push_back(std::static_pointer_cast<Token>(
        std::make_shared<SpecialCharToken>("!=")));
    tokens.push_back(
        std::static_pointer_cast<Token>(std::make_shared<IntegerToken>("0")));
    tokens.push_back(std::static_pointer_cast<Token>(
        std::make_shared<SpecialCharToken>(")")));
    tokens.push_back(std::static_pointer_cast<Token>(
        std::make_shared<SpecialCharToken>(")")));
    tokens.push_back(std::static_pointer_cast<Token>(
        std::make_shared<SpecialCharToken>("{")));
    tokens.push_back(
        std::static_pointer_cast<Token>(std::make_shared<WordToken>("read")));
    tokens.push_back(
        std::static_pointer_cast<Token>(std::make_shared<WordToken>("i")));
    tokens.push_back(std::static_pointer_cast<Token>(
        std::make_shared<SpecialCharToken>(";")));
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

    TestParseWhileNode::TestableParser parser =
        TestParseWhileNode::TestableParser(tokens);

    REQUIRE_THROWS_WITH(parser.parseWhile(), Contains("Invalid while"));
  }

  SECTION(
      "Test wrong character at open brace throws invalid while as the error "
      "message") {
    std::vector<std::shared_ptr<Token>> tokens;
    tokens.push_back(std::static_pointer_cast<Token>(
        std::make_shared<SpecialCharToken>("(")));
    tokens.push_back(
        std::static_pointer_cast<Token>(std::make_shared<WordToken>("i")));
    tokens.push_back(std::static_pointer_cast<Token>(
        std::make_shared<SpecialCharToken>("!=")));
    tokens.push_back(
        std::static_pointer_cast<Token>(std::make_shared<IntegerToken>("0")));
    tokens.push_back(std::static_pointer_cast<Token>(
        std::make_shared<SpecialCharToken>(")")));
    tokens.push_back(std::static_pointer_cast<Token>(
        std::make_shared<SpecialCharToken>("wrong char")));
    tokens.push_back(
        std::static_pointer_cast<Token>(std::make_shared<WordToken>("read")));
    tokens.push_back(
        std::static_pointer_cast<Token>(std::make_shared<WordToken>("i")));
    tokens.push_back(std::static_pointer_cast<Token>(
        std::make_shared<SpecialCharToken>(";")));
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

    TestParseWhileNode::TestableParser parser =
        TestParseWhileNode::TestableParser(tokens);

    REQUIRE_THROWS_WITH(parser.parseWhile(), Contains("Invalid while"));
  }

  SECTION("Test missing open brace throws invalid while as the error message") {
    std::vector<std::shared_ptr<Token>> tokens;
    tokens.push_back(std::static_pointer_cast<Token>(
        std::make_shared<SpecialCharToken>("(")));
    tokens.push_back(
        std::static_pointer_cast<Token>(std::make_shared<WordToken>("i")));
    tokens.push_back(std::static_pointer_cast<Token>(
        std::make_shared<SpecialCharToken>("!=")));
    tokens.push_back(
        std::static_pointer_cast<Token>(std::make_shared<IntegerToken>("0")));
    tokens.push_back(std::static_pointer_cast<Token>(
        std::make_shared<SpecialCharToken>(")")));
    tokens.push_back(
        std::static_pointer_cast<Token>(std::make_shared<WordToken>("read")));
    tokens.push_back(
        std::static_pointer_cast<Token>(std::make_shared<WordToken>("i")));
    tokens.push_back(std::static_pointer_cast<Token>(
        std::make_shared<SpecialCharToken>(";")));
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

    TestParseWhileNode::TestableParser parser =
        TestParseWhileNode::TestableParser(tokens);

    REQUIRE_THROWS_WITH(parser.parseWhile(), Contains("Invalid while"));
  }

  SECTION("Test missing braces throws invalid while as the error message") {
    std::vector<std::shared_ptr<Token>> tokens;
    tokens.push_back(std::static_pointer_cast<Token>(
        std::make_shared<SpecialCharToken>("(")));
    tokens.push_back(
        std::static_pointer_cast<Token>(std::make_shared<WordToken>("i")));
    tokens.push_back(std::static_pointer_cast<Token>(
        std::make_shared<SpecialCharToken>("!=")));
    tokens.push_back(
        std::static_pointer_cast<Token>(std::make_shared<IntegerToken>("0")));
    tokens.push_back(std::static_pointer_cast<Token>(
        std::make_shared<SpecialCharToken>(")")));
    tokens.push_back(std::static_pointer_cast<Token>(
        std::make_shared<SpecialCharToken>("{")));
    tokens.push_back(
        std::static_pointer_cast<Token>(std::make_shared<WordToken>("read")));
    tokens.push_back(
        std::static_pointer_cast<Token>(std::make_shared<WordToken>("i")));
    tokens.push_back(std::static_pointer_cast<Token>(
        std::make_shared<SpecialCharToken>(";")));
    tokens.push_back(
        std::static_pointer_cast<Token>(std::make_shared<WordToken>("print")));
    tokens.push_back(std::static_pointer_cast<Token>(
        std::make_shared<WordToken>("variable")));
    tokens.push_back(std::static_pointer_cast<Token>(
        std::make_shared<SpecialCharToken>(";")));
    tokens.push_back(
        std::static_pointer_cast<Token>(std::make_shared<WordToken>("")));
    tokens.push_back(
        std::static_pointer_cast<Token>(std::make_shared<EofToken>()));

    TestParseWhileNode::TestableParser parser =
        TestParseWhileNode::TestableParser(tokens);

    REQUIRE_THROWS_AS(parser.parseWhile(), std::invalid_argument);
  }

  SECTION("Test correct while with 2 statments") {
    std::vector<std::shared_ptr<Token>> tokens;
    tokens.push_back(std::static_pointer_cast<Token>(
        std::make_shared<SpecialCharToken>("(")));
    tokens.push_back(
        std::static_pointer_cast<Token>(std::make_shared<WordToken>("i")));
    tokens.push_back(std::static_pointer_cast<Token>(
        std::make_shared<SpecialCharToken>("!=")));
    tokens.push_back(
        std::static_pointer_cast<Token>(std::make_shared<IntegerToken>("0")));
    tokens.push_back(std::static_pointer_cast<Token>(
        std::make_shared<SpecialCharToken>(")")));
    tokens.push_back(std::static_pointer_cast<Token>(
        std::make_shared<SpecialCharToken>("{")));
    tokens.push_back(
        std::static_pointer_cast<Token>(std::make_shared<WordToken>("read")));
    tokens.push_back(
        std::static_pointer_cast<Token>(std::make_shared<WordToken>("i")));
    tokens.push_back(std::static_pointer_cast<Token>(
        std::make_shared<SpecialCharToken>(";")));
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

    TestParseWhileNode::TestableParser parser =
        TestParseWhileNode::TestableParser(tokens);
    std::shared_ptr<WhileNode> whileNode = parser.parseWhile();

    REQUIRE(whileNode->getStmtLst()->getChildren().size() == 2);
  }

  SECTION("Test correct while with 3 statements") {
    std::vector<std::shared_ptr<Token>> tokens;
    tokens.push_back(std::static_pointer_cast<Token>(
        std::make_shared<SpecialCharToken>("(")));
    tokens.push_back(
        std::static_pointer_cast<Token>(std::make_shared<WordToken>("i")));
    tokens.push_back(std::static_pointer_cast<Token>(
        std::make_shared<SpecialCharToken>("!=")));
    tokens.push_back(
        std::static_pointer_cast<Token>(std::make_shared<IntegerToken>("0")));
    tokens.push_back(std::static_pointer_cast<Token>(
        std::make_shared<SpecialCharToken>(")")));
    tokens.push_back(std::static_pointer_cast<Token>(
        std::make_shared<SpecialCharToken>("{")));
    tokens.push_back(
        std::static_pointer_cast<Token>(std::make_shared<WordToken>("read")));
    tokens.push_back(
        std::static_pointer_cast<Token>(std::make_shared<WordToken>("i")));
    tokens.push_back(std::static_pointer_cast<Token>(
        std::make_shared<SpecialCharToken>(";")));
    tokens.push_back(
        std::static_pointer_cast<Token>(std::make_shared<WordToken>("print")));
    tokens.push_back(std::static_pointer_cast<Token>(
        std::make_shared<WordToken>("variable")));
    tokens.push_back(std::static_pointer_cast<Token>(
        std::make_shared<SpecialCharToken>(";")));
    tokens.push_back(
        std::static_pointer_cast<Token>(std::make_shared<WordToken>("call")));
    tokens.push_back(std::static_pointer_cast<Token>(
        std::make_shared<WordToken>("readPoint")));
    tokens.push_back(std::static_pointer_cast<Token>(
        std::make_shared<SpecialCharToken>(";")));
    tokens.push_back(std::static_pointer_cast<Token>(
        std::make_shared<SpecialCharToken>("}")));
    tokens.push_back(
        std::static_pointer_cast<Token>(std::make_shared<EofToken>()));

    TestParseWhileNode::TestableParser parser =
        TestParseWhileNode::TestableParser(tokens);
    std::shared_ptr<WhileNode> whileNode = parser.parseWhile();

    REQUIRE(whileNode->getStmtLst()->getChildren().size() == 3);
  }

  SECTION("Test rel_expr i.e. i > 0") {
    std::vector<std::shared_ptr<Token>> tokens;
    tokens.push_back(std::static_pointer_cast<Token>(
        std::make_shared<SpecialCharToken>("(")));
    tokens.push_back(
        std::static_pointer_cast<Token>(std::make_shared<WordToken>("i")));
    tokens.push_back(std::static_pointer_cast<Token>(
        std::make_shared<SpecialCharToken>(">")));
    tokens.push_back(
        std::static_pointer_cast<Token>(std::make_shared<IntegerToken>("0")));
    tokens.push_back(std::static_pointer_cast<Token>(
        std::make_shared<SpecialCharToken>(")")));
    tokens.push_back(std::static_pointer_cast<Token>(
        std::make_shared<SpecialCharToken>("{")));
    tokens.push_back(
        std::static_pointer_cast<Token>(std::make_shared<WordToken>("read")));
    tokens.push_back(
        std::static_pointer_cast<Token>(std::make_shared<WordToken>("i")));
    tokens.push_back(std::static_pointer_cast<Token>(
        std::make_shared<SpecialCharToken>(";")));
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

    TestParseWhileNode::TestableParser parser =
        TestParseWhileNode::TestableParser(tokens);
    std::shared_ptr<WhileNode> whileNode = parser.parseWhile();

    REQUIRE(whileNode->getStmtLst()->getChildren().size() == 2);
  }

  SECTION("Test 2 condExpr i.e. (i != 0) && (j == 10)") {
    std::vector<std::shared_ptr<Token>> tokens;
    tokens.push_back(std::static_pointer_cast<Token>(
        std::make_shared<SpecialCharToken>("(")));
    tokens.push_back(std::static_pointer_cast<Token>(
        std::make_shared<SpecialCharToken>("(")));
    tokens.push_back(
        std::static_pointer_cast<Token>(std::make_shared<WordToken>("i")));
    tokens.push_back(std::static_pointer_cast<Token>(
        std::make_shared<SpecialCharToken>("!=")));
    tokens.push_back(
        std::static_pointer_cast<Token>(std::make_shared<IntegerToken>("0")));
    tokens.push_back(std::static_pointer_cast<Token>(
        std::make_shared<SpecialCharToken>(")")));
    tokens.push_back(std::static_pointer_cast<Token>(
        std::make_shared<SpecialCharToken>("&&")));
    tokens.push_back(std::static_pointer_cast<Token>(
        std::make_shared<SpecialCharToken>("(")));
    tokens.push_back(
        std::static_pointer_cast<Token>(std::make_shared<WordToken>("j")));
    tokens.push_back(std::static_pointer_cast<Token>(
        std::make_shared<SpecialCharToken>("==")));
    tokens.push_back(
        std::static_pointer_cast<Token>(std::make_shared<IntegerToken>("10")));
    tokens.push_back(std::static_pointer_cast<Token>(
        std::make_shared<SpecialCharToken>(")")));
    tokens.push_back(std::static_pointer_cast<Token>(
        std::make_shared<SpecialCharToken>(")")));
    tokens.push_back(std::static_pointer_cast<Token>(
        std::make_shared<SpecialCharToken>("{")));
    tokens.push_back(
        std::static_pointer_cast<Token>(std::make_shared<WordToken>("read")));
    tokens.push_back(
        std::static_pointer_cast<Token>(std::make_shared<WordToken>("i")));
    tokens.push_back(std::static_pointer_cast<Token>(
        std::make_shared<SpecialCharToken>(";")));
    tokens.push_back(
        std::static_pointer_cast<Token>(std::make_shared<WordToken>("print")));
    tokens.push_back(std::static_pointer_cast<Token>(
        std::make_shared<WordToken>("variable")));
    tokens.push_back(std::static_pointer_cast<Token>(
        std::make_shared<SpecialCharToken>(";")));
    tokens.push_back(
        std::static_pointer_cast<Token>(std::make_shared<WordToken>("call")));
    tokens.push_back(std::static_pointer_cast<Token>(
        std::make_shared<WordToken>("readPoint")));
    tokens.push_back(std::static_pointer_cast<Token>(
        std::make_shared<SpecialCharToken>(";")));
    tokens.push_back(std::static_pointer_cast<Token>(
        std::make_shared<SpecialCharToken>("}")));
    tokens.push_back(
        std::static_pointer_cast<Token>(std::make_shared<EofToken>()));

    TestParseWhileNode::TestableParser parser =
        TestParseWhileNode::TestableParser(tokens);
    std::shared_ptr<WhileNode> whileNode = parser.parseWhile();

    REQUIRE(whileNode->getStmtLst()->getChildren().size() == 3);
  }

  SECTION("Test cond_expr i.e. (j == 10) || (k == 20)") {
    std::vector<std::shared_ptr<Token>> tokens;
    tokens.push_back(std::static_pointer_cast<Token>(
        std::make_shared<SpecialCharToken>("(")));
    tokens.push_back(std::static_pointer_cast<Token>(
        std::make_shared<SpecialCharToken>("(")));
    tokens.push_back(
        std::static_pointer_cast<Token>(std::make_shared<WordToken>("j")));
    tokens.push_back(std::static_pointer_cast<Token>(
        std::make_shared<SpecialCharToken>("==")));
    tokens.push_back(
        std::static_pointer_cast<Token>(std::make_shared<IntegerToken>("10")));
    tokens.push_back(std::static_pointer_cast<Token>(
        std::make_shared<SpecialCharToken>(")")));
    tokens.push_back(std::static_pointer_cast<Token>(
        std::make_shared<SpecialCharToken>("||")));
    tokens.push_back(std::static_pointer_cast<Token>(
        std::make_shared<SpecialCharToken>("(")));
    tokens.push_back(
        std::static_pointer_cast<Token>(std::make_shared<WordToken>("k")));
    tokens.push_back(std::static_pointer_cast<Token>(
        std::make_shared<SpecialCharToken>("==")));
    tokens.push_back(
        std::static_pointer_cast<Token>(std::make_shared<IntegerToken>("20")));
    tokens.push_back(std::static_pointer_cast<Token>(
        std::make_shared<SpecialCharToken>(")")));
    tokens.push_back(std::static_pointer_cast<Token>(
        std::make_shared<SpecialCharToken>(")")));
    tokens.push_back(std::static_pointer_cast<Token>(
        std::make_shared<SpecialCharToken>("{")));
    tokens.push_back(
        std::static_pointer_cast<Token>(std::make_shared<WordToken>("read")));
    tokens.push_back(
        std::static_pointer_cast<Token>(std::make_shared<WordToken>("i")));
    tokens.push_back(std::static_pointer_cast<Token>(
        std::make_shared<SpecialCharToken>(";")));
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

    TestParseWhileNode::TestableParser parser =
        TestParseWhileNode::TestableParser(tokens);
    std::shared_ptr<WhileNode> whileNode = parser.parseWhile();

    REQUIRE(whileNode->getStmtLst()->getChildren().size() == 2);
  }

  SECTION("Test negation i.e. !(x == 0)") {
    std::vector<std::shared_ptr<Token>> tokens;
    tokens.push_back(std::static_pointer_cast<Token>(
        std::make_shared<SpecialCharToken>("(")));
    tokens.push_back(std::static_pointer_cast<Token>(
        std::make_shared<SpecialCharToken>("!")));
    tokens.push_back(std::static_pointer_cast<Token>(
        std::make_shared<SpecialCharToken>("(")));
    tokens.push_back(
        std::static_pointer_cast<Token>(std::make_shared<WordToken>("i")));
    tokens.push_back(std::static_pointer_cast<Token>(
        std::make_shared<SpecialCharToken>("!=")));
    tokens.push_back(
        std::static_pointer_cast<Token>(std::make_shared<IntegerToken>("0")));
    tokens.push_back(std::static_pointer_cast<Token>(
        std::make_shared<SpecialCharToken>(")")));
    tokens.push_back(std::static_pointer_cast<Token>(
        std::make_shared<SpecialCharToken>(")")));
    tokens.push_back(std::static_pointer_cast<Token>(
        std::make_shared<SpecialCharToken>("{")));
    tokens.push_back(
        std::static_pointer_cast<Token>(std::make_shared<WordToken>("read")));
    tokens.push_back(
        std::static_pointer_cast<Token>(std::make_shared<WordToken>("i")));
    tokens.push_back(std::static_pointer_cast<Token>(
        std::make_shared<SpecialCharToken>(";")));
    tokens.push_back(
        std::static_pointer_cast<Token>(std::make_shared<WordToken>("print")));
    tokens.push_back(std::static_pointer_cast<Token>(
        std::make_shared<WordToken>("variable")));
    tokens.push_back(std::static_pointer_cast<Token>(
        std::make_shared<SpecialCharToken>(";")));
    tokens.push_back(
        std::static_pointer_cast<Token>(std::make_shared<WordToken>("call")));
    tokens.push_back(std::static_pointer_cast<Token>(
        std::make_shared<WordToken>("readPoint")));
    tokens.push_back(std::static_pointer_cast<Token>(
        std::make_shared<SpecialCharToken>(";")));
    tokens.push_back(std::static_pointer_cast<Token>(
        std::make_shared<SpecialCharToken>("}")));
    tokens.push_back(
        std::static_pointer_cast<Token>(std::make_shared<EofToken>()));

    TestParseWhileNode::TestableParser parser =
        TestParseWhileNode::TestableParser(tokens);
    std::shared_ptr<WhileNode> whileNode = parser.parseWhile();

    REQUIRE(whileNode->getStmtLst()->getChildren().size() == 3);
  }
}