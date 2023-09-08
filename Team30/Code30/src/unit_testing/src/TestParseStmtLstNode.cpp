#include "catch.hpp"
#include "../../spa/src/source_processor/parser/SpParser.h"
#include "../../spa/src/shared/tokenizer/token/Token.h"
#include "../../spa/src/shared/tokenizer/token/SpecialCharToken.h"
#include "../../spa/src/shared/tokenizer/token/EofToken.h"
#include "../../spa/src/shared/tokenizer/token/WordToken.h"

TEST_CASE("Parser parseStmtLst") {
    std::vector<std::shared_ptr<Token>> tokens;

    SECTION("StmtLst with no statement return empty vector") {
        tokens.push_back(std::static_pointer_cast<Token>(std::make_shared<SpecialCharToken>("}")));

        SpParser parser = SpParser(tokens);
        REQUIRE(parser.parseStmtLst()->getChildren().empty());
    }
}