#include <string.h>

#include <iostream>

#include "../../spa/src/query_processing_system/QPSController.h"
#include "../../spa/src/query_processing_system/common/SelectClause.h"
#include "ParserHelperFunctions.h"
#include "catch.hpp"

static void AddDeclaration(std::vector<std::shared_ptr<Token>>& tokens, std::string design_entity, std::vector<std::string> synonyms) {
  AddWordVector(tokens, {design_entity});
  AddWordVector(tokens, {synonyms[0]});
  if (synonyms.size() > 1) {
    std::vector<std::string> sliced_synonyms(synonyms.begin() + 1, synonyms.end());
    for (const std::string& synonym : sliced_synonyms) {
      AddSpecialCharVector(tokens, {","});
      AddWordVector(tokens, {synonym});
    }
  }
  AddSpecialCharVector(tokens, {";"});
}

TEST_CASE("Parse select query") {
  std::vector<std::shared_ptr<Token>> tokens;

  SECTION("1 stmt declaration; Select Clause") {
    AddDeclaration(tokens, "stmt", {"s"});
    AddWordVector(tokens, {"Select", "s"});
    AddEOF(tokens);

    QPSController controller = QPSController();

    std::vector<std::unique_ptr<Clause>> clauses = controller.ParseAndGetClauses(tokens);

    std::unique_ptr<Clause> clause = std::move(clauses[0]); // Move ownership
    std::unique_ptr<SelectClause> select_clause(dynamic_cast<SelectClause*>(clause.release()));

    REQUIRE(*(select_clause->getDeclaration()->getName()) == "s");
  }

  SECTION("Multiple declarations for stmt; Select Clause") {
    AddDeclaration(tokens, "stmt", {"s1", "s2", "Select"});
    AddWordVector(tokens, {"Select", "s1"});
    AddEOF(tokens);

    QPSController controller = QPSController();
    std::vector<std::unique_ptr<Clause>> clauses = controller.ParseAndGetClauses(tokens);
    std::unique_ptr<Clause> clause = std::move(clauses[0]); // Move ownership
    std::unique_ptr<SelectClause> select_clause(dynamic_cast<SelectClause*>(clause.release()));

    REQUIRE(*(select_clause->getDeclaration()->getName()) == "s1");
  }
}

TEST_CASE("Parse Select + Follows query") {
  std::vector<std::shared_ptr<Token>> tokens;

  SECTION("1 stmt declaration; Select + Follows") {
    AddDeclaration(tokens, "stmt", {"s123"});
    AddWordVector(tokens, {"Select", "s123", "such", "that", "Follows"});
    AddSpecialCharVector(tokens, {"("});
    AddInteger(tokens, "1");
    AddSpecialCharVector(tokens, {","});
    AddWordVector(tokens, {"s123"});
    AddSpecialCharVector(tokens, {")"});
    AddEOF(tokens);

    QPSController controller = QPSController();
    std::vector<std::unique_ptr<Clause>> clauses = controller.ParseAndGetClauses(tokens);
    std::unique_ptr<SelectClause> select_clause(dynamic_cast<SelectClause*>(clauses[0].release()));
    std::unique_ptr<SuchThatClause> such_that_clause(dynamic_cast<SuchThatClause*>(clauses[1].release()));

    REQUIRE(*(select_clause->getDeclaration()->getName()) == "s123");
  }
}
