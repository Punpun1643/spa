#include <string.h>

#include "../../spa/src/query_processing_system/parser/QpParser.h"
#include "../../spa/src/shared/tokenizer/token/EofToken.h"
#include "../../spa/src/shared/tokenizer/token/IntegerToken.h"
#include "../../spa/src/shared/tokenizer/token/SpecialCharToken.h"
#include "../../spa/src/shared/tokenizer/token/Token.h"
#include "../../spa/src/shared/tokenizer/token/WordToken.h"
#include "catch.hpp"

// TODO: Isolate test cases away from token implementation
TEST_CASE("Parse select query") {
  std::vector<std::shared_ptr<Token>> tokens;

  tokens.push_back(
      std::static_pointer_cast<Token>(std::make_shared<WordToken>("stmt")));
  tokens.push_back(
      std::static_pointer_cast<Token>(std::make_shared<WordToken>("s")));
  tokens.push_back(
      std::static_pointer_cast<Token>(std::make_shared<SpecialCharToken>(";")));
  tokens.push_back(
      std::static_pointer_cast<Token>(std::make_shared<WordToken>("Select")));
  tokens.push_back(
      std::static_pointer_cast<Token>(std::make_shared<WordToken>("s")));
  tokens.push_back(
      std::static_pointer_cast<Token>(std::make_shared<WordToken>("such")));
  tokens.push_back(
      std::static_pointer_cast<Token>(std::make_shared<WordToken>("that")));
  tokens.push_back(
      std::static_pointer_cast<Token>(std::make_shared<WordToken>("Follows")));
  tokens.push_back(
      std::static_pointer_cast<Token>(std::make_shared<SpecialCharToken>("(")));
  tokens.push_back(
      std::static_pointer_cast<Token>(std::make_shared<IntegerToken>("2")));
  tokens.push_back(
      std::static_pointer_cast<Token>(std::make_shared<SpecialCharToken>(",")));
  tokens.push_back(
      std::static_pointer_cast<Token>(std::make_shared<IntegerToken>("3")));
  tokens.push_back(
      std::static_pointer_cast<Token>(std::make_shared<SpecialCharToken>(")")));
  tokens.push_back(
      std::static_pointer_cast<Token>(std::make_shared<EofToken>()));
  QpParser parser = QpParser(tokens);
  std::queue<std::shared_ptr<Clause>> clauseQueue = parser.parseQuery();

  std::shared_ptr<SelectClause> selectClause =
      std::static_pointer_cast<SelectClause>(clauseQueue.front());
  REQUIRE(*(selectClause->getDeclaration()->getName()) == "s");
}
