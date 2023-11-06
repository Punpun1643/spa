#include <string.h>

#include <iostream>

#include "../../../spa/src/query_processing_system/QPSController.h"
#include "../../../spa/src/query_processing_system/exceptions/InvalidSemanticsException.h"
#include "../../../spa/src/query_processing_system/exceptions/InvalidSyntaxException.h"
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

static void AddIdentWild(std::vector<std::shared_ptr<Token>>& tokens,
                         std::string arg1) {
  AddSpecialCharVector(tokens, {"(", "\""});
  AddWordVector(tokens, {arg1});
  AddSpecialCharVector(tokens, {"\"", ",", "_", ")"});
}

static void AddIdentIdent(std::vector<std::shared_ptr<Token>>& tokens,
                          std::string arg1, std::string arg2) {
  AddSpecialCharVector(tokens, {"(", "\""});
  AddWordVector(tokens, {arg1});
  AddSpecialCharVector(tokens, {"\"", ",", "\""});
  AddWordVector(tokens, {arg2});
  AddSpecialCharVector(tokens, {"\"", ")"});
}

static void AddIntWord(std::vector<std::shared_ptr<Token>>& tokens,
                       std::string arg1, std::string arg2) {
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
  QPSController controller = QPSController();

  SECTION("1 stmt declaration; Select Clause") {
    AddDeclaration(tokens, "stmt", {"s"});
    AddWordVector(tokens, {"Select", "s"});
    AddEOF(tokens);

    controller.TokensToClauses(tokens);
  }
  SECTION("1 read declaration; Select Clause") {
    AddDeclaration(tokens, "read", {"r"});
    AddWordVector(tokens, {"Select", "r"});
    AddEOF(tokens);

    controller.TokensToClauses(tokens);
  }
  SECTION("1 print declaration; Select Clause") {
    AddDeclaration(tokens, "print", {"p"});
    AddWordVector(tokens, {"Select", "p"});
    AddEOF(tokens);

    controller.TokensToClauses(tokens);
  }
  SECTION("1 call declaration; Select Clause") {
    AddDeclaration(tokens, "call", {"c"});
    AddWordVector(tokens, {"Select", "c"});
    AddEOF(tokens);

    controller.TokensToClauses(tokens);
  }
  SECTION("1 while declaration; Select Clause") {
    AddDeclaration(tokens, "while", {"wh"});
    AddWordVector(tokens, {"Select", "wh"});
    AddEOF(tokens);
  }
  SECTION("1 if declaration; Select Clause") {
    AddDeclaration(tokens, "if", {"if"});
    AddWordVector(tokens, {"Select", "if"});
    AddEOF(tokens);
  }
  SECTION("1 assign declaration; Select Clause") {
    AddDeclaration(tokens, "assign", {"as"});
    AddWordVector(tokens, {"Select", "as"});
    AddEOF(tokens);
  }
  SECTION("1 variable declaration; Select Clause") {
    AddDeclaration(tokens, "variable", {"v"});
    AddWordVector(tokens, {"Select", "v"});
    AddEOF(tokens);
  }
  SECTION("1 constant declaration; Select Clause") {
    AddDeclaration(tokens, "constant", {"c"});
    AddWordVector(tokens, {"Select", "c"});
    AddEOF(tokens);

    controller.TokensToClauses(tokens);
  }
  SECTION("1 procedure declaration; Select Clause") {
    AddDeclaration(tokens, "procedure", {"pr"});
    AddWordVector(tokens, {"Select", "pr"});
    AddEOF(tokens);

    controller.TokensToClauses(tokens);
  }

  SECTION("stmt s1, s2, Select; Select s1") {
    AddDeclaration(tokens, "stmt", {"s1", "s2", "Select"});
    AddWordVector(tokens, {"Select", "s1"});
    AddEOF(tokens);

    controller.TokensToClauses(tokens);
  }
  SECTION("Negative: stmt s1; Select s2") {
    AddDeclaration(tokens, "stmt", {"s1"});
    AddWordVector(tokens, {"Select", "s2"});
    AddEOF(tokens);

    REQUIRE_THROWS(controller.TokensToClauses(tokens));
  }
  SECTION("Negative: Select a") {
    AddWordVector(tokens, {"Select", "a"});
    AddEOF(tokens);

    REQUIRE_THROWS(controller.TokensToClauses(tokens));
  }
  SECTION("Select BOOLEAN") {
    AddWordVector(tokens, {"Select", "BOOLEAN"});
    AddEOF(tokens);

    controller.TokensToClauses(tokens);
  }
  SECTION("assign a; Select <a a>") {
    AddDeclaration(tokens, "assign", {"a"});
    AddWordVector(tokens, {"Select"});
    AddSpecialCharVector(tokens, {"<"});
    AddWordVector(tokens, {"a", "a"});
    AddSpecialCharVector(tokens, {">"});
    AddEOF(tokens);

    REQUIRE_THROWS_AS(controller.TokensToClauses(tokens), InvalidSyntaxException);
  }
}

