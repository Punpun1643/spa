#include "../../../../spa/src/shared/tokenizer/token/EofToken.h"
#include "../../../../spa/src/shared/tokenizer/token/IntegerToken.h"
#include "../../../../spa/src/shared/tokenizer/token/SpecialCharToken.h"
#include "../../../../spa/src/shared/tokenizer/token/Token.h"
#include "../../../../spa/src/shared/tokenizer/token/WordToken.h"
#include "../../../../spa/src/source_processor/node/stmt_node/StmtType.h"
#include "../../../../spa/src/source_processor/parser/SpParser.h"
#include "catch.hpp"

namespace TestParseCall {
class TestableParser : public SpParser {
 public:
  TestableParser(std::vector<std::shared_ptr<Token>> tokens)
      : SpParser(tokens) {}

  using SpParser::ParseCall;

  void parse() override {}
};
}  // namespace TestParseCall

TEST_CASE("Test parse call", "[parseCall]") {
  SECTION("Test valid call stmt should not throw error (i.e. call variable;)") {
    std::vector<std::shared_ptr<Token>> tokens;

    tokens.push_back(std::static_pointer_cast<Token>(
        std::make_shared<WordToken>("variable")));
    tokens.push_back(std::static_pointer_cast<Token>(
        std::make_shared<SpecialCharToken>(";")));
    tokens.push_back(
        std::static_pointer_cast<Token>(std::make_shared<EofToken>()));

    TestParseCall::TestableParser parser =
        TestParseCall::TestableParser(tokens);
    REQUIRE_NOTHROW(parser.ParseCall());
  }

  SECTION(
      "Test valid call stmt should have the correct procedure name (i.e. call "
      "variable;)") {
    std::vector<std::shared_ptr<Token>> tokens;

    tokens.push_back(std::static_pointer_cast<Token>(
        std::make_shared<WordToken>("variable")));
    tokens.push_back(std::static_pointer_cast<Token>(
        std::make_shared<SpecialCharToken>(";")));
    tokens.push_back(
        std::static_pointer_cast<Token>(std::make_shared<EofToken>()));

    TestParseCall::TestableParser parser =
        TestParseCall::TestableParser(tokens);
    std::shared_ptr<CallNode> callNode = parser.ParseCall();

    REQUIRE(callNode->GetProcName() == "variable");
  }

  SECTION("Test invalid call stmt should throw error (i.e. call variable)") {
    std::vector<std::shared_ptr<Token>> tokens;

    tokens.push_back(std::static_pointer_cast<Token>(
        std::make_shared<WordToken>("variable")));

    tokens.push_back(
        std::static_pointer_cast<Token>(std::make_shared<EofToken>()));

    TestParseCall::TestableParser parser =
        TestParseCall::TestableParser(tokens);
    REQUIRE_THROWS(parser.ParseCall());
  }
}