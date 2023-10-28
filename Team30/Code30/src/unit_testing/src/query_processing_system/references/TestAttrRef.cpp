#include "../../stub/PkbQpsInterfaceStub.h"
#include "catch.hpp"
#include "query_processing_system/exceptions/InvalidSemanticsException.h"
#include "query_processing_system/references/AttrRef.h"
#include "shared/types/AttrType.h"

TEST_CASE("Test AttrRef") {
  PqlDeclaration con = PqlDeclaration("const", EntityType::CONSTANT);
  PqlDeclaration read = PqlDeclaration("read", EntityType::READ);
  PqlDeclaration print = PqlDeclaration("print", EntityType::PRINT);
  PqlDeclaration call = PqlDeclaration("c", EntityType::CALL);
  PqlDeclaration a = PqlDeclaration("assign", EntityType::ASSIGN);
  PqlDeclaration proc = PqlDeclaration("procedure", EntityType::PROCEDURE);
  PkbQpsInterfaceStub pkb = PkbQpsInterfaceStub();

  SECTION("Test basic functionality") {
    // Default argument initialization
    auto ref = AttrRef(read);
    REQUIRE_FALSE(ref.IsAttrTypeAnAlias());
    REQUIRE(ref.GetDecl() == read);
    REQUIRE(ref.GetEntityType() == read.GetEntityType());
    REQUIRE(ref.GetAttrType() == AttrType::STMT_NUM);
    REQUIRE(ref.GetOutputType() == AttrRefOutputType::INTEGER);
    REQUIRE_FALSE(ref.GetDecl() == print);

    ref = AttrRef(print);
    REQUIRE_FALSE(ref.IsAttrTypeAnAlias());
    REQUIRE(ref.GetDecl() == print);
    REQUIRE_FALSE(ref.GetDecl() == con);

    ref = AttrRef(con);
    REQUIRE_FALSE(ref.IsAttrTypeAnAlias());
    REQUIRE(ref.GetEntityType() == con.GetEntityType());
    REQUIRE(ref.GetAttrType() == AttrType::VALUE);
    REQUIRE(ref.GetOutputType() == AttrRefOutputType::INTEGER);
    REQUIRE(ref.GetDecl() == con);
    REQUIRE_FALSE(ref.GetDecl() == a);

    ref = AttrRef(proc);
    REQUIRE_FALSE(ref.IsAttrTypeAnAlias());
    REQUIRE(ref.GetEntityType() == proc.GetEntityType());
    REQUIRE(ref.GetAttrType() == AttrType::PROC_NAME);
    REQUIRE(ref.GetOutputType() == AttrRefOutputType::NAME);
    REQUIRE(ref.GetDecl() == proc);
    REQUIRE_FALSE(ref.GetDecl() == a);

    // Two argument initialization
    ref = AttrRef(print, AttrType::VAR_NAME);
    REQUIRE(ref.IsAttrTypeAnAlias());
    REQUIRE(ref.GetEntityType() == print.GetEntityType());
    REQUIRE(ref.GetAttrType() == AttrType::VAR_NAME);
    REQUIRE(ref.GetOutputType() == AttrRefOutputType::NAME);
    REQUIRE_FALSE(ref.GetDecl() == a);
    REQUIRE(ref.GetDecl() == print);

    ref = AttrRef(call, AttrType::PROC_NAME);
    REQUIRE(ref.IsAttrTypeAnAlias());
    REQUIRE(ref.GetDecl() == call);
  }

  SECTION("Test GetAliasFromDefault") {
    // If attrRef is already of type 'default' value, does nothing
    REQUIRE(AttrRef(read).GetAliasFromDefault(pkb, "25") == "25");
    REQUIRE(AttrRef(print).GetAliasFromDefault(pkb, "42") == "42");
    REQUIRE(AttrRef(con).GetAliasFromDefault(pkb, "23") == "23");
    REQUIRE(AttrRef(call).GetAliasFromDefault(pkb, "1000") == "1000");

    // if non-default value is passed
    auto ref = AttrRef(read, AttrType::VAR_NAME);
    pkb.converted_entity = "random_name";
    auto output = ref.GetAliasFromDefault(pkb, "20");
    REQUIRE(pkb.last_value_passed == "20");
    REQUIRE(pkb.last_entity_type_passed == read.GetEntityType());
    REQUIRE(pkb.last_attr_type_2_passed == AttrType::VAR_NAME);
    REQUIRE(output == "random_name");

    ref = AttrRef(call, AttrType::PROC_NAME);
    pkb.converted_entity = "blah";
    output = ref.GetAliasFromDefault(pkb, "12");
    REQUIRE(pkb.last_value_passed == "12");
    REQUIRE(pkb.last_entity_type_passed == call.GetEntityType());
    REQUIRE(pkb.last_attr_type_2_passed == AttrType::PROC_NAME);
    REQUIRE(output == "blah");
  }

  SECTION("Test invalid entity-type attr-type combinations") {
    std::vector<EntityType> stmt_types = {EntityType::READ,   EntityType::PRINT,
                                          EntityType::ASSIGN, EntityType::IF,
                                          EntityType::WHILE,  EntityType::CALL,
                                          EntityType::STMT};
    for (EntityType type : stmt_types) {
      REQUIRE_THROWS_AS(AttrRef(PqlDeclaration("blah", type), AttrType::VALUE),
                        InvalidSemanticsException);
      if (type != EntityType::CALL) {
        REQUIRE_THROWS_AS(
            AttrRef(PqlDeclaration("blah", type), AttrType::PROC_NAME),
            InvalidSemanticsException);
      } else {
        REQUIRE_NOTHROW(
            AttrRef(PqlDeclaration("blah", type), AttrType::PROC_NAME));
      }
      if (type != EntityType::READ && type != EntityType::PRINT) {
        REQUIRE_THROWS_AS(
            AttrRef(PqlDeclaration("blah", type), AttrType::VAR_NAME),
            InvalidSemanticsException);
      } else {
        REQUIRE_NOTHROW(AttrRef(PqlDeclaration("b", type), AttrType::VAR_NAME));
      }
      REQUIRE_NOTHROW(
          AttrRef(PqlDeclaration("blah", type), AttrType::STMT_NUM));
    }
    // Just try out another to make sure the logic works
    REQUIRE_NOTHROW(AttrRef(con, AttrType::VALUE));
    REQUIRE_THROWS_AS(AttrRef(con, AttrType::VAR_NAME),
                      InvalidSemanticsException);
  }
}
