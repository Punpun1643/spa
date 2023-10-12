#include "../../spa/src/shared/tokenizer/token/EofToken.h"
#include "../../spa/src/shared/tokenizer/token/SpecialCharToken.h"
#include "../../spa/src/shared/tokenizer/token/Token.h"
#include "../../spa/src/shared/tokenizer/token/WordToken.h"
#include "../../spa/src/source_processor/parser/SpParser.h"
#include "catch.hpp"

namespace TestParseProcedureNode {
class TestableParser : public SpParser {
 public:
  TestableParser(std::vector<std::shared_ptr<Token>> tokens)
      : SpParser(tokens) {}

  using SpParser::ParseProcedure;

  void parse() override {}
};
}  // namespace TestParseProcedureNode

TEST_CASE("Parser parseProcedure") {
  std::vector<std::shared_ptr<Token>> tokens;

  SECTION("Invalid procedure with no statements throws exception") {
    tokens.push_back(std::static_pointer_cast<Token>(
        std::make_shared<SpecialCharToken>("}")));
    tokens.push_back(
        std::static_pointer_cast<Token>(std::make_shared<EofToken>()));

    TestParseProcedureNode::TestableParser parser =
        TestParseProcedureNode::TestableParser(tokens);
    REQUIRE_THROWS_AS(parser.ParseProcedure(), std::invalid_argument);
  }

  SECTION("Invalid procedure with } and { swap position") {
    tokens.push_back(
        std::static_pointer_cast<Token>(std::make_shared<WordToken>("main")));
    tokens.push_back(std::static_pointer_cast<Token>(
        std::make_shared<SpecialCharToken>("}")));
    tokens.push_back(
        std::static_pointer_cast<Token>(std::make_shared<WordToken>("print")));
    tokens.push_back(std::static_pointer_cast<Token>(
        std::make_shared<SpecialCharToken>("{")));
    tokens.push_back(
        std::static_pointer_cast<Token>(std::make_shared<EofToken>()));

    TestParseProcedureNode::TestableParser parser =
        TestParseProcedureNode::TestableParser(tokens);
    REQUIRE_THROWS_AS(parser.ParseProcedure(), std::invalid_argument);
  }

  SECTION("Invalid procedure with no }") {
    tokens.push_back(
        std::static_pointer_cast<Token>(std::make_shared<WordToken>("main")));
    tokens.push_back(std::static_pointer_cast<Token>(
        std::make_shared<SpecialCharToken>("{")));
    tokens.push_back(
        std::static_pointer_cast<Token>(std::make_shared<WordToken>("print")));
    tokens.push_back(std::static_pointer_cast<Token>(
        std::make_shared<WordToken>("variable")));
    tokens.push_back(
        std::static_pointer_cast<Token>(std::make_shared<WordToken>(";")));
    tokens.push_back(
        std::static_pointer_cast<Token>(std::make_shared<EofToken>()));

    TestParseProcedureNode::TestableParser parser =
        TestParseProcedureNode::TestableParser(tokens);
    REQUIRE_THROWS_AS(parser.ParseProcedure(), std::invalid_argument);
  }

  SECTION("Valid procedure with one statement has one statement node") {
    tokens.push_back(
        std::static_pointer_cast<Token>(std::make_shared<WordToken>("main")));
    tokens.push_back(std::static_pointer_cast<Token>(
        std::make_shared<SpecialCharToken>("{")));
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

    TestParseProcedureNode::TestableParser parser =
        TestParseProcedureNode::TestableParser(tokens);
    auto procedureNode = parser.ParseProcedure();

    REQUIRE(procedureNode != nullptr);
    REQUIRE(procedureNode->GetProcedureName() == "main");
    REQUIRE(procedureNode->GetChildren().at(0) != nullptr);
  }
}
