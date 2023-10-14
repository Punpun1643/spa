#include "../../stub/PkbQpsInterfaceStub.h"
#include "catch.hpp"
#include "query_processing_system/common/AffectsClause.h"
#include "query_processing_system/common/CallsClause.h"
#include "query_processing_system/common/Clause.h"
#include "query_processing_system/common/FollowsClause.h"
#include "query_processing_system/common/ModifiesPClause.h"
#include "query_processing_system/common/ModifiesSClause.h"
#include "query_processing_system/common/NextClause.h"
#include "query_processing_system/common/ParentClause.h"
#include "query_processing_system/common/UsesPClause.h"
#include "query_processing_system/common/UsesSClause.h"
#include "query_processing_system/exceptions/InvalidSemanticsException.h"


TEST_CASE("Test SuchThat Clauses") {
  PkbQpsInterfaceStub pkb = PkbQpsInterfaceStub();
  std::unique_ptr<ClauseResult> result;

  PqlDeclaration s = PqlDeclaration("stmt", EntityType::STMT);
  PqlDeclaration v = PqlDeclaration("var", EntityType::VARIABLE);
  PqlDeclaration re = PqlDeclaration("read", EntityType::READ);
  PqlDeclaration w = PqlDeclaration("while", EntityType::WHILE);
  PqlDeclaration a = PqlDeclaration("assign", EntityType::ASSIGN);
  PqlDeclaration print = PqlDeclaration("print", EntityType::PRINT);
  PqlDeclaration proc = PqlDeclaration("procedure1", EntityType::PROCEDURE);
  PqlDeclaration proc2 = PqlDeclaration("procedure2", EntityType::PROCEDURE);
  PqlDeclaration constant = PqlDeclaration("constant", EntityType::CONSTANT);

  SECTION("Follows/* Clauses") {
    // Also tests wildWild and valueWild
    FollowsClause follows = FollowsClause(std::make_unique<StmtRef>(),
                                          std::make_unique<StmtRef>(), false);
    FollowsClause follows_star = FollowsClause(
        std::make_unique<StmtRef>(4), std::make_unique<StmtRef>(), true);
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
    REQUIRE(pkb.last_value_passed == "4");
    REQUIRE(result->isBooleanResult());
    REQUIRE(result->getBooleanClauseValue() == pkb.valueWildBool);

    REQUIRE_NOTHROW(FollowsClause(std::make_unique<StmtRef>(s),
                                  std::make_unique<StmtRef>(print), false));
  }

  SECTION("Parent/* Clauses") {
    // Also tests wildValue and valueValue
    ParentClause parent = ParentClause(std::make_unique<StmtRef>(),
                                       std::make_unique<StmtRef>(1), false);
    ParentClause parent_star = ParentClause(std::make_unique<StmtRef>(2),
                                            std::make_unique<StmtRef>(3), true);
    REQUIRE(pkb.wildValueCalls == 0);
    result = parent.evaluate(pkb);
    REQUIRE(pkb.wildValueCalls == 1);
    REQUIRE(pkb.last_rel_passed == RelationType::PARENT);
    REQUIRE(pkb.last_value_passed == "1");
    REQUIRE(result->isBooleanResult());
    REQUIRE(result->getBooleanClauseValue() == pkb.wildValueBool);

    REQUIRE(pkb.valueValueCalls == 0);
    result = parent_star.evaluate(pkb);
    REQUIRE(pkb.valueValueCalls == 1);
    REQUIRE(pkb.last_rel_passed == RelationType::PARENT_STAR);
    REQUIRE(pkb.last_value_passed == "2");
    REQUIRE(pkb.last_value_2_passed == "3");
    REQUIRE(result->isBooleanResult());
    REQUIRE(result->getBooleanClauseValue() == pkb.valueValueBool);

    REQUIRE_NOTHROW(ParentClause(std::make_unique<StmtRef>(re),
                                 std::make_unique<StmtRef>(print), false));
  }

  SECTION("Calls/* Clauses") {
    // Also tests synonymSynonym and wildSynonym
    CallsClause calls = CallsClause(std::make_unique<EntRef>(proc),
                                    std::make_unique<EntRef>(proc2), false);
    CallsClause calls_star = CallsClause(std::make_unique<EntRef>(),
                                         std::make_unique<EntRef>(proc), true);
    REQUIRE(pkb.synonymSynonymCalls == 0);
    result = calls.evaluate(pkb);
    REQUIRE(pkb.synonymSynonymCalls == 1);
    REQUIRE(pkb.last_rel_passed == RelationType::CALLS);
    REQUIRE(pkb.last_entity_type_passed == EntityType::PROCEDURE);
    REQUIRE(pkb.last_entity_type_2_passed == EntityType::PROCEDURE);
    REQUIRE(result->getNumDeclarations() == 2);
    REQUIRE_THAT(
        result->getDeclarations(),
        Catch::UnorderedEquals(std::vector<PqlDeclaration>{proc, proc2}));
    auto values = result->getValues(proc);
    REQUIRE(*values == pkb.synonymSynonymValues1);
    values = result->getValues(proc2);
    REQUIRE(*values == pkb.synonymSynonymValues2);

    pkb.last_entity_type_passed = EntityType::IF;  // reset
    REQUIRE(pkb.wildSynonymCalls == 0);
    result = calls_star.evaluate(pkb);
    REQUIRE(pkb.wildSynonymCalls == 1);
    REQUIRE(pkb.last_rel_passed == RelationType::CALLS_STAR);
    REQUIRE(pkb.last_entity_type_passed == EntityType::PROCEDURE);
    REQUIRE(result->getNumDeclarations() == 1);
    REQUIRE(result->contains(proc));
    REQUIRE_FALSE(result->contains(proc2));
    values = result->getValues(proc);
    REQUIRE(*values == pkb.wildSynonymValues);

    // Test type checking for Calls/*
    REQUIRE_THROWS_AS(CallsClause(std::make_unique<EntRef>(constant),
                                  std::make_unique<EntRef>(proc), true),
                      InvalidSemanticsException);
    REQUIRE_THROWS_AS(CallsClause(std::make_unique<EntRef>(v),
                                  std::make_unique<EntRef>(proc2), false),
                      InvalidSemanticsException);
    REQUIRE_THROWS_AS(CallsClause(std::make_unique<EntRef>(proc),
                                  std::make_unique<EntRef>(constant), true),
                      InvalidSemanticsException);

    REQUIRE_THROWS_AS(CallsClause(std::make_unique<EntRef>(proc2),
                                  std::make_unique<EntRef>(v), false),
                      InvalidSemanticsException);
  }

  SECTION("ModifiesS and ModifiesP Clauses") {
    // Also tests synonymWild and synonymValue

    // Expected behaviour
    ModifiesSClause modifies_s = ModifiesSClause(
        std::make_unique<StmtRef>(print), std::make_unique<EntRef>());
    ModifiesPClause modifies_p = ModifiesPClause(
        std::make_unique<EntRef>(proc), std::make_unique<EntRef>("var"));
    REQUIRE(pkb.synonymWildCalls == 0);
    result = modifies_s.evaluate(pkb);
    REQUIRE(pkb.synonymWildCalls == 1);
    REQUIRE(pkb.last_rel_passed == RelationType::MODIFIES_S);
    REQUIRE(pkb.last_entity_type_passed == EntityType::PRINT);
    REQUIRE(result->getNumDeclarations() == 1);
    REQUIRE(result->contains(print));
    REQUIRE_FALSE(result->contains(proc));
    REQUIRE(result->getDeclarations() == std::vector<PqlDeclaration>{print});
    auto values = result->getValues(print);
    REQUIRE(*values == pkb.synonymWildValues);

    REQUIRE(pkb.synonymValueCalls == 0);
    result = modifies_p.evaluate(pkb);
    REQUIRE(pkb.synonymValueCalls == 1);
    REQUIRE(pkb.last_rel_passed == RelationType::MODIFIES_P);
    REQUIRE(pkb.last_entity_type_passed == EntityType::PROCEDURE);
    REQUIRE(pkb.last_value_passed == "var");
    REQUIRE(result->getNumDeclarations() == 1);
    REQUIRE(result->contains(proc));
    REQUIRE_FALSE(result->contains(s));
    REQUIRE(result->getDeclarations() == std::vector<PqlDeclaration>{proc});
    values = result->getValues(proc);
    REQUIRE(*values == pkb.synonymValueValues);

    // first arg cannot be wild.
    REQUIRE_THROWS_AS(ModifiesSClause(std::make_unique<StmtRef>(),
                                      std::make_unique<EntRef>(v)),
                      InvalidSemanticsException);
    REQUIRE_THROWS_AS(ModifiesPClause(std::make_unique<EntRef>(),
                                      std::make_unique<EntRef>("abc")),
                      InvalidSemanticsException);
    // if first arg is a declaration, must conform to expected types
    REQUIRE_THROWS_AS(ModifiesPClause(std::make_unique<EntRef>(s),
                                      std::make_unique<EntRef>("abc")),
                      InvalidSemanticsException);
    REQUIRE_THROWS_AS(ModifiesPClause(std::make_unique<EntRef>(print),
                                      std::make_unique<EntRef>("abc")),
                      InvalidSemanticsException);
    // Second arg must be a variable synonym
    REQUIRE_THROWS_AS(ModifiesSClause(std::make_unique<StmtRef>(s),
                                      std::make_unique<EntRef>(proc)),
                      InvalidSemanticsException);
    REQUIRE_THROWS_AS(ModifiesPClause(std::make_unique<EntRef>(proc),
                                      std::make_unique<EntRef>(constant)),
                      InvalidSemanticsException);
    REQUIRE_NOTHROW(ModifiesSClause(std::make_unique<StmtRef>(s),
                                    std::make_unique<EntRef>(v)));
    REQUIRE_NOTHROW(ModifiesPClause(std::make_unique<EntRef>(proc),
                                    std::make_unique<EntRef>(v)));
  }

  SECTION("UsesS and UsesP Clauses") {
    // Tests valueSynonym, valueWild
    UsesSClause uses_s =
        UsesSClause(std::make_unique<StmtRef>(2), std::make_unique<EntRef>(v));
    UsesPClause uses_p = UsesPClause(std::make_unique<EntRef>("name"),
                                     std::make_unique<EntRef>());
    REQUIRE(pkb.valueSynonymCalls == 0);
    result = uses_s.evaluate(pkb);
    REQUIRE(pkb.valueSynonymCalls == 1);
    REQUIRE(pkb.last_rel_passed == RelationType::USES_S);
    REQUIRE(pkb.last_value_passed == "2");
    REQUIRE(pkb.last_entity_type_passed == EntityType::VARIABLE);
    REQUIRE(result->getNumDeclarations() == 1);
    REQUIRE(result->contains(v));
    REQUIRE(result->getDeclarations() == std::vector<PqlDeclaration>{v});
    auto values = result->getValues(v);
    REQUIRE(*values == pkb.valueSynonymValues);

    REQUIRE(pkb.valueWildCalls == 0);
    result = uses_p.evaluate(pkb);
    REQUIRE(pkb.valueWildCalls == 1);
    REQUIRE(pkb.last_rel_passed == RelationType::USES_P);
    REQUIRE(result->getNumDeclarations() == 0);
    REQUIRE(result->getBooleanClauseValue() == pkb.valueWildBool);
  }

  SECTION("Modifies_S/Uses_S error handling") {
    // first arg cannot be wild.
    REQUIRE_THROWS_AS(
        UsesSClause(std::make_unique<StmtRef>(), std::make_unique<EntRef>(v)),
        InvalidSemanticsException);
    REQUIRE_THROWS_AS(ModifiesSClause(std::make_unique<StmtRef>(),
                                      std::make_unique<EntRef>()),
                      InvalidSemanticsException);
    // Second arg must be a variable synonym
    REQUIRE_THROWS_AS(UsesSClause(std::make_unique<StmtRef>(s),
                                  std::make_unique<EntRef>(proc)),
                      InvalidSemanticsException);
    REQUIRE_NOTHROW(
        UsesSClause(std::make_unique<StmtRef>(s), std::make_unique<EntRef>(v)));
    REQUIRE_THROWS_AS(ModifiesSClause(std::make_unique<StmtRef>(s),
                                      std::make_unique<EntRef>(constant)),
                      InvalidSemanticsException);
    REQUIRE_NOTHROW(ModifiesSClause(std::make_unique<StmtRef>(s),
                                    std::make_unique<EntRef>(v)));
  }

  SECTION("Modifies_P/Uses_P error handling") {
    // first arg cannot be wild
    REQUIRE_THROWS_AS(ModifiesPClause(std::make_unique<EntRef>(),
                                      std::make_unique<EntRef>(v)),
                      InvalidSemanticsException);
    REQUIRE_THROWS_AS(UsesPClause(std::make_unique<EntRef>(),
                                  std::make_unique<EntRef>("abc")),
                      InvalidSemanticsException);
    // if first arg is a declaration, must conform to expected types
    REQUIRE_THROWS_AS(UsesPClause(std::make_unique<EntRef>(s),
                                  std::make_unique<EntRef>("abc")),
                      InvalidSemanticsException);
    REQUIRE_THROWS_AS(UsesPClause(std::make_unique<EntRef>(print),
                                  std::make_unique<EntRef>("abc")),
                      InvalidSemanticsException);
    REQUIRE_THROWS_AS(ModifiesPClause(std::make_unique<EntRef>(s),
                                      std::make_unique<EntRef>("abc")),
                      InvalidSemanticsException);
    REQUIRE_THROWS_AS(ModifiesPClause(std::make_unique<EntRef>(print),
                                      std::make_unique<EntRef>("abc")),
                      InvalidSemanticsException);
    // Second arg must be a variable synonym
    REQUIRE_THROWS_AS(UsesPClause(std::make_unique<EntRef>(proc),
                                  std::make_unique<EntRef>(constant)),
                      InvalidSemanticsException);
    REQUIRE_NOTHROW(UsesPClause(std::make_unique<EntRef>(proc),
                                std::make_unique<EntRef>(v)));
    REQUIRE_THROWS_AS(ModifiesPClause(std::make_unique<EntRef>("abc"),
                                      std::make_unique<EntRef>(proc2)),
                      InvalidSemanticsException);
    REQUIRE_NOTHROW(ModifiesPClause(std::make_unique<EntRef>(proc),
                                    std::make_unique<EntRef>(v)));
  }

  SECTION("Next/* Clauses") {
    NextClause next = NextClause(std::make_unique<StmtRef>(w),
                                 std::make_unique<StmtRef>(s), false);
    NextClause next_star = NextClause(std::make_unique<StmtRef>(),
                                      std::make_unique<StmtRef>(12), true);
    result = next.evaluate(pkb);
    REQUIRE(pkb.synonymSynonymCalls == 1);
    REQUIRE(pkb.last_rel_passed == RelationType::NEXT);
    REQUIRE(pkb.last_entity_type_passed == EntityType::WHILE);
    REQUIRE(pkb.last_entity_type_2_passed == EntityType::STMT);
    REQUIRE(result->getNumDeclarations() == 2);
    REQUIRE_THAT(result->getDeclarations(),
                 Catch::UnorderedEquals(std::vector<PqlDeclaration>{w, s}));

    result = next_star.evaluate(pkb);
    REQUIRE(pkb.wildValueCalls == 1);
    REQUIRE(pkb.last_rel_passed == RelationType::NEXT_STAR);
    REQUIRE(pkb.last_value_passed == "12");
    REQUIRE(result->isBooleanResult());

    // NextClause accepts all types that StmtRef would accept.
  }

  SECTION("Affects Clauses") {
    AffectsClause affects = AffectsClause(std::make_unique<StmtRef>(a),
                                          std::make_unique<StmtRef>(22));
    result = affects.evaluate(pkb);
    REQUIRE(pkb.synonymValueCalls == 1);
    REQUIRE(pkb.last_rel_passed == RelationType::AFFECTS);
    REQUIRE(pkb.last_entity_type_passed == EntityType::ASSIGN);
    REQUIRE(pkb.last_value_passed == "22");
    REQUIRE(result->getNumDeclarations() == 1);
    REQUIRE_THAT(result->getDeclarations(),
                 Catch::UnorderedEquals(std::vector<PqlDeclaration>{a}));
    // AffectsClause accepts all types that StmtRef would accept. But only
    // returns values other than 'None' for Assign statements.
  }
}
