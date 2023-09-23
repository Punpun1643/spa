#include "catch.hpp"
#include "query_processing_system/common/PqlReference.h"
#include "query_processing_system/exceptions/InvalidSemanticsException.h"
#include "query_processing_system/common/StmtRef.h"
#include "query_processing_system/common/EntRef.h"

TEST_CASE("Tests on PqlReferences") {
  // Declarations
  auto a = PqlDeclaration(std::make_shared<std::string>("a"), EntityType::ASSIGN);
  auto b = PqlDeclaration(std::make_shared<std::string>("b"), EntityType::PRINT);
  auto c = PqlDeclaration(std::make_shared<std::string>("constant"), EntityType::CONSTANT);
  auto v = PqlDeclaration(std::make_shared<std::string>("variable"), EntityType::VARIABLE);
  auto s = PqlDeclaration(std::make_shared<std::string>("stmt"), EntityType::STMT);

  SECTION ("StmtRef") {
    auto wild_stmt_ref = std::make_shared<PqlReference>(StmtRef());
    auto value_stmt_ref = std::make_shared<PqlReference>(StmtRef(5));
    auto decl_stmt_ref = std::make_shared<PqlReference>(StmtRef(std::make_shared<PqlDeclaration>(a)));

    REQUIRE(wild_stmt_ref->getRefType() == WILD);
    REQUIRE(value_stmt_ref->getRefType() == VALUE);
    REQUIRE(decl_stmt_ref->getRefType() == DECLARATION);

    // Copying works
    auto ref = *value_stmt_ref;
    auto copied_ref = ref;

    auto ref_2 = *decl_stmt_ref;
    auto copied_ref_2 = ref_2;

    REQUIRE_THROWS_AS(StmtRef(std::make_shared<PqlDeclaration>(c)), InvalidSemanticsException);
  }
}