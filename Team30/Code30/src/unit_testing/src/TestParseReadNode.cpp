#include "../../spa/src/shared/tokenizer/token/EofToken.h"
#include "../../spa/src/shared/tokenizer/token/IntegerToken.h"
#include "../../spa/src/shared/tokenizer/token/SpecialCharToken.h"
#include "../../spa/src/shared/tokenizer/token/Token.h"
#include "../../spa/src/shared/tokenizer/token/WordToken.h"
#include "../../spa/src/source_processor/node/stmt_node/StmtType.h"
#include "../../spa/src/source_processor/parser/SpParser.h"
#include "catch.hpp"

namespace TestParseReadNode {
class TestableParser : public SpParser {
 public:
  TestableParser(std::vector<std::shared_ptr<Token>> tokens)
      : SpParser(tokens) {}

  using SpParser::parseRead;

  void parse() override {}
};
}  // namespace TestParseReadNode

TEST_CASE("Test parse read", "[parseRead]") {
  SECTION("Test valid read stmt should not throw error (i.e. read variable;)") {
    std::vector<std::shared_ptr<Token>> tokens;

    tokens.push_back(std::static_pointer_cast<Token>(
        std::make_shared<WordToken>("variable")));
    tokens.push_back(std::static_pointer_cast<Token>(
        std::make_shared<SpecialCharToken>(";")));
    tokens.push_back(
        std::static_pointer_cast<Token>(std::make_shared<EofToken>()));

        TestParseReadNode::TestableParser parser =
                TestParseReadNode::TestableParser(tokens);
    REQUIRE_NOTHROW(parser.parseRead());
  }

  SECTION("Test valid read stmt should have the correct procedure name (i.e. read variable;)") {
    std::vector<std::shared_ptr<Token>> tokens;

    tokens.push_back(std::static_pointer_cast<Token>(
        std::make_shared<WordToken>("variable")));
    tokens.push_back(std::static_pointer_cast<Token>(
        std::make_shared<SpecialCharToken>(";")));
    tokens.push_back(
        std::static_pointer_cast<Token>(std::make_shared<EofToken>()));

    TestParseReadNode::TestableParser parser =
        TestParseReadNode::TestableParser(tokens);
    std::shared_ptr<ReadNode> readNode = parser.parseRead();

    REQUIRE(readNode->getVarName() == "variable");
  }

  SECTION("Test invalid read stmt should throw error (i.e. read variable)") {
    std::vector<std::shared_ptr<Token>> tokens;

    tokens.push_back(std::static_pointer_cast<Token>(
        std::make_shared<WordToken>("variable")));
    tokens.push_back(
        std::static_pointer_cast<Token>(std::make_shared<EofToken>()));

    TestParseReadNode::TestableParser parser =
        TestParseReadNode::TestableParser(tokens);
    REQUIRE_THROWS(parser.parseRead());
  }
}