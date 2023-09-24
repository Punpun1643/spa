#include "catch.hpp"
#include "query_processing_system/common/EntRef.h"
#include "query_processing_system/common/PqlReference.h"
#include "query_processing_system/common/StmtRef.h"
#include "query_processing_system/exceptions/InvalidSemanticsException.h"

TEST_CASE("Tests on PqlReferences") {
  // Declarations
  auto a =
      PqlDeclaration(std::make_shared<std::string>("a"), EntityType::ASSIGN);
  auto print =
      PqlDeclaration(std::make_shared<std::string>("print"), EntityType::PRINT);
  auto c = PqlDeclaration(std::make_shared<std::string>("constant"),
                          EntityType::CONSTANT);
  auto v = PqlDeclaration(std::make_shared<std::string>("variable"),
                          EntityType::VARIABLE);
  auto proc = PqlDeclaration(std::make_shared<std::string>("procedure"),
                             EntityType::PROCEDURE);
  auto s =
      PqlDeclaration(std::make_shared<std::string>("stmt"), EntityType::STMT);

  SECTION("StmtRef") {
    auto wild_stmt_ref = std::make_shared<PqlReference>(StmtRef());
    auto value_stmt_ref = std::make_shared<PqlReference>(StmtRef(42));
    auto decl_stmt_ref = std::make_shared<PqlReference>(
        StmtRef(std::make_shared<PqlDeclaration>(a)));

    REQUIRE(wild_stmt_ref->getRefType() == WILD);
    REQUIRE(value_stmt_ref->getRefType() == VALUE);
    REQUIRE(decl_stmt_ref->getRefType() == DECLARATION);

    REQUIRE(value_stmt_ref->getValue() == "42");
    REQUIRE(decl_stmt_ref->getDeclarationType() == a.getEntityType());
    REQUIRE(*(decl_stmt_ref->getDeclaration()) == a);

    // Copying works
    auto ref = *value_stmt_ref;
    auto copied_ref = ref;

    auto ref_2 = *decl_stmt_ref;
    auto copied_ref_2 = ref_2;

    // Invalid input declaration types
    REQUIRE_THROWS_AS(StmtRef(std::make_shared<PqlDeclaration>(c)),
                      InvalidSemanticsException);
    REQUIRE_THROWS_AS(StmtRef(std::make_shared<PqlDeclaration>(v)),
                      InvalidSemanticsException);
    REQUIRE_THROWS_AS(StmtRef(std::make_shared<PqlDeclaration>(proc)),
                      InvalidSemanticsException);
  }

  SECTION("EntRef") {
    auto wild_ent_ref = std::make_shared<PqlReference>(EntRef());
    auto value_ent_ref = std::make_shared<PqlReference>(EntRef("variable"));
    auto decl_ent_ref = std::make_shared<PqlReference>(
        EntRef(std::make_shared<PqlDeclaration>(v)));

    REQUIRE(wild_ent_ref->getRefType() == WILD);
    REQUIRE(value_ent_ref->getRefType() == VALUE);
    REQUIRE(decl_ent_ref->getRefType() == DECLARATION);

    REQUIRE(value_ent_ref->getValue() == "variable");
    REQUIRE(decl_ent_ref->getDeclarationType() == v.getEntityType());
    REQUIRE(*(decl_ent_ref->getDeclaration()) == v);

    // Copying works
    auto ref = *value_ent_ref;
    auto copied_ref = ref;

    auto ref_2 = *decl_ent_ref;
    auto copied_ref_2 = ref_2;

    // Invalid input declaration types
    REQUIRE_THROWS_AS(EntRef(std::make_shared<PqlDeclaration>(a)),
                      InvalidSemanticsException);
    REQUIRE_THROWS_AS(EntRef(std::make_shared<PqlDeclaration>(s)),
                      InvalidSemanticsException);
    REQUIRE_THROWS_AS(EntRef(std::make_shared<PqlDeclaration>(print)),
                      InvalidSemanticsException);
  }
}
