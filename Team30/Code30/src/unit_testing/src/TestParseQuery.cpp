#include <string.h>

#include <iostream>

#include "../../spa/src/query_processing_system/QPSController.h"
#include "ParserHelperFunctions.h"
#include "catch.hpp"

static void AddDeclaration(std::vector<std::shared_ptr<Token>>& tokens,
                           std::string design_entity,
                           std::vector<std::string> synonyms) {
  AddWordVector(tokens, {design_entity});
  AddWordVector(tokens, {synonyms[0]});
  if (synonyms.size() > 1) {
    std::vector<std::string> sliced_synonyms(synonyms.begin() + 1,
                                             synonyms.end());
    for (std::string const& synonym : sliced_synonyms) {
      AddSpecialCharVector(tokens, {","});
      AddWordVector(tokens, {synonym});
    }
  }
  AddSpecialCharVector(tokens, {";"});
}

static void AddIntWord(std::vector<std::shared_ptr<Token>>& tokens, std::string arg1, std::string arg2) {
  AddSpecialCharVector(tokens, {"("});
  AddInteger(tokens, arg1);
  AddSpecialCharVector(tokens, {","});
  AddWordVector(tokens, {arg2});
  AddSpecialCharVector(tokens, {")"});
}

static void AddWordWord(std::vector<std::shared_ptr<Token>>& tokens,
                        std::string arg1, std::string arg2) {
  AddSpecialCharVector(tokens, {"("});
  AddWordVector(tokens, {arg1});
  AddSpecialCharVector(tokens, {","});
  AddWordVector(tokens, {arg2});
  AddSpecialCharVector(tokens, {")"});
}

static void AddWordIdent(std::vector<std::shared_ptr<Token>>& tokens,
    std::string arg1, std::string arg2) {
  AddSpecialCharVector(tokens, {"("});
  AddWordVector(tokens, {arg1});
  AddSpecialCharVector(tokens, {",", "\""});
  AddWordVector(tokens, {arg2});
  AddSpecialCharVector(tokens, {"\"", ")"});
}

static void AddWildWord(std::vector<std::shared_ptr<Token>>& tokens,
                        std::string arg) {
  AddSpecialCharVector(tokens, {"("});
  AddSpecialCharVector(tokens, {"_", ","});
  AddWordVector(tokens, {arg});
  AddSpecialCharVector(tokens, {")"});
}


/* TEST_CASE("Parse select query") { */
/*   std::vector<std::shared_ptr<Token>> tokens; */

/*   SECTION("1 stmt declaration; Select Clause") { */
/*     AddDeclaration(tokens, "stmt", {"s"}); */
/*     AddWordVector(tokens, {"Select", "s"}); */
/*     AddEOF(tokens); */

/*     QPSController controller = QPSController(); */

/*     std::vector<std::unique_ptr<Clause>> clauses = */
/*         controller.ParseAndGetClauses(tokens); */

/*     std::unique_ptr<Clause> clause = std::move(clauses[0]);  // Move ownership */
/*     std::unique_ptr<SelectClause> select_clause( */
/*         dynamic_cast<SelectClause*>(clause.release())); */

/*     REQUIRE(*(select_clause->getDeclaration()->getName()) == "s"); */
/*   } */
/*   SECTION("1 read declaration; Select Clause") { */
/*     AddDeclaration(tokens, "read", {"r"}); */
/*     AddWordVector(tokens, {"Select", "r"}); */
/*     AddEOF(tokens); */

/*     QPSController controller = QPSController(); */

/*     std::vector<std::unique_ptr<Clause>> clauses = */
/*         controller.ParseAndGetClauses(tokens); */

/*     std::unique_ptr<Clause> clause = std::move(clauses[0]);  // Move ownership */
/*     std::unique_ptr<SelectClause> select_clause( */
/*         dynamic_cast<SelectClause*>(clause.release())); */

/*     REQUIRE(*(select_clause->getDeclaration()->getName()) == "r"); */
/*   } */
/*   SECTION("1 print declaration; Select Clause") { */
/*     AddDeclaration(tokens, "print", {"p"}); */
/*     AddWordVector(tokens, {"Select", "p"}); */
/*     AddEOF(tokens); */

/*     QPSController controller = QPSController(); */

