#include "catch.hpp"
#include "../../stub/PkbQpsInterfaceStub.h"
#include "query_processing_system/common/WithClause.h"
#include "query_processing_system/exceptions/InvalidSemanticsException.h"

TEST_CASE("Test WithClause") {
  PkbQpsInterfaceStub pkb = PkbQpsInterfaceStub();
  WithClause with_clause = WithClause(1, 1);
  AttrRef attr_ref_proc = AttrRef(PqlDeclaration("proc", EntityType::PROCEDURE));
  AttrRef attr_ref_var = AttrRef(PqlDeclaration("var", EntityType::VARIABLE));
  AttrRef attr_ref_stmt = AttrRef(PqlDeclaration("s", EntityType::STMT));
  AttrRef attr_ref_const = AttrRef(PqlDeclaration("c", EntityType::CONSTANT));
  AttrRef attr_ref_read_alias = AttrRef(PqlDeclaration("read", EntityType::READ), AttrType::VAR_NAME);
  AttrRef attr_ref_read = AttrRef(PqlDeclaration("read", EntityType::READ));
  AttrRef attr_ref_call_alias = AttrRef(PqlDeclaration("call", EntityType::CALL), AttrType::PROC_NAME);

  SECTION("Constructor tests") {
    REQUIRE_NOTHROW(WithClause(1, 23));
    REQUIRE_NOTHROW(WithClause("la", "abc"));
    REQUIRE_NOTHROW(WithClause(attr_ref_stmt, attr_ref_const));
    REQUIRE_NOTHROW(WithClause(attr_ref_read, attr_ref_stmt));
    REQUIRE_NOTHROW(WithClause(attr_ref_proc, attr_ref_var));
  }

  SECTION("Test Int-Int evaluation logic") {
    with_clause = WithClause(42, 42);
    auto result = with_clause.evaluate(pkb);
    REQUIRE(result->isBooleanResult());
    REQUIRE(result->getBooleanClauseValue());
    REQUIRE(result->getDeclarations().empty());

    with_clause = WithClause(2, 16);
    result = with_clause.evaluate(pkb);
    REQUIRE(result->isBooleanResult());
    REQUIRE_FALSE(result->getBooleanClauseValue());
    REQUIRE(result->getDeclarations().empty());
  }

  SECTION("Test Str-Str evaluation logic") {
    with_clause = WithClause("varName", "varName");
    auto result = with_clause.evaluate(pkb);
    REQUIRE(result->isBooleanResult());
    REQUIRE(result->getBooleanClauseValue());
    REQUIRE(result->getDeclarations().empty());

    with_clause = WithClause("abc", "def");
    result = with_clause.evaluate(pkb);
    REQUIRE(result->isBooleanResult());
    REQUIRE_FALSE(result->getBooleanClauseValue());
    REQUIRE(result->getDeclarations().empty());
  }

  SECTION("Test Int-AttrRef evaluation logic") {
    with_clause = WithClause(52, attr_ref_stmt);
    auto result = with_clause.evaluate(pkb);
    REQUIRE(result->getNumDeclarations() == 1);
    REQUIRE(pkb.last_value_passed == "52");
    REQUIRE(pkb.last_entity_type_passed == EntityType::STMT);
    REQUIRE(pkb.last_attr_type_passed == AttrType::STMT_NUM);
    REQUIRE(result->getDeclarations() == std::vector<PqlDeclaration>{attr_ref_stmt.GetDecl()});
    REQUIRE(*(result->getValues(attr_ref_stmt.GetDecl())) == pkb.attr_value_matches);

    with_clause = WithClause(attr_ref_const, 21);
    result = with_clause.evaluate(pkb);
    REQUIRE(result->getNumDeclarations() == 1);
    REQUIRE(pkb.last_value_passed == "21");
    REQUIRE(pkb.last_entity_type_passed == EntityType::CONSTANT);
    REQUIRE(pkb.last_attr_type_passed == AttrType::VALUE);
    REQUIRE(result->getDeclarations() == std::vector<PqlDeclaration>{attr_ref_const.GetDecl()});
    REQUIRE(*(result->getValues(attr_ref_const.GetDecl())) == pkb.attr_value_matches);
  }

  SECTION("Test String-AttrRef evaluation logic") {
    with_clause = WithClause("abc", attr_ref_var);
    auto result = with_clause.evaluate(pkb);
    REQUIRE(result->getNumDeclarations() == 1);
    REQUIRE(pkb.last_value_passed == "abc");
    REQUIRE(pkb.last_entity_type_passed == EntityType::VARIABLE);
    REQUIRE(pkb.last_attr_type_passed == AttrType::VAR_NAME);
    REQUIRE(result->getDeclarations() == std::vector<PqlDeclaration>{attr_ref_var.GetDecl()});
    REQUIRE(*(result->getValues(attr_ref_var.GetDecl())) == pkb.attr_value_matches);

    with_clause = WithClause(attr_ref_call_alias, "iamastring");
    result = with_clause.evaluate(pkb);
    REQUIRE(result->getNumDeclarations() == 1);
    REQUIRE(pkb.last_value_passed == "iamastring");
    REQUIRE(pkb.last_entity_type_passed == EntityType::CALL);
    REQUIRE(pkb.last_attr_type_passed == AttrType::PROC_NAME);
    REQUIRE(result->getDeclarations() == std::vector<PqlDeclaration>{attr_ref_call_alias.GetDecl()});
    REQUIRE(*(result->getValues(attr_ref_call_alias.GetDecl())) == pkb.attr_value_matches);
  }

  SECTION("Test AttrRef-AttrRef evaluation logic") {

  }

  SECTION("Incompatible args should throw semantic error") {
    REQUIRE_THROWS_AS(WithClause(1, "abc"), InvalidSemanticsException);
    REQUIRE_THROWS_AS(WithClause("abc", 1), InvalidSemanticsException);
    REQUIRE_THROWS_AS(WithClause(attr_ref_proc, 2), InvalidSemanticsException);
    REQUIRE_THROWS_AS(WithClause(2, attr_ref_var), InvalidSemanticsException);
    REQUIRE_THROWS_AS(WithClause(attr_ref_stmt, "wwdwdw"), InvalidSemanticsException);
    REQUIRE_THROWS_AS(WithClause("abc", attr_ref_const), InvalidSemanticsException);

    REQUIRE_THROWS_AS(WithClause(attr_ref_stmt, attr_ref_read_alias), InvalidSemanticsException);
    REQUIRE_THROWS_AS(WithClause(attr_ref_call_alias, attr_ref_const), InvalidSemanticsException);
    REQUIRE_NOTHROW(WithClause(attr_ref_call_alias, attr_ref_var));
    REQUIRE_NOTHROW(WithClause(attr_ref_read_alias, attr_ref_proc));
  }
}
