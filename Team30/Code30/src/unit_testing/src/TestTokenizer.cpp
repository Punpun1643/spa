#include "../../spa/src/shared/tokenizer/token/IntegerToken.h"

#include "catch.hpp"
using namespace std;

TEST_CASE("Integer Token Test")
{
    IntegerToken intToken = IntegerToken("33");

    REQUIRE(intToken.getTokenType() == TokenType::INTEGER_TOKEN);
    REQUIRE(intToken.getTokenVal() == "33");
}