/*     std::vector<std::unique_ptr<Clause>> clauses = */
/*         controller.ParseAndGetClauses(tokens); */

/*     std::unique_ptr<Clause> clause = std::move(clauses[0]);  // Move ownership */
/*     std::unique_ptr<SelectClause> select_clause( */
/*         dynamic_cast<SelectClause*>(clause.release())); */

/*     REQUIRE(*(select_clause->getDeclaration()->getName()) == "p"); */
/*   } */
/*   SECTION("1 call declaration; Select Clause") { */
/*     AddDeclaration(tokens, "call", {"c"}); */
/*     AddWordVector(tokens, {"Select", "c"}); */
/*     AddEOF(tokens); */

/*     QPSController controller = QPSController(); */

/*     std::vector<std::unique_ptr<Clause>> clauses = */
/*         controller.ParseAndGetClauses(tokens); */

/*     std::unique_ptr<Clause> clause = std::move(clauses[0]);  // Move ownership */
/*     std::unique_ptr<SelectClause> select_clause( */
/*         dynamic_cast<SelectClause*>(clause.release())); */

/*     REQUIRE(*(select_clause->getDeclaration()->getName()) == "c"); */
/*   } */
/*   SECTION("1 while declaration; Select Clause") { */
/*     AddDeclaration(tokens, "while", {"wh"}); */
/*     AddWordVector(tokens, {"Select", "wh"}); */
/*     AddEOF(tokens); */

/*     QPSController controller = QPSController(); */

/*     std::vector<std::unique_ptr<Clause>> clauses = */
/*         controller.ParseAndGetClauses(tokens); */

/*     std::unique_ptr<Clause> clause = std::move(clauses[0]);  // Move ownership */
/*     std::unique_ptr<SelectClause> select_clause( */
/*         dynamic_cast<SelectClause*>(clause.release())); */

/*     REQUIRE(*(select_clause->getDeclaration()->getName()) == "wh"); */
/*   } */
/*   SECTION("1 if declaration; Select Clause") { */
/*     AddDeclaration(tokens, "if", {"if"}); */
/*     AddWordVector(tokens, {"Select", "if"}); */
/*     AddEOF(tokens); */

/*     QPSController controller = QPSController(); */

/*     std::vector<std::unique_ptr<Clause>> clauses = */
/*         controller.ParseAndGetClauses(tokens); */

/*     std::unique_ptr<Clause> clause = std::move(clauses[0]);  // Move ownership */
/*     std::unique_ptr<SelectClause> select_clause( */
/*         dynamic_cast<SelectClause*>(clause.release())); */

/*     REQUIRE(*(select_clause->getDeclaration()->getName()) == "if"); */
/*   } */
/*   SECTION("1 assign declaration; Select Clause") { */
/*     AddDeclaration(tokens, "assign", {"as"}); */
/*     AddWordVector(tokens, {"Select", "as"}); */
/*     AddEOF(tokens); */

/*     QPSController controller = QPSController(); */

/*     std::vector<std::unique_ptr<Clause>> clauses = */
/*         controller.ParseAndGetClauses(tokens); */

/*     std::unique_ptr<Clause> clause = std::move(clauses[0]);  // Move ownership */
/*     std::unique_ptr<SelectClause> select_clause( */
/*         dynamic_cast<SelectClause*>(clause.release())); */

/*     REQUIRE(*(select_clause->getDeclaration()->getName()) == "as"); */
/*   } */
/*   SECTION("1 variable declaration; Select Clause") { */
/*     AddDeclaration(tokens, "variable", {"v"}); */
/*     AddWordVector(tokens, {"Select", "v"}); */
/*     AddEOF(tokens); */

/*     QPSController controller = QPSController(); */

/*     std::vector<std::unique_ptr<Clause>> clauses = */
/*         controller.ParseAndGetClauses(tokens); */

/*     std::unique_ptr<Clause> clause = std::move(clauses[0]);  // Move ownership */
/*     std::unique_ptr<SelectClause> select_clause( */
/*         dynamic_cast<SelectClause*>(clause.release())); */

/*     REQUIRE(*(select_clause->getDeclaration()->getName()) == "v"); */
/*   } */
/*   SECTION("1 constant declaration; Select Clause") { */
/*     AddDeclaration(tokens, "constant", {"c"}); */
/*     AddWordVector(tokens, {"Select", "c"}); */
/*     AddEOF(tokens); */

