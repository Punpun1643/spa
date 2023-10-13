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

static void AddWordWild(std::vector<std::shared_ptr<Token>>& tokens,
                        std::string arg) {
  AddSpecialCharVector(tokens, {"("});
  AddWordVector(tokens, {arg});
  AddSpecialCharVector(tokens, {",", "_"});
  AddSpecialCharVector(tokens, {")"});
}

static void AddWildWord(std::vector<std::shared_ptr<Token>>& tokens,
                        std::string arg) {
  AddSpecialCharVector(tokens, {"("});
  AddSpecialCharVector(tokens, {"_", ","});
  AddWordVector(tokens, {arg});
  AddSpecialCharVector(tokens, {")"});
}

static void AddWildWild(std::vector<std::shared_ptr<Token>>& tokens) {
  AddSpecialCharVector(tokens, {"("});
  AddSpecialCharVector(tokens, {"_", ","});
  AddSpecialCharVector(tokens, {"_"});
  AddSpecialCharVector(tokens, {")"});
}


TEST_CASE("Parse select query") {
  std::vector<std::shared_ptr<Token>> tokens;

  SECTION("1 stmt declaration; Select Clause") {
    AddDeclaration(tokens, "stmt", {"s"});
    AddWordVector(tokens, {"Select", "s"});
    AddEOF(tokens);

    QPSController controller = QPSController();
    controller.TokensToClauses(tokens);
  }
  SECTION("1 read declaration; Select Clause") {
    AddDeclaration(tokens, "read", {"r"});
    AddWordVector(tokens, {"Select", "r"});
    AddEOF(tokens);

    QPSController controller = QPSController();
    controller.TokensToClauses(tokens);
}
  SECTION("1 print declaration; Select Clause") {
    AddDeclaration(tokens, "print", {"p"});
    AddWordVector(tokens, {"Select", "p"});
    AddEOF(tokens);

    QPSController controller = QPSController();
    controller.TokensToClauses(tokens);
}
  SECTION("1 call declaration; Select Clause") {
    AddDeclaration(tokens, "call", {"c"});
    AddWordVector(tokens, {"Select", "c"});
    AddEOF(tokens);

    QPSController controller = QPSController();
    controller.TokensToClauses(tokens);
  }
  SECTION("1 while declaration; Select Clause") {
    AddDeclaration(tokens, "while", {"wh"});
    AddWordVector(tokens, {"Select", "wh"});
    AddEOF(tokens);

    QPSController controller = QPSController();
    controller.TokensToClauses(tokens);
  }
  SECTION("1 if declaration; Select Clause") {
    AddDeclaration(tokens, "if", {"if"});
    AddWordVector(tokens, {"Select", "if"});
    AddEOF(tokens);

    QPSController controller = QPSController();
    controller.TokensToClauses(tokens);
  }
  SECTION("1 assign declaration; Select Clause") {
    AddDeclaration(tokens, "assign", {"as"});
    AddWordVector(tokens, {"Select", "as"});
    AddEOF(tokens);

    QPSController controller = QPSController();
    controller.TokensToClauses(tokens);
  }
  SECTION("1 variable declaration; Select Clause") {
    AddDeclaration(tokens, "variable", {"v"});
    AddWordVector(tokens, {"Select", "v"});
    AddEOF(tokens);

    QPSController controller = QPSController();
    controller.TokensToClauses(tokens);
  }
  SECTION("1 constant declaration; Select Clause") {
    AddDeclaration(tokens, "constant", {"c"});
    AddWordVector(tokens, {"Select", "c"});
    AddEOF(tokens);

    QPSController controller = QPSController();
    controller.TokensToClauses(tokens);

  }
  SECTION("1 procedure declaration; Select Clause") {
    AddDeclaration(tokens, "procedure", {"pr"});
    AddWordVector(tokens, {"Select", "pr"});
    AddEOF(tokens);

    QPSController controller = QPSController();
    controller.TokensToClauses(tokens);
  }

  SECTION("stmt s1, s2, Select; Select s1") {
    AddDeclaration(tokens, "stmt", {"s1", "s2", "Select"});
    AddWordVector(tokens, {"Select", "s1"});
    AddEOF(tokens);

    QPSController controller = QPSController();
    controller.TokensToClauses(tokens);
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
    controller.TokensToClauses(tokens);
  }
  SECTION("1 stmt; 1 print; 3 read declaration; Select + Follows(read, print") {
    AddDeclaration(tokens, "stmt", {"s1"});
    AddDeclaration(tokens, "print", {"pr1"});
    AddDeclaration(tokens, "read", {"r1", "r2", "r3"});
    AddWordVector(tokens, {"Select", "s1", "such", "that", "Follows"});
    AddWordWord(tokens, "r1", "pr1");
    AddEOF(tokens);

    QPSController controller = QPSController();
    controller.TokensToClauses(tokens);
  }
  SECTION("2 variable; 1 constant, 1 stmt; Select + Follows(_, variable") {
    AddDeclaration(tokens, "variable", {"v1", "v2"});
    AddDeclaration(tokens, "constant", {"c"});
    AddDeclaration(tokens, "stmt", {"s1"});
    AddWordVector(tokens, {"Select", "v1", "such", "that", "Follows"});
    AddWildWord(tokens, "s1");
    AddEOF(tokens);

    QPSController controller = QPSController();
    controller.TokensToClauses(tokens);
  }
}

