#include "PkbQpsInterfaceStub.h"
#include "catch.hpp"
#include "query_processing_system/common/Clause.h"
#include "query_processing_system/common/FollowsClause.h"
#include "query_processing_system/common/ModifiesPClause.h"
#include "query_processing_system/common/ModifiesSClause.h"
#include "query_processing_system/common/ParentClause.h"
#include "query_processing_system/exceptions/InvalidSemanticsException.h"

TEST_CASE("Test SuchThat Clauses") {
  PkbQpsInterfaceStub pkb = PkbQpsInterfaceStub();
  std::unique_ptr<ClauseResult> result;

  PqlDeclaration s = PqlDeclaration("stmt", EntityType::STMT);
  PqlDeclaration v = PqlDeclaration("var", EntityType::VARIABLE);
  PqlDeclaration re = PqlDeclaration("read", EntityType::READ);
  PqlDeclaration print = PqlDeclaration("print", EntityType::PRINT);
  PqlDeclaration proc = PqlDeclaration("procedure", EntityType::PROCEDURE);

  SECTION("Follows/* Clauses") {
    // Also tests wildWild and valueWild
    FollowsClause follows = FollowsClause(std::make_unique<StmtRef>(),
                                          std::make_unique<StmtRef>(), false);
    FollowsClause follows_star = FollowsClause(std::make_unique<StmtRef>(1),
                                          std::make_unique<StmtRef>(), true);
    REQUIRE(pkb.wildWildCalls == 0);
    result = follows.evaluate(pkb);
    REQUIRE(pkb.wildWildCalls == 1);
    REQUIRE(pkb.last_rel_passed == RelationType::FOLLOWS);
    REQUIRE(result->isBooleanResult());
    REQUIRE(result->getBooleanClauseValue() == pkb.wildWildBool);

    REQUIRE(pkb.valueWildCalls == 0);
    result = follows_star.evaluate(pkb);
    REQUIRE(pkb.valueWildCalls == 1);
    REQUIRE(pkb.last_rel_passed == RelationType::FOLLOWS_STAR);
    REQUIRE(result->isBooleanResult());
    REQUIRE(result->getBooleanClauseValue() == pkb.valueWildBool);
  }

  SECTION("Parent/* Clauses") {
    // Also tests wildValue and valueValue
    ParentClause parent = ParentClause(std::make_unique<StmtRef>(),
                                          std::make_unique<StmtRef>(1), false);
    ParentClause parent_star = ParentClause(std::make_unique<StmtRef>(1),
                                               std::make_unique<StmtRef>(2), true);
    REQUIRE(pkb.wildValueCalls == 0);
    result = parent.evaluate(pkb);
    REQUIRE(pkb.wildValueCalls == 1);
    REQUIRE(pkb.last_rel_passed == RelationType::PARENT);
    REQUIRE(result->isBooleanResult());
    REQUIRE(result->getBooleanClauseValue() == pkb.wildValueBool);

    REQUIRE(pkb.valueValueCalls == 0);
    result = parent_star.evaluate(pkb);
    REQUIRE(pkb.valueValueCalls == 1);
    REQUIRE(pkb.last_rel_passed == RelationType::PARENT_STAR);
    REQUIRE(result->isBooleanResult());
    REQUIRE(result->getBooleanClauseValue() == pkb.valueValueBool);
  }

  SECTION("ModifiesS and ModifiesP Clauses") {
    // Also tests synonymWild and synonymValue

    // Expected behaviour
    ModifiesSClause modifies_s = ModifiesSClause(std::make_unique<StmtRef>(s),
                                       std::make_unique<EntRef>());
    ModifiesPClause modifies_p = ModifiesPClause(std::make_unique<EntRef>(proc),
                                            std::make_unique<EntRef>("var"));
    REQUIRE(pkb.synonymWildCalls == 0);
    result = modifies_s.evaluate(pkb);
    REQUIRE(pkb.synonymWildCalls == 1);
    REQUIRE(pkb.last_rel_passed == RelationType::MODIFIES_S);
    REQUIRE(result->getNumDeclarations() == 1);
    REQUIRE(result->contains(s));
    REQUIRE_FALSE(result->contains(proc));
    REQUIRE(result->getDeclarations() == std::vector<PqlDeclaration>{s});
    auto values = result->getValues(s);
    REQUIRE(*values == pkb.synonymWildValues);

    REQUIRE(pkb.synonymValueCalls == 0);
    result = modifies_p.evaluate(pkb);
    REQUIRE(pkb.synonymValueCalls == 1);
    REQUIRE(pkb.last_rel_passed == RelationType::MODIFIES_P);
    REQUIRE(result->getNumDeclarations() == 1);
    REQUIRE(result->contains(proc));
    REQUIRE_FALSE(result->contains(s));
    REQUIRE(result->getDeclarations() == std::vector<PqlDeclaration>{proc});
    values = result->getValues(proc);
    REQUIRE(*values == pkb.synonymValueValues);

    // catch errors as well
    // first arg cannot be wild.
    REQUIRE_THROWS_AS(ModifiesSClause(std::make_unique<StmtRef>(),
                                  std::make_unique<EntRef>(v)), InvalidSemanticsException);
    REQUIRE_THROWS_AS(ModifiesPClause(std::make_unique<EntRef>(),
                                      std::make_unique<EntRef>("abc")), InvalidSemanticsException);
    // if first arg is a declaration, must conform to expected types
    REQUIRE_THROWS_AS(ModifiesSClause(std::make_unique<StmtRef>(proc),
                                      std::make_unique<EntRef>(v)), InvalidSemanticsException);
    REQUIRE_THROWS_AS(ModifiesPClause(std::make_unique<EntRef>(s),
                                      std::make_unique<EntRef>("abc")), InvalidSemanticsException);
  }
}
