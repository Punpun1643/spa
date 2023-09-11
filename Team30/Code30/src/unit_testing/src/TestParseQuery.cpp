#include <string.h>

#include "../../spa/src/query_processing_system/parser/QpParser.h"
#include "../../spa/src/shared/tokenizer/token/EofToken.h"
#include "../../spa/src/shared/tokenizer/token/SpecialCharToken.h"
#include "../../spa/src/shared/tokenizer/token/Token.h"
#include "../../spa/src/shared/tokenizer/token/WordToken.h"
#include "catch.hpp"

// TODO: Isolate test cases away from token implementation
TEST_CASE("Parse select query") {
  SECTION("Valid query") {
    std::vector<std::shared_ptr<Token>> tokens;

    tokens.push_back(
        std::static_pointer_cast<Token>(std::make_shared<WordToken>("stmt")));
    tokens.push_back(
        std::static_pointer_cast<Token>(std::make_shared<WordToken>("s")));
    tokens.push_back(std::static_pointer_cast<Token>(
        std::make_shared<SpecialCharToken>(";")));
    tokens.push_back(
        std::static_pointer_cast<Token>(std::make_shared<WordToken>("select")));
    tokens.push_back(
        std::static_pointer_cast<Token>(std::make_shared<WordToken>("s")));
    tokens.push_back(
        std::static_pointer_cast<Token>(std::make_shared<EofToken>()));
    QpParser parser = QpParser(tokens);
    std::shared_ptr<ParsedQuery> parsedQuery = parser.parseQuery();
    REQUIRE(strcmp(
        parsedQuery->selectClause.getDeclaration()->getName()->c_str(), "s "));
  }
}