/*     QPSController controller = QPSController(); */

/*     std::vector<std::unique_ptr<Clause>> clauses = */
/*         controller.ParseAndGetClauses(tokens); */

/*     std::unique_ptr<Clause> clause = std::move(clauses[0]);  // Move ownership */
/*     std::unique_ptr<SelectClause> select_clause( */
/*         dynamic_cast<SelectClause*>(clause.release())); */

/*     REQUIRE(*(select_clause->getDeclaration()->getName()) == "c"); */
/*   } */
/*   SECTION("1 procedure declaration; Select Clause") { */
/*     AddDeclaration(tokens, "procedure", {"pr"}); */
/*     AddWordVector(tokens, {"Select", "pr"}); */
/*     AddEOF(tokens); */

/*     QPSController controller = QPSController(); */

/*     std::vector<std::unique_ptr<Clause>> clauses = */
/*         controller.ParseAndGetClauses(tokens); */

/*     std::unique_ptr<Clause> clause = std::move(clauses[0]);  // Move ownership */
/*     std::unique_ptr<SelectClause> select_clause( */
/*         dynamic_cast<SelectClause*>(clause.release())); */

/*     REQUIRE(*(select_clause->getDeclaration()->getName()) == "pr"); */
/*   } */

/*   SECTION("Multiple declarations for stmt; Select Clause") { */
/*     AddDeclaration(tokens, "stmt", {"s1", "s2", "Select"}); */
/*     AddWordVector(tokens, {"Select", "s1"}); */
/*     AddEOF(tokens); */

/*     QPSController controller = QPSController(); */
/*     std::vector<std::unique_ptr<Clause>> clauses = */
/*         controller.ParseAndGetClauses(tokens); */
/*     std::unique_ptr<Clause> clause = std::move(clauses[0]);  // Move ownership */
/*     std::unique_ptr<SelectClause> select_clause( */
/*         dynamic_cast<SelectClause*>(clause.release())); */

/*     REQUIRE(*(select_clause->getDeclaration()->getName()) == "s1"); */
/*   } */
/* } */

/* TEST_CASE("Parse Select + Follows query") { */
/*   std::vector<std::shared_ptr<Token>> tokens; */

/*   SECTION("1 stmt declaration; Select + Follows") { */
/*     AddDeclaration(tokens, "stmt", {"s123"}); */
/*     AddWordVector(tokens, {"Select", "s123", "such", "that", "Follows"}); */
/*     AddSpecialCharVector(tokens, {"("}); */
/*     AddInteger(tokens, "1"); */
/*     AddSpecialCharVector(tokens, {","}); */
/*     AddWordVector(tokens, {"s123"}); */
/*     AddSpecialCharVector(tokens, {")"}); */
/*     AddEOF(tokens); */

/*     QPSController controller = QPSController(); */
/*     std::vector<std::unique_ptr<Clause>> clauses = */
/*         controller.ParseAndGetClauses(tokens); */
/*     std::unique_ptr<SelectClause> select_clause( */
/*         dynamic_cast<SelectClause*>(clauses[0].release())); */
/*     std::unique_ptr<SuchThatClause> such_that_clause( */
/*         dynamic_cast<SuchThatClause*>(clauses[1].release())); */

/*     REQUIRE(*(select_clause->getDeclaration()->getName()) == "s123"); */
/*   } */
/*   SECTION("1 stmt; 1 print; 3 read declaration; Select + Follows(read, print") { */
/*     AddDeclaration(tokens, "stmt", {"s1"}); */
/*     AddDeclaration(tokens, "print", {"pr1"}); */
/*     AddDeclaration(tokens, "read", {"r1", "r2", "r3"}); */
/*     AddWordVector(tokens, {"Select", "s1", "such", "that", "Follows"}); */
/*     AddWordWord(tokens, "r1", "pr1"); */
/*     AddEOF(tokens); */

/*     QPSController controller = QPSController(); */
/*     std::vector<std::unique_ptr<Clause>> clauses = */
/*         controller.ParseAndGetClauses(tokens); */
/*     std::unique_ptr<SelectClause> select_clause( */
/*         dynamic_cast<SelectClause*>(clauses[0].release())); */
/*     std::unique_ptr<SuchThatClause> such_that_clause( */
/*         dynamic_cast<SuchThatClause*>(clauses[1].release())); */

