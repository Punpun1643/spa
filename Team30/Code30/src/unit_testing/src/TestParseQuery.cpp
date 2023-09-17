#include <string.h>

#include <iostream>

#include "../../spa/src/query_processing_system/QPSController.h"
#include "../../spa/src/query_processing_system/common/SelectClause.h"
#include "../../spa/src/shared/tokenizer/token/EofToken.h"
#include "../../spa/src/shared/tokenizer/token/IntegerToken.h"
#include "../../spa/src/shared/tokenizer/token/SpecialCharToken.h"
#include "../../spa/src/shared/tokenizer/token/Token.h"
#include "../../spa/src/shared/tokenizer/token/WordToken.h"
#include "catch.hpp"

// TODO: Isolate test cases away from token implementation
TEST_CASE("Parse select query") {
  std::vector<std::shared_ptr<Token>> tokens;

  SECTION("Valid Select") {
    tokens.push_back(
        std::static_pointer_cast<Token>(std::make_shared<WordToken>("stmt")));
    tokens.push_back(
        std::static_pointer_cast<Token>(std::make_shared<WordToken>("s")));
    tokens.push_back(std::static_pointer_cast<Token>(
        std::make_shared<SpecialCharToken>(";")));
    tokens.push_back(
        std::static_pointer_cast<Token>(std::make_shared<WordToken>("Select")));
    tokens.push_back(
        std::static_pointer_cast<Token>(std::make_shared<WordToken>("s")));
    tokens.push_back(
        std::static_pointer_cast<Token>(std::make_shared<EofToken>()));

    QPSController controller = QPSController(tokens);
    std::vector<std::shared_ptr<Clause>> clause_list =
        controller.ParseAndGetClauses();

    std::shared_ptr<SelectClause> select_clause =
        std::static_pointer_cast<SelectClause>(clause_list[0]);
    REQUIRE(*(select_clause->getDeclaration()->getName()) == "s");
    /* std::cout << "test4 " << *(select_clause->getDeclaration()->getName()) <<
     * "\n"; */
    /* std::cout << "test5 " << select_clause->getDeclaration()->getEntityType()
     * << "\n"; */
  }

  /* SECTION("Valid Select and Follows") { */
  /*   tokens.push_back( */
  /*       std::static_pointer_cast<Token>(std::make_shared<WordToken>("stmt")));
   */
  /*   tokens.push_back( */
  /*       std::static_pointer_cast<Token>(std::make_shared<WordToken>("s")));
   */
  /*   tokens.push_back( */
  /*       std::static_pointer_cast<Token>(std::make_shared<SpecialCharToken>(";")));
   */
  /*   tokens.push_back( */
  /*       std::static_pointer_cast<Token>(std::make_shared<WordToken>("Select")));
   */
  /*   tokens.push_back( */
  /*       std::static_pointer_cast<Token>(std::make_shared<WordToken>("s")));
   */
  /*   tokens.push_back( */
  /*       std::static_pointer_cast<Token>(std::make_shared<WordToken>("such")));
   */
  /*   tokens.push_back( */
  /*       std::static_pointer_cast<Token>(std::make_shared<WordToken>("that")));
   */
  /*   tokens.push_back( */
  /*       std::static_pointer_cast<Token>(std::make_shared<WordToken>("Follows")));
   */
  /*   tokens.push_back( */
  /*       std::static_pointer_cast<Token>(std::make_shared<SpecialCharToken>("(")));
   */
  /*   tokens.push_back( */
  /*       std::static_pointer_cast<Token>(std::make_shared<IntegerToken>("2")));
   */
  /*   tokens.push_back( */
  /*       std::static_pointer_cast<Token>(std::make_shared<SpecialCharToken>(",")));
   */
  /*   tokens.push_back( */
  /*       std::static_pointer_cast<Token>(std::make_shared<IntegerToken>("3")));
   */
  /*   tokens.push_back( */
  /*       std::static_pointer_cast<Token>(std::make_shared<SpecialCharToken>(")")));
   */
  /*   tokens.push_back( */
  /*       std::static_pointer_cast<Token>(std::make_shared<EofToken>())); */
  /* } */
}