TEST_CASE("Parse Select + Follows query") {
  std::vector<std::shared_ptr<Token>> tokens;
  QPSController controller = QPSController();

  SECTION("Invalid such that") {
    AddDeclaration(tokens, "stmt", {"s123"});
    AddWordVector(tokens, {"Select", "s123", "such", "tht", "Follows"});
    AddIntWord(tokens, "1", "s123");
    AddEOF(tokens);

    REQUIRE_THROWS(controller.TokensToClauses(tokens));
  }

  SECTION("1 stmt declaration; Select + Follows") {
    AddDeclaration(tokens, "stmt", {"s123"});
    AddWordVector(tokens, {"Select", "s123", "such", "that", "Follows"});
    AddSpecialCharVector(tokens, {"("});
    AddInteger(tokens, "1");
    AddSpecialCharVector(tokens, {","});
    AddWordVector(tokens, {"s123"});
    AddSpecialCharVector(tokens, {")"});
    AddEOF(tokens);

    controller.TokensToClauses(tokens);
  }
  SECTION("1 stmt; 1 print; 3 read declaration; Select + Follows(read, print") {
    AddDeclaration(tokens, "stmt", {"s1"});
    AddDeclaration(tokens, "print", {"pr1"});
    AddDeclaration(tokens, "read", {"r1", "r2", "r3"});
    AddWordVector(tokens, {"Select", "s1", "such", "that", "Follows"});
    AddWordWord(tokens, "r1", "pr1");
    AddEOF(tokens);

    controller.TokensToClauses(tokens);
  }
  SECTION("2 variable; 1 constant, 1 stmt; Select + Follows(_, variable") {
    AddDeclaration(tokens, "variable", {"v1", "v2"});
    AddDeclaration(tokens, "constant", {"c"});
    AddDeclaration(tokens, "stmt", {"s1"});
    AddWordVector(tokens, {"Select", "v1", "such", "that", "Follows"});
    AddWildWord(tokens, "s1");
    AddEOF(tokens);

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

  SECTION("assign a; Select a such that Parent (_, a)") {
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
  QPSController controller = QPSController();

  SECTION(
      "Positive. stmt s1, assign a; Select s1 such that Uses(s1, \"count\") "
      "pattern a (\"count\", _)") {
    AddDeclaration(tokens, "stmt", {"s1"});
    AddDeclaration(tokens, "assign", {"a"});
    AddWordVector(tokens, {"Select", "s1", "such", "that", "Uses"});
    AddWordIdent(tokens, "s1", "count");
    AddWordVector(tokens, {"pattern", "a"});
    AddSpecialCharVector(tokens, {"(", "\""});
    AddWordVector(tokens, {"count"});
    AddSpecialCharVector(tokens, {"\"", ",", "_", ")"});
    AddEOF(tokens);

    controller.TokensToClauses(tokens);
  }

  SECTION(
      "Positive. stmt s1, assign a; Select s1 such that Uses(s1, \"count\") "
      "pattern a (_, _\"x\"_)") {
    AddDeclaration(tokens, "stmt", {"s1"});
    AddDeclaration(tokens, "assign", {"a"});
    AddWordVector(tokens, {"Select", "s1", "such", "that", "Uses"});
    AddWordIdent(tokens, "s1", "count");
    AddWordVector(tokens, {"pattern", "a"});
    AddSpecialCharVector(tokens, {"(", "_", ",", "_", "\""});
    AddWordVector(tokens, {"x"});
    AddSpecialCharVector(tokens, {"\"", "_", ")"});
    AddEOF(tokens);

    controller.TokensToClauses(tokens);
  }

  SECTION(
      "Positive. variable v, stmt s1, assign a; Select s1 such that Uses(s1, "
      "\"count\") pattern a (v, _\"x\"_)") {
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

    controller.TokensToClauses(tokens);
  }

  SECTION(
      "Negative, undeclared synonym: assign a; Select a such that pattern b "
      "(\"x\", _)") {
    AddDeclaration(tokens, "assign", {"a"});
    AddWordVector(tokens, {"Select", "a", "such", "that", "pattern", "b"});
    AddIdentWild(tokens, "x");
    AddEOF(tokens);

    REQUIRE_THROWS(controller.TokensToClauses(tokens));
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

  SECTION(
      "stmt s1, s2; Select s1 such that Follows*(_, s2) and Modifies(s2, _)") {
    AddDeclaration(tokens, "stmt", {"s1", "s2"});
    AddWordVector(tokens, {"Select", "s1", "such", "that", "Follows*"});
    AddWildWord(tokens, "s2");
    AddWordVector(tokens, {"and"});
    AddWordVector(tokens, {"Modifies"});
    AddWordWild(tokens, "s2");
    AddEOF(tokens);

    controller.TokensToClauses(tokens);
  }
}

TEST_CASE("Select clauses with attr ref") {
  std::vector<std::shared_ptr<Token>> tokens;
  QPSController controller = QPSController();

  SECTION("Positive: stmt s1; Select s1.stmt# such that Follows(2, s1)") {
    AddDeclaration(tokens, "stmt", {"s1"});
    AddWordVector(tokens, {"Select", "s1"});
    AddSpecialCharVector(tokens, {"."});
    AddWordVector(tokens, {"stmt#"});
    AddWordVector(tokens, {"such", "that", "Follows"});
    AddIntWord(tokens, "2", "s1");
    AddEOF(tokens);

    controller.TokensToClauses(tokens);
  }

  SECTION(
      "Positive: procedure p1, p2; Select p1.procName such that Calls(p1, "
      "p2)") {
    AddDeclaration(tokens, "procedure", {"p1", "p2"});
    AddWordVector(tokens, {"Select", "p1"});
    AddSpecialCharVector(tokens, {"."});
    AddWordVector(tokens, {"procName"});
    AddWordVector(tokens, {"such", "that", "Calls"});
    AddWordWord(tokens, "p1", "p2");
    AddEOF(tokens);

    controller.TokensToClauses(tokens);
  }

  SECTION("Positive: read r1, r2; Select r1.varName such that Follows(2, r1)") {
    AddDeclaration(tokens, "read", {"r1", "r2"});
    AddWordVector(tokens, {"Select", "r1"});
    AddSpecialCharVector(tokens, {"."});
    AddWordVector(tokens, {"varName"});
    AddWordVector(tokens, {"such", "that", "Follows"});
    AddIntWord(tokens, "2", "r1");
    AddEOF(tokens);

    controller.TokensToClauses(tokens);
  }

  SECTION(
      "Positive: stmt s; procedure p, q; Select <s.stmt#, p.procName> such "
      "that Calls (p, q) and Follows (3, s)") {
    AddDeclaration(tokens, "stmt", {"s"});
    AddDeclaration(tokens, "procedure", {"p", "q"});
    AddWordVector(tokens, {"Select"});
    AddSpecialCharVector(tokens, {"<"});
    AddWordVector(tokens, {"s"});
    AddSpecialCharVector(tokens, {"."});
    AddWordVector(tokens, {"stmt#"});
    AddSpecialCharVector(tokens, {","});
    AddWordVector(tokens, {"p"});
    AddSpecialCharVector(tokens, {"."});
    AddWordVector(tokens, {"procName"});
    AddSpecialCharVector(tokens, {">"});
    AddWordVector(tokens, {"such", "that", "Calls"});
    AddWordWord(tokens, "p", "q");
    AddWordVector(tokens, {"and", "Follows"});
    AddIntWord(tokens, "3", "s");
    AddEOF(tokens);

    controller.TokensToClauses(tokens);
  }
}

TEST_CASE("Next queries") {
  std::vector<std::shared_ptr<Token>> tokens;
  QPSController controller = QPSController();

  SECTION("stmt s1, s2; Select s1 such that Next(3, s1)") {
    AddDeclaration(tokens, "stmt", {"s1", "s2"});
    AddWordVector(tokens, {"Select", "s1", "such", "that", "Next"});
    AddIntWord(tokens, "3", "s1");
    AddEOF(tokens);

    controller.TokensToClauses(tokens);
  }

  SECTION("stmt s1, s2; Select s1 such that Next*(3, s1)") {
    AddDeclaration(tokens, "stmt", {"s1", "s2"});
    AddWordVector(tokens, {"Select", "s1", "such", "that", "Next*"});
    AddIntWord(tokens, "3", "s1");
    AddEOF(tokens);

    controller.TokensToClauses(tokens);
  }
}

TEST_CASE("With queries") {
  std::vector<std::shared_ptr<Token>> tokens;
  QPSController controller = QPSController();

  SECTION(
      "procedure p, q; Select p such that Calls (p, q) with q.procName = "
      "\"hello\"") {
    AddDeclaration(tokens, "procedure", {"p", "q"});
    AddWordVector(tokens, {"Select", "p", "such", "that", "Calls"});
    AddWordWord(tokens, "p", "q");
    AddWordVector(tokens, {"with", "q"});
    AddSpecialCharVector(tokens, {"."});
    AddWordVector(tokens, {"procName"});
    AddSpecialCharVector(tokens, {"=", "\""});
    AddWordVector(tokens, {"hello"});
    AddSpecialCharVector(tokens, {"\""});
    AddEOF(tokens);

    controller.TokensToClauses(tokens);
  }

  SECTION("stmt s1; Select s1 with s1.stmt = 2") {
    AddDeclaration(tokens, "stmt", {"s1"});
    AddWordVector(tokens, {"Select", "s1", "with", "s1"});
    AddSpecialCharVector(tokens, {"."});
    AddWordVector(tokens, {"stmt"});
    AddSpecialCharVector(tokens, {"="});
    AddIntVector(tokens, {"2"});
    AddEOF(tokens);

    REQUIRE_THROWS_AS(controller.TokensToClauses(tokens),
                      InvalidSyntaxException);
  }

  SECTION("stmt s; constant c; Select s with s.stmt# = c.value") {
    AddDeclaration(tokens, "stmt", {"s"});
    AddDeclaration(tokens, "constant", {"c"});
    AddWordVector(tokens, {"Select", "s", "with", "s"});
    AddSpecialCharVector(tokens, {"."});
    AddWordVector(tokens, {"stmt#"});
    AddSpecialCharVector(tokens, {"="});
    AddWordVector(tokens, {"c"});
    AddSpecialCharVector(tokens, {"."});
    AddWordVector(tokens, {"value"});
    AddEOF(tokens);

    controller.TokensToClauses(tokens);
  }
  SECTION("procedure p; variable v; Select p with p.procName = v.varName") {
    AddDeclaration(tokens, "procedure", {"p"});
    AddDeclaration(tokens, "variable", {"v"});
    AddWordVector(tokens, {"Select", "p", "with", "p"});
    AddSpecialCharVector(tokens, {"."});
    AddWordVector(tokens, {"procName"});
    AddSpecialCharVector(tokens, {"="});
    AddWordVector(tokens, {"v"});
    AddSpecialCharVector(tokens, {"."});
    AddWordVector(tokens, {"varName"});
    AddEOF(tokens);

    controller.TokensToClauses(tokens);
  }
  SECTION(
      "procedure p, q; variable v; stmt s; assign stmt; constant c; Select s "
      "with s.stmt# = c.value and p.procName = v.varName such that Calls (p, "
      "q) and Uses (s, v) pattern stmt (_, _) with q.procName = \"hello\"") {
    AddDeclaration(tokens, "procedure", {"p", "q"});
    AddDeclaration(tokens, "variable", {"v"});
    AddDeclaration(tokens, "stmt", {"s"});
    AddDeclaration(tokens, "assign", {"stmt"});
    AddDeclaration(tokens, "constant", {"c"});
    AddWordVector(tokens, {"Select", "s", "with", "s"});
    AddSpecialCharVector(tokens, {"."});
    AddWordVector(tokens, {"stmt#"});
    AddSpecialCharVector(tokens, {"="});
    AddWordVector(tokens, {"c"});
    AddSpecialCharVector(tokens, {"."});
    AddWordVector(tokens, {"value", "and", "p"});
    AddSpecialCharVector(tokens, {"."});
    AddWordVector(tokens, {"procName"});
    AddSpecialCharVector(tokens, {"="});
    AddWordVector(tokens, {"v"});
    AddSpecialCharVector(tokens, {"."});
    AddWordVector(tokens, {"varName", "such", "that", "Calls"});
    AddWordWord(tokens, "p", "q");
    AddWordVector(tokens, {"and", "Uses"});
    AddWordWord(tokens, "s", "v");
    AddWordVector(tokens, {"pattern", "stmt"});
    AddWildWild(tokens);
    AddWordVector(tokens, {"with", "q"});
    AddSpecialCharVector(tokens, {"."});
    AddWordVector(tokens, {"procName"});
    AddSpecialCharVector(tokens, {"=", "\""});
    AddWordVector(tokens, {"hello"});
    AddSpecialCharVector(tokens, {"\""});
    AddEOF(tokens);

    controller.TokensToClauses(tokens);
  }
  SECTION("stmt s; Select s with 5 = 5") {
    AddDeclaration(tokens, "stmt", {"s"});
    AddWordVector(tokens, {"Select", "s", "with"});
    AddIntVector(tokens, {"5"});
    AddSpecialCharVector(tokens, {"="});
    AddIntVector(tokens, {"5"});
    AddEOF(tokens);

    controller.TokensToClauses(tokens);
  }
  SECTION("stmt s; Select s with \"abc\" = \"abc\"") {
    AddDeclaration(tokens, "stmt", {"s"});
    AddWordVector(tokens, {"Select", "s", "with"});
    AddSpecialCharVector(tokens, {"\""});
    AddWordVector(tokens, {"abc"});
    AddSpecialCharVector(tokens, {"\""});
    AddSpecialCharVector(tokens, {"="});
    AddSpecialCharVector(tokens, {"\""});
    AddWordVector(tokens, {"abc"});
    AddSpecialCharVector(tokens, {"\""});
    AddEOF(tokens);

    controller.TokensToClauses(tokens);
  }
  SECTION("stmt s; Select s with s.stmt# = 20000 such that Follows (5, s)") {
    AddDeclaration(tokens, "stmt", {"s"});
    AddWordVector(tokens, {"Select", "s", "with", "s"});
    AddSpecialCharVector(tokens, {"."});
    AddWordVector(tokens, {"stmt#"});
    AddSpecialCharVector(tokens, {"="});
    AddIntVector(tokens, {"20000"});
    AddWordVector(tokens, {"such", "that", "Follows"});
    AddIntWord(tokens, "5", "s");
    AddEOF(tokens);

    controller.TokensToClauses(tokens);
  }
  SECTION("stmt s; Select s with 5 = \"s\"") {
    AddDeclaration(tokens, "stmt", {"s"});
    AddWordVector(tokens, {"Select", "s", "with"});
    AddIntVector(tokens, {"5"});
    AddSpecialCharVector(tokens, {"=", "\""});
    AddWordVector(tokens, {"s"});
    AddSpecialCharVector(tokens, {"\""});
    AddEOF(tokens);

    REQUIRE_THROWS(controller.TokensToClauses(tokens));
  }
  SECTION("procedure p; Select p with 5 = p.procName") {
    AddDeclaration(tokens, "proedure", {"p"});
    AddWordVector(tokens, {"Select", "p", "with"});
    AddIntVector(tokens, {"5"});
    AddSpecialCharVector(tokens, {"=", "\""});
    AddWordVector(tokens, {"p"});
    AddSpecialCharVector(tokens, {"."});
    AddWordVector(tokens, {"procName"});
    AddEOF(tokens);

    REQUIRE_THROWS(controller.TokensToClauses(tokens));
  }
  SECTION(
      "Negative: stmt s; Select s with s1.stmt# = 20000 such that Follows (5, "
      "s)") {
    AddDeclaration(tokens, "stmt", {"s"});
    AddWordVector(tokens, {"Select", "s", "with", "s1"});
    AddSpecialCharVector(tokens, {"."});
    AddWordVector(tokens, {"stmt"});
    AddSpecialCharVector(tokens, {"#", "="});
    AddIntVector(tokens, {"20000"});
    AddWordVector(tokens, {"such", "that", "Follows"});
    AddIntWord(tokens, "5", "s");
    AddEOF(tokens);

    REQUIRE_THROWS(controller.TokensToClauses(tokens));
  }
}

TEST_CASE("pattern and") {
  std::vector<std::shared_ptr<Token>> tokens;
  QPSController controller = QPSController();

  SECTION(
      "stmt s1; assign a; Select a pattern a (\"v\", _) and a (\"v\", \"x\")") {
    AddDeclaration(tokens, "stmt", {"s1"});
    AddDeclaration(tokens, "assign", {"a"});
    AddWordVector(tokens, {"Select", "a", "pattern", "a"});
    AddIdentWild(tokens, "v");
    AddWordVector(tokens, {"and", "a"});
    AddIdentIdent(tokens, "v", "x");
    AddEOF(tokens);

    controller.TokensToClauses(tokens);
  }
}

TEST_CASE("Affects") {
  std::vector<std::shared_ptr<Token>> tokens;
  QPSController controller = QPSController();

  SECTION("stmt s1, s2; Select s1 such that Affects(s1, s2)") {
    AddDeclaration(tokens, "stmt", {"s1", "s2"});
    AddWordVector(tokens, {"Select", "s1", "such", "that", "Affects"});
    AddWordWord(tokens, "s1", "s2");
    AddEOF(tokens);

    controller.TokensToClauses(tokens);
  }
}

TEST_CASE("Valid/Invalid Integer") {
  std::vector<std::shared_ptr<Token>> tokens;
  QPSController controller = QPSController();

  SECTION("stmt s1; Select s1 such that Follows(01, s1)") {
    AddDeclaration(tokens, "stmt", {"s1"});
    AddWordVector(tokens, {"Select", "s1", "such", "that", "Follows"});
    AddIntWord(tokens, "01", "s1");
    AddEOF(tokens);

    REQUIRE_THROWS(controller.TokensToClauses(tokens));
  }

  SECTION("constant c; Select c with c.value = 0") {
    AddDeclaration(tokens, "constant", {"c"});
    AddWordVector(tokens, {"Select", "c", "with", "c"});
    AddSpecialCharVector(tokens, {"."});
    AddWordVector(tokens, {"value"});
    AddSpecialCharVector(tokens, {"="});
    AddIntVector(tokens, {"0"});
    AddEOF(tokens);

    controller.TokensToClauses(tokens);
  }

  SECTION("stmt s1; constant c; Select c with c.value = 01") {
    AddDeclaration(tokens, "constant", {"c"});
    AddWordVector(tokens, {"Select", "c", "with", "c"});
    AddSpecialCharVector(tokens, {"."});
    AddWordVector(tokens, {"value"});
    AddSpecialCharVector(tokens, {"="});
    AddIntVector(tokens, {"01"});
    AddEOF(tokens);

    REQUIRE_THROWS(controller.TokensToClauses(tokens));
  }
}

TEST_CASE("not clauses") {
  std::vector<std::shared_ptr<Token>> tokens;
  QPSController controller = QPSController();

  SECTION("stmt s1; Select s1 such that not Follows(2, s1)") {
    AddDeclaration(tokens, "stmt", {"s1"});
    AddWordVector(tokens, {"Select", "s1", "such", "that", "not", "Follows"});
    AddIntWord(tokens, "2", "s1");
    AddEOF(tokens);

    controller.TokensToClauses(tokens);
  }

  SECTION("assign a; Select a pattern not a (\"x\", _\"y\"_)") {
    AddDeclaration(tokens, "assign", {"a"});
    AddWordVector(tokens, {"Select", "a", "pattern", "not", "a"});
    AddSpecialCharVector(tokens, {"(", "\""});
    AddWordVector(tokens, {"x"});
    AddSpecialCharVector(tokens, {"\"", ",", "_", "\""});
    AddWordVector(tokens, {"y"});
    AddSpecialCharVector(tokens, {"\"", "_", ")"});
    AddEOF(tokens);

    controller.TokensToClauses(tokens);
  }

  SECTION("stmt s1; Select s1 with not s1.stmt# = 10") {
    AddDeclaration(tokens, "stmt", {"s1"});
    AddWordVector(tokens, {"Select", "s1", "with", "not", "s1"});
    AddSpecialCharVector(tokens, {"."});
    AddWordVector(tokens, {"stmt#"});
    AddSpecialCharVector(tokens, {"="});
    AddIntVector(tokens, {"10"});
    AddEOF(tokens);

    controller.TokensToClauses(tokens);
  }

  SECTION(
      "stmt s1, s2; Select <s1, s2> such that Follows(2, s1) and not "
      "Follows*(10, s2)") {
    AddDeclaration(tokens, "stmt", {"s1", "s2"});
    AddWordVector(tokens, {"Select"});
    AddSpecialCharVector(tokens, {"<"});
    AddWordVector(tokens, {"s1"});
    AddSpecialCharVector(tokens, {","});
    AddWordVector(tokens, {"s2"});
    AddSpecialCharVector(tokens, {">"});
    AddWordVector(tokens, {"such", "that", "not", "Follows"});
    AddIntWord(tokens, "2", "s1");
    AddWordVector(tokens, {"and", "not", "Follows*"});
    AddIntWord(tokens, "10", "s2");
    AddEOF(tokens);

    controller.TokensToClauses(tokens);
  }
}

TEST_CASE("Pattern While Clauses") {
  std::vector<std::shared_ptr<Token>> tokens;
  QPSController controller = QPSController();

  SECTION("Positive, simple: while w; variable v; Select w pattern w (v, _)") {
    AddDeclaration(tokens, "while", {"w"});
    AddDeclaration(tokens, "variable", {"v"});
    AddWordVector(tokens, {"Select", "w", "pattern", "w"});
    AddWordWild(tokens, "v");
    AddEOF(tokens);

    controller.TokensToClauses(tokens);
  }

  SECTION("Positive, double _: while w; Select w pattern w (_, _)") {
    AddDeclaration(tokens, "while", {"w"});
    AddWordVector(tokens, {"Select", "w", "pattern", "w"});
    AddWildWild(tokens);
    AddEOF(tokens);

    controller.TokensToClauses(tokens);
  }

  SECTION(
      "Positive: while w1, w2; Select <w1, w2> pattern w1 (v, _) and w2 (x, "
      "_)") {
    AddDeclaration(tokens, "while", {"w1", "w2"});
    AddDeclaration(tokens, "variable", {"v", "x"});
    AddWordVector(tokens, {"Select"});
    AddSpecialCharVector(tokens, {"<"});
    AddWordVector(tokens, {"w1"});
    AddSpecialCharVector(tokens, {","});
    AddWordVector(tokens, {"w2"});
    AddSpecialCharVector(tokens, {">"});
    AddWordVector(tokens, {"pattern", "w1"});
    AddWordWild(tokens, "v");
    AddWordVector(tokens, {"and", "w2"});
    AddWordWild(tokens, "x");
    AddEOF(tokens);

    controller.TokensToClauses(tokens);
  }

  SECTION("Negative: while w; Select w pattern w (v, _)") {
    AddDeclaration(tokens, "while", {"w"});
    AddWordVector(tokens, {"Select", "w", "pattern", "w"});
    AddWordWild(tokens, "v");
    AddEOF(tokens);

    REQUIRE_THROWS_AS(controller.TokensToClauses(tokens),
                      InvalidSemanticsException);
  }

  SECTION("Negative: while w; stmt s1; Select w pattern w (s1, _)") {
    AddDeclaration(tokens, "while", {"w"});
    AddDeclaration(tokens, "stmt", {"s1"});
    AddWordVector(tokens, {"Select", "w", "pattern", "w"});
    AddWordWild(tokens, "s1");
    AddEOF(tokens);

    REQUIRE_THROWS_AS(controller.TokensToClauses(tokens),
                      InvalidSemanticsException);
  }

  SECTION("Negative: while w; stmt s1; Select w patternn w (s1, _)") {
    AddDeclaration(tokens, "while", {"w"});
    AddDeclaration(tokens, "stmt", {"s1"});
    AddWordVector(tokens, {"Select", "w", "patternn", "w"});
    AddWordWild(tokens, "s1");
    AddEOF(tokens);

    REQUIRE_THROWS_AS(controller.TokensToClauses(tokens),
                      InvalidSyntaxException);
  }
}

TEST_CASE("Pattern If Clauses") {
  std::vector<std::shared_ptr<Token>> tokens;
  QPSController controller = QPSController();

  SECTION("Positive, simple: if i; variable v; Select i pattern i (v, _, _)") {
    AddDeclaration(tokens, "if", {"i"});
    AddDeclaration(tokens, "variable", {"v"});
    AddWordVector(tokens, {"Select", "i", "pattern", "i"});
    AddSpecialCharVector(tokens, {"("});
    AddWordVector(tokens, {"v"});
    AddSpecialCharVector(tokens, {",", "_", ",", "_", ")"});
    AddEOF(tokens);

    controller.TokensToClauses(tokens);
  }

  SECTION("Positive, all _: if i; Select i pattern i (_, _, _)") {
    AddDeclaration(tokens, "if", {"i"});
    AddWordVector(tokens, {"Select", "i", "pattern", "i"});
    AddSpecialCharVector(tokens, {"(", "_", ",", "_", ",", "_", ")"});
    AddEOF(tokens);

    controller.TokensToClauses(tokens);
  }

  SECTION(
      "Positive: if i1, i2; Select <i1, i2> pattern i1 (v, _, _) and i2 (x, _, "
      "_)") {
    AddDeclaration(tokens, "if", {"i1", "i2"});
    AddDeclaration(tokens, "variable", {"v", "x"});
    AddWordVector(tokens, {"Select"});
    AddSpecialCharVector(tokens, {"<"});
    AddWordVector(tokens, {"i1"});
    AddSpecialCharVector(tokens, {","});
    AddWordVector(tokens, {"i2"});
    AddSpecialCharVector(tokens, {">"});
    AddWordVector(tokens, {"pattern", "i1"});
    AddSpecialCharVector(tokens, {"("});
    AddWordVector(tokens, {"v"});
    AddSpecialCharVector(tokens, {",", "_", ",", "_", ")"});
    AddWordVector(tokens, {"and", "i2"});
    AddSpecialCharVector(tokens, {"("});
    AddWordVector(tokens, {"x"});
    AddSpecialCharVector(tokens, {",", "_", ",", "_", ")"});
    AddEOF(tokens);

    controller.TokensToClauses(tokens);
  }

  SECTION("Negative: if i; Select i pattern i (v, _, _)") {
    AddDeclaration(tokens, "if", {"i"});
    AddWordVector(tokens, {"Select", "i", "pattern", "i"});
    AddSpecialCharVector(tokens, {"("});
    AddWordVector(tokens, {"v"});
    AddSpecialCharVector(tokens, {",", "_", ",", "_", ")"});
    AddEOF(tokens);

    REQUIRE_THROWS_AS(controller.TokensToClauses(tokens),
                      InvalidSemanticsException);
  }

  SECTION("Negative: if i; stmt s1; Select i pattern i (s1, _, _)") {
    AddDeclaration(tokens, "if", {"i"});
    AddDeclaration(tokens, "stmt", {"s1"});
    AddWordVector(tokens, {"Select", "i", "pattern", "i"});
    AddSpecialCharVector(tokens, {"("});
    AddWordVector(tokens, {"s1"});
    AddSpecialCharVector(tokens, {",", "_", ",", "_", ")"});
    AddEOF(tokens);

    REQUIRE_THROWS_AS(controller.TokensToClauses(tokens),
                      InvalidSemanticsException);
  }

  SECTION("Negative: if i; stmt s1; Select i patternn i (s1, _, _)") {
    AddDeclaration(tokens, "if", {"i"});
    AddDeclaration(tokens, "stmt", {"s1"});
    AddWordVector(tokens, {"Select", "i", "patternn", "i"});
    AddSpecialCharVector(tokens, {"("});
    AddWordVector(tokens, {"s1"});
    AddSpecialCharVector(tokens, {",", "_", ",", "_", ")"});
    AddEOF(tokens);

    REQUIRE_THROWS_AS(controller.TokensToClauses(tokens),
                      InvalidSyntaxException);
  }

  SECTION("Negative: if i; Select w pattern w (_, _, _)") {
    AddDeclaration(tokens, "if", {"i"});
    AddWordVector(tokens, {"Select", "w", "pattern", "w"});
    AddSpecialCharVector(tokens, {"(", "_", ",", "_", ",", "_", ")"});
    AddEOF(tokens);

    REQUIRE_THROWS_AS(controller.TokensToClauses(tokens),
                      InvalidSemanticsException);
  }

  SECTION("Negative: if i; Selec w pattern w (_, _, _)") {
    AddDeclaration(tokens, "if", {"i"});
    AddWordVector(tokens, {"Selec", "w", "pattern", "w"});
    AddSpecialCharVector(tokens, {"(", "_", ",", "_", ",", "_", ")"});
    AddEOF(tokens);

    REQUIRE_THROWS_AS(controller.TokensToClauses(tokens),
                      InvalidSyntaxException);
  }
}