TEST_CASE("Parse Select + Follows* query") {
  std::vector<std::shared_ptr<Token>> tokens;

  SECTION("stmt s1; Select s1 such that Follows*(_, stmt)") {
    AddDeclaration(tokens, "stmt", {"s1"});
    AddWordVector(tokens, {"Select", "s1", "such", "that", "Follows*"});
    AddWildWord(tokens, "s1");
    AddEOF(tokens);

    QPSController controller = QPSController();
    controller.TokensToClauses(tokens);
  }
}

TEST_CASE("Parse Select + Parent query") {
  std::vector<std::shared_ptr<Token>> tokens;

  SECTION("1 stmt declaration; Select + Parent") {
    AddDeclaration(tokens, "stmt", {"s123"});
    AddWordVector(tokens, {"Select", "s123", "such", "that", "Parent"});
    AddSpecialCharVector(tokens, {"("});
    AddInteger(tokens, "1");
    AddSpecialCharVector(tokens, {","});
    AddWordVector(tokens, {"s123"});
    AddSpecialCharVector(tokens, {")"});
    AddEOF(tokens);

    QPSController controller = QPSController();
    controller.TokensToClauses(tokens);
  }

  SECTION("assign a; Select a such that parent (_, a)") {
    AddDeclaration(tokens, "assign", {"a"});
    AddWordVector(tokens, {"Select", "a", "such", "that", "Parent"});
    AddWildWord(tokens, "a");
    AddEOF(tokens);

    QPSController controller = QPSController();
    controller.TokensToClauses(tokens);
  }
}

TEST_CASE("Parse Select + Parent* query") {
  std::vector<std::shared_ptr<Token>> tokens;

  SECTION("1 stmt; Select + Parent*(_, stmt)") {
    AddDeclaration(tokens, "stmt", {"s1"});
    AddWordVector(tokens, {"Select", "s1", "such", "that", "Parent*"});
    AddWildWord(tokens, "s1");
    AddEOF(tokens);

    QPSController controller = QPSController();
    controller.TokensToClauses(tokens);
  }
}

TEST_CASE("Parse Select + Modifies query") {
  std::vector<std::shared_ptr<Token>> tokens;

  SECTION("2 assign, 1 var; Select + Modifies(assign, var)") {
    AddDeclaration(tokens, "assign", {"a1", "a2"});
    AddDeclaration(tokens, "variable", {"v"});
    AddWordVector(tokens, {"Select", "a1", "such", "that", "Modifies"});
    AddWordWord(tokens, "a1", "v");
    AddEOF(tokens);

    QPSController controller = QPSController();
    controller.TokensToClauses(tokens);
  }
}

TEST_CASE("Parse Select + Uses query") {
  std::vector<std::shared_ptr<Token>> tokens;

  SECTION("2 assign, 1 var; Select + Uses(assign, var)") {
    AddDeclaration(tokens, "assign", {"a1", "a2"});
    AddDeclaration(tokens, "variable", {"v"});
    AddWordVector(tokens, {"Select", "a1", "such", "that", "Uses"});
    AddWordWord(tokens, "a1", "v");
    AddEOF(tokens);

    QPSController controller = QPSController();
    controller.TokensToClauses(tokens);
  }
}

TEST_CASE("Parse Select + Uses(stmtref, entref)") {
  std::vector<std::shared_ptr<Token>> tokens;

  SECTION("1 stmt; Select + Uses(stmt, \"x\")") {
    AddDeclaration(tokens, "stmt", {"s1"});
    AddWordVector(tokens, {"Select", "s1", "such", "that", "Uses"});
    AddWordIdent(tokens, "s1", "x");
    AddEOF(tokens);

    QPSController controller = QPSController();
    controller.TokensToClauses(tokens);
  }
}

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
    controller.TokensToClauses(tokens);
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
    controller.TokensToClauses(tokens);
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
    controller.TokensToClauses(tokens);
  }
}