/*     REQUIRE(*(select_clause->getDeclaration()->getName()) == "s1"); */
/*   } */
/*   SECTION("2 variable; 1 constant, 1 stmt; Select + Follows(_, variable") { */
/*     AddDeclaration(tokens, "variable", {"v1", "v2"}); */
/*     AddDeclaration(tokens, "constant", {"c"}); */
/*     AddDeclaration(tokens, "stmt", {"s1"}); */
/*     AddWordVector(tokens, {"Select", "v1", "such", "that", "Follows"}); */
/*     AddWildWord(tokens, "s1"); */
/*     AddEOF(tokens); */

/*     QPSController controller = QPSController(); */
/*     std::vector<std::unique_ptr<Clause>> clauses = */
/*         controller.ParseAndGetClauses(tokens); */
/*     std::unique_ptr<SelectClause> select_clause( */
/*         dynamic_cast<SelectClause*>(clauses[0].release())); */
/*     std::unique_ptr<SuchThatClause> such_that_clause( */
/*         dynamic_cast<SuchThatClause*>(clauses[1].release())); */

/*     REQUIRE(*(select_clause->getDeclaration()->getName()) == "v1"); */
/*   } */
/* } */

/* TEST_CASE("Parse Select + Follows* query") { */
/*   std::vector<std::shared_ptr<Token>> tokens; */

/*   SECTION("1 stmt; Select + Follows*(_, stmt)") { */
/*     AddDeclaration(tokens, "stmt", {"s1"}); */
/*     AddWordVector(tokens, {"Select", "s1", "such", "that", "Follows"}); */
/*     AddSpecialCharVector(tokens, {"*"}); */
/*     AddWildWord(tokens, "s1"); */
/*     AddEOF(tokens); */

/*     QPSController controller = QPSController(); */
/*     std::vector<std::unique_ptr<Clause>> clauses = */
/*         controller.ParseAndGetClauses(tokens); */
/*     std::unique_ptr<SelectClause> select_clause( */
/*         dynamic_cast<SelectClause*>(clauses[0].release())); */
/*     std::unique_ptr<SuchThatClause> such_that_clause( */
/*         dynamic_cast<SuchThatClause*>(clauses[1].release())); */

/*     REQUIRE(*(select_clause->getDeclaration()->getName()) == "s1"); */
/*   } */
/* } */

/* TEST_CASE("Parse Select + Parent query") { */
/*   std::vector<std::shared_ptr<Token>> tokens; */

/*   SECTION("1 stmt declaration; Select + Parent") { */
/*     AddDeclaration(tokens, "stmt", {"s123"}); */
/*     AddWordVector(tokens, {"Select", "s123", "such", "that", "Parent"}); */
/*     AddSpecialCharVector(tokens, {"("}); */
/*     AddInteger(tokens, "1"); */
/*     AddSpecialCharVector(tokens, {","}); */
/*     AddWordVector(tokens, {"s123"}); */
/*     AddSpecialCharVector(tokens, {")"}); */
/*     AddEOF(tokens); */

/*     QPSController controller = QPSController(); */
/*     std::vector<std::unique_ptr<Clause>> clauses = */
/*         controller.ParseAndGetClauses(tokens); */
/*     std::unique_ptr<SelectClause> select_clause( */
/*         dynamic_cast<SelectClause*>(clauses[0].release())); */
/*     std::unique_ptr<SuchThatClause> such_that_clause( */
/*         dynamic_cast<SuchThatClause*>(clauses[1].release())); */

/*     REQUIRE(*(select_clause->getDeclaration()->getName()) == "s123"); */
/*   } */

/*   SECTION("assign a; Select a such that parent (_, a)") { */
/*     AddDeclaration(tokens, "assign", {"a"}); */
/*     AddWordVector(tokens, {"Select", "a", "such", "that", "Parent"}); */
/*     AddWildWord(tokens, "a"); */
/*     AddEOF(tokens); */

