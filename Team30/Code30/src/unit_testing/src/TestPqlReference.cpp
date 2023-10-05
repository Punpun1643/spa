#include "catch.hpp"
#include "query_processing_system/common/EntRef.h"
#include "query_processing_system/common/PqlReference.h"
#include "query_processing_system/common/StmtRef.h"
#include "query_processing_system/exceptions/InvalidSemanticsException.h"

TEST_CASE("Tests on PqlReferences") {
  // Declarations
  auto a = PqlDeclaration("a", EntityType::ASSIGN);
  auto print = PqlDeclaration("print", EntityType::PRINT);
  auto c = PqlDeclaration("constant", EntityType::CONSTANT);
  auto v = PqlDeclaration("variable", EntityType::VARIABLE);
  auto proc = PqlDeclaration("procedure", EntityType::PROCEDURE);
  auto s = PqlDeclaration("stmt", EntityType::STMT);

  SECTION("StmtRef") {
    auto wild_stmt_ref = std::make_shared<PqlReference>(StmtRef());
    auto value_stmt_ref = std::make_shared<PqlReference>(StmtRef(42));
    auto decl_stmt_ref = std::make_shared<PqlReference>(
        StmtRef(a));

    REQUIRE(wild_stmt_ref->getRefType() == PqlRefType::WILD);
    REQUIRE(value_stmt_ref->getRefType() == PqlRefType::VALUE);
    REQUIRE(decl_stmt_ref->getRefType() == PqlRefType::DECLARATION);

    REQUIRE(value_stmt_ref->getValue() == "42");
    REQUIRE(decl_stmt_ref->getDeclarationType() == a.getEntityType());
    REQUIRE(decl_stmt_ref->getDeclaration() == a);

    // Copying works
    auto ref = *value_stmt_ref;
    auto copied_ref = ref;

    auto ref_2 = *decl_stmt_ref;
    auto copied_ref_2 = ref_2;

    // Invalid input declaration types
    REQUIRE_THROWS_AS(StmtRef(c),
                      InvalidSemanticsException);
    REQUIRE_THROWS_AS(StmtRef(v),
                      InvalidSemanticsException);
    REQUIRE_THROWS_AS(StmtRef(proc),
                      InvalidSemanticsException);
  }

  SECTION("EntRef") {
    auto wild_ent_ref = std::make_shared<PqlReference>(EntRef());
    auto value_ent_ref = std::make_shared<PqlReference>(EntRef("variable"));
    auto decl_ent_ref = std::make_shared<PqlReference>(
        EntRef(v));

    REQUIRE(wild_ent_ref->getRefType() == PqlRefType::WILD);
    REQUIRE(value_ent_ref->getRefType() == PqlRefType::VALUE);
    REQUIRE(decl_ent_ref->getRefType() == PqlRefType::DECLARATION);

    REQUIRE(value_ent_ref->getValue() == "variable");
    REQUIRE(decl_ent_ref->getDeclarationType() == v.getEntityType());
    REQUIRE(decl_ent_ref->getDeclaration() == v);

    // Copying works
    auto ref = *value_ent_ref;
    auto copied_ref = ref;

    auto ref_2 = *decl_ent_ref;
    auto copied_ref_2 = ref_2;

    // Invalid input declaration types
    REQUIRE_THROWS_AS(EntRef(a),
                      InvalidSemanticsException);
    REQUIRE_THROWS_AS(EntRef(s),
                      InvalidSemanticsException);
    REQUIRE_THROWS_AS(EntRef(print),
                      InvalidSemanticsException);
  }
}