TEST_CASE("Multiple Select, BOOLEAN Select") {
  std::vector<std::shared_ptr<Token>> tokens;
  QPSController controller = QPSController();

  SECTION("Positive Select BOOLEAN") {
    AddDeclaration(tokens, "stmt", {"s1"});
    AddWordVector(tokens, {"Select", "BOOLEAN", "such", "that", "Follows"});
    AddIntWord(tokens, "3", "s1");
    AddEOF(tokens);

    controller.TokensToClauses(tokens);
  }

  SECTION("Positive Select declared BOOLEAN") {
    AddDeclaration(tokens, "stmt", {"s1", "BOOLEAN"});
    AddWordVector(tokens, {"Select", "BOOLEAN", "such", "that", "Follows"});
    AddIntWord(tokens, "3", "s1");
    AddEOF(tokens);

    controller.TokensToClauses(tokens);
  }

  SECTION("Positive Select Multiple") {
    AddDeclaration(tokens, "stmt", {"s1", "BOOLEAN"});
    AddDeclaration(tokens, "assign", {"a1", "a2"});
    AddWordVector(tokens, {"Select", "BOOLEAN", "such", "that", "Follows"});
    AddWordWord(tokens, "s1", "a2");
    AddWordVector(tokens, {"pattern", "a2"});
    AddWildWild(tokens);
    AddEOF(tokens);

    controller.TokensToClauses(tokens);
  }

  SECTION("Positive Select Multiple with declared BOOLEAN") {
    AddDeclaration(tokens, "stmt", {"s1", "BOOLEAN"});
    AddDeclaration(tokens, "assign", {"a1", "a2"});
    AddWordVector(tokens, {"Select"});
    AddSpecialCharVector(tokens, {"<"});
    AddWordVector(tokens, {"s1"});
    AddSpecialCharVector(tokens, {","});
    AddWordVector(tokens, {"a1"});
    AddSpecialCharVector(tokens, {","});
    AddWordVector(tokens, {"BOOLEAN"});
    AddSpecialCharVector(tokens, {">"});
    AddWordVector(tokens, {"such", "that", "Follows"});
    AddWordWord(tokens, "s1", "a2");
    AddWordVector(tokens, {"pattern", "a2"});
    AddWildWild(tokens);
    AddEOF(tokens);

    controller.TokensToClauses(tokens);
  }

  SECTION("Negative Select Multiple. Additional comma.") {
    AddDeclaration(tokens, "stmt", {"s1", "BOOLEAN"});
    AddDeclaration(tokens, "assign", {"a1", "a2"});
    AddWordVector(tokens, {"Select"});
    AddSpecialCharVector(tokens, {"<"});
    AddWordVector(tokens, {"s1"});
    AddSpecialCharVector(tokens, {","});
    AddWordVector(tokens, {"a1"});
    AddSpecialCharVector(tokens, {","});
    AddWordVector(tokens, {"BOOLEAN"});
    AddSpecialCharVector(tokens, {","});
    AddSpecialCharVector(tokens, {">"});
    AddWordVector(tokens, {"such", "that", "Follows"});
    AddWordWord(tokens, "s1", "a2");
    AddWordVector(tokens, {"pattern", "a2"});
    AddWildWild(tokens);
    AddEOF(tokens);

    REQUIRE_THROWS(controller.TokensToClauses(tokens));
  }

  SECTION("Negative Select Multiple. Wrong ending of multiple select.") {
    AddDeclaration(tokens, "stmt", {"s1", "BOOLEAN"});
    AddDeclaration(tokens, "assign", {"a1", "a2"});
    AddWordVector(tokens, {"Select"});
    AddSpecialCharVector(tokens, {"<"});
    AddWordVector(tokens, {"s1"});
    AddSpecialCharVector(tokens, {","});
    AddWordVector(tokens, {"a1"});
    AddSpecialCharVector(tokens, {","});
    AddWordVector(tokens, {"BOOLEAN"});
    AddSpecialCharVector(tokens, {"<"});
    AddWordVector(tokens, {"such", "that", "Follows"});
    AddWordWord(tokens, "s1", "a2");
    AddWordVector(tokens, {"pattern", "a2"});
    AddWildWild(tokens);
    AddEOF(tokens);

    REQUIRE_THROWS(controller.TokensToClauses(tokens));
  }
}

TEST_CASE("Such That And") {
  std::vector<std::shared_ptr<Token>> tokens;
  QPSController controller = QPSController();

  SECTION("stmt s1, s2; Select s1 such that Follows*(_, s2) and Modifies(s2, _)") {
    AddDeclaration(tokens, "stmt", {"s1", "s2"});
    AddWordVector(tokens, {"Select", "s1", "such", "that", "Follows*"});
    AddWildWord(tokens, "s2");
    AddWordVector(tokens, {"and"});
    AddWordVector(tokens, {"Modifies"});
    AddWordWild(tokens, "s2");
    AddEOF(tokens);

    controller.TokensToClauses(tokens);
  }

  SECTION("Positive. Complex") {
  }

  SECTION ("Syntax Error") {
  }
}