/*     QPSController controller = QPSController(); */
/*     std::vector<std::unique_ptr<Clause>> clauses = */
/*         controller.ParseAndGetClauses(tokens); */
/*     std::unique_ptr<SelectClause> select_clause( */
/*         dynamic_cast<SelectClause*>(clauses[0].release())); */
/*     std::unique_ptr<SuchThatClause> such_that_clause( */
/*         dynamic_cast<SuchThatClause*>(clauses[1].release())); */

/*     REQUIRE(*(select_clause->getDeclaration()->getName()) == "a"); */
/*   } */
/* } */

/* TEST_CASE("Parse Select + Parent* query") { */
/*   std::vector<std::shared_ptr<Token>> tokens; */

/*   SECTION("1 stmt; Select + Parent*(_, stmt)") { */
/*     AddDeclaration(tokens, "stmt", {"s1"}); */
/*     AddWordVector(tokens, {"Select", "s1", "such", "that", "Parent"}); */
/*     AddWildWord(tokens, "s1"); */
/*     AddEOF(tokens); */

/*     QPSController controller = QPSController(); */
/*     std::vector<std::unique_ptr<Clause>> clauses = */
/*         controller.ParseAndGetClauses(tokens); */
/*     std::unique_ptr<SelectClause> select_clause( */
/*         dynamic_cast<SelectClause*>(clauses[0].release())); */
/*     std::unique_ptr<SuchThatClause> such_that_clause( */
/*         dynamic_cast<SuchThatClause*>(clauses[1].release())); */

/*     REQUIRE(*(select_clause->getDeclaration()->getName()) == "s1"); */
/*   } */
/* } */

/* TEST_CASE("Parse Select + Modifies query") { */
/*   std::vector<std::shared_ptr<Token>> tokens; */

/*   SECTION("2 assign, 1 var; Select + Modifies(assign, var)") { */
/*     AddDeclaration(tokens, "assign", {"a1", "a2"}); */
/*     AddDeclaration(tokens, "variable", {"v"}); */
/*     AddWordVector(tokens, {"Select", "a1", "such", "that", "Modifies"}); */
/*     AddWordWord(tokens, "a1", "v"); */
/*     AddEOF(tokens); */

/*     QPSController controller = QPSController(); */
/*     std::vector<std::unique_ptr<Clause>> clauses = */
/*         controller.ParseAndGetClauses(tokens); */
/*     std::unique_ptr<SelectClause> select_clause( */
/*         dynamic_cast<SelectClause*>(clauses[0].release())); */
/*     std::unique_ptr<SuchThatClause> such_that_clause( */
/*         dynamic_cast<SuchThatClause*>(clauses[1].release())); */

/*     REQUIRE(*(select_clause->getDeclaration()->getName()) == "a1"); */
/*   } */
/* } */

/* TEST_CASE("Parse Select + Uses query") { */
/*   std::vector<std::shared_ptr<Token>> tokens; */

/*   SECTION("2 assign, 1 var; Select + Uses(assign, var)") { */
/*     AddDeclaration(tokens, "assign", {"a1", "a2"}); */
/*     AddDeclaration(tokens, "variable", {"v"}); */
/*     AddWordVector(tokens, {"Select", "a1", "such", "that", "Uses"}); */
/*     AddWordWord(tokens, "a1", "v"); */
/*     AddEOF(tokens); */

/*     QPSController controller = QPSController(); */
/*     std::vector<std::unique_ptr<Clause>> clauses = */
/*         controller.ParseAndGetClauses(tokens); */
/*     std::unique_ptr<SelectClause> select_clause( */
/*         dynamic_cast<SelectClause*>(clauses[0].release())); */
/*     std::unique_ptr<SuchThatClause> such_that_clause( */
/*         dynamic_cast<SuchThatClause*>(clauses[1].release())); */

/*     REQUIRE(*(select_clause->getDeclaration()->getName()) == "a1"); */
/*   } */

/* } */

/* TEST_CASE("Parse Select + Uses(stmtref, entref)") { */
/*   std::vector<std::shared_ptr<Token>> tokens; */

/*   SECTION("1 stmt; Select + Uses(stmt, \"x\")") { */
/*     AddDeclaration(tokens, "stmt", {"s1"}); */
/*     AddWordVector(tokens, {"Select", "s1", "such", "that", "Uses"}); */
/*     AddWordIdent(tokens, "s1", "x"); */
/*     AddEOF(tokens); */

