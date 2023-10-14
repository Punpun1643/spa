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

  SECTION("Evaluation logic") {
    // Test each of the combinations in turn.
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