/*     QPSController controller = QPSController(); */
/*     std::vector<std::unique_ptr<Clause>> clauses = */
/*         controller.ParseAndGetClauses(tokens); */
/*     std::unique_ptr<SelectClause> select_clause( */
/*         dynamic_cast<SelectClause*>(clauses[0].release())); */
/*     std::unique_ptr<SuchThatClause> such_that_clause( */
/*         dynamic_cast<SuchThatClause*>(clauses[1].release())); */

/*     REQUIRE(*(select_clause->getDeclaration()->getName()) == "s1"); */
/*   } */

/* } */

TEST_CASE("Pattern") {
  std::vector<std::shared_ptr<Token>> tokens;

  SECTION("Positive. stmt s1, assign a; Select s1 such that Uses(s1, \"count\") pattern a (\"count\", _)") {
    AddDeclaration(tokens, "stmt", {"s1"});
    AddDeclaration(tokens, "assign", {"a"});
    AddWordVector(tokens, {"Select", "s1", "such", "that", "Uses"});
    AddWordIdent(tokens, "s1", "count");
    AddWordVector(tokens, {"pattern", "a"});
    AddSpecialCharVector(tokens, {"(", "\""});
    AddWordVector(tokens, {"count"});
    AddSpecialCharVector(tokens, {"\"", ",", "_", ")"});
    AddEOF(tokens);

    QPSController controller = QPSController();
    std::vector<std::unique_ptr<Clause>> clauses =
        controller.ParseAndGetClauses(tokens);
    std::unique_ptr<SelectClause> select_clause(
        dynamic_cast<SelectClause*>(clauses[0].release()));
    std::unique_ptr<SuchThatClause> such_that_clause(
        dynamic_cast<SuchThatClause*>(clauses[1].release()));
    /* std::unique_ptr<PatternClause*> pattern_clause( */
    /*     dynamic_cast<PatternClause*>(clauses[2].release())); */
  }

  SECTION("Positive. stmt s1, assign a; Select s1 such that Uses(s1, \"count\") pattern a (_, _\"x\"_)") {
    AddDeclaration(tokens, "stmt", {"s1"});
    AddDeclaration(tokens, "assign", {"a"});
    AddWordVector(tokens, {"Select", "s1", "such", "that", "Uses"});
    AddWordIdent(tokens, "s1", "count");
    AddWordVector(tokens, {"pattern", "a"});
    AddSpecialCharVector(tokens, {"(", "_", ",", "_", "\""});
    AddWordVector(tokens, {"x"});
    AddSpecialCharVector(tokens, {"\"", "_", ")"});
    AddEOF(tokens);

    QPSController controller = QPSController();
    std::vector<std::unique_ptr<Clause>> clauses =
        controller.ParseAndGetClauses(tokens);
    std::unique_ptr<SelectClause> select_clause(
        dynamic_cast<SelectClause*>(clauses[0].release()));
    std::unique_ptr<SuchThatClause> such_that_clause(
        dynamic_cast<SuchThatClause*>(clauses[1].release()));
  }

  SECTION("Positive. variable v, stmt s1, assign a; Select s1 such that Uses(s1, \"count\") pattern a (v, _\"x\"_)") {
    AddDeclaration(tokens, "variable", {"v"});
    AddDeclaration(tokens, "stmt", {"s1"});
    AddDeclaration(tokens, "assign", {"a"});
    AddWordVector(tokens, {"Select", "s1", "such", "that", "Uses"});
    AddWordIdent(tokens, "s1", "count");
    AddWordVector(tokens, {"pattern", "a"});
    AddSpecialCharVector(tokens, {"("});
    AddWordVector(tokens, {"v"});
    AddSpecialCharVector(tokens, {",", "_", "\""});
    AddWordVector(tokens, {"x"});
    AddSpecialCharVector(tokens, {"\"", "_", ")"});
    AddEOF(tokens);

    QPSController controller = QPSController();
    std::vector<std::unique_ptr<Clause>> clauses =
        controller.ParseAndGetClauses(tokens);
    std::unique_ptr<SelectClause> select_clause(
        dynamic_cast<SelectClause*>(clauses[0].release()));
    std::unique_ptr<SuchThatClause> such_that_clause(
        dynamic_cast<SuchThatClause*>(clauses[1].release()));
  }
}
