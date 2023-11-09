#include "../../stub/PkbQpsInterfaceStub.h"
#include "catch.hpp"
#include "query_processing_system/clauses/AffectsClause.h"
#include "query_processing_system/clauses/CallsClause.h"
#include "query_processing_system/clauses/Clause.h"
#include "query_processing_system/clauses/FollowsClause.h"
#include "query_processing_system/clauses/ModifiesPClause.h"
#include "query_processing_system/clauses/ModifiesSClause.h"
#include "query_processing_system/clauses/NextClause.h"
#include "query_processing_system/clauses/ParentClause.h"
#include "query_processing_system/clauses/UsesPClause.h"
#include "query_processing_system/clauses/UsesSClause.h"
#include "query_processing_system/exceptions/InvalidSemanticsException.h"
#include "shared/ArrayUtility.h"

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
    REQUIRE(pkb.wild_wild_calls == 0);
    result = follows.Evaluate(pkb);
    REQUIRE(pkb.wild_wild_calls == 1);
    REQUIRE(pkb.last_rel_passed == RelationType::FOLLOWS);
    REQUIRE(result->IsBooleanResult());
    REQUIRE(result->GetBooleanClauseValue() == pkb.wild_wild_bool);

    REQUIRE(pkb.value_wild_calls == 0);
    result = follows_star.Evaluate(pkb);
    REQUIRE(pkb.value_wild_calls == 1);
    REQUIRE(pkb.last_rel_passed == RelationType::FOLLOWS_STAR);
    REQUIRE(pkb.last_value_passed == "4");
    REQUIRE(result->IsBooleanResult());
    REQUIRE(result->GetBooleanClauseValue() == pkb.value_wild_bool);

    REQUIRE_NOTHROW(FollowsClause(std::make_unique<StmtRef>(s),
                                  std::make_unique<StmtRef>(print), false));
  }

  SECTION("Parent/* Clauses") {
    // Also tests wildValue and valueValue
    ParentClause parent = ParentClause(std::make_unique<StmtRef>(),
                                       std::make_unique<StmtRef>(1), false);
    ParentClause parent_star = ParentClause(std::make_unique<StmtRef>(2),
                                            std::make_unique<StmtRef>(3), true);
    REQUIRE(pkb.wild_value_calls == 0);
    result = parent.Evaluate(pkb);
    REQUIRE(pkb.wild_value_calls == 1);
    REQUIRE(pkb.last_rel_passed == RelationType::PARENT);
    REQUIRE(pkb.last_value_passed == "1");
    REQUIRE(result->IsBooleanResult());
    REQUIRE(result->GetBooleanClauseValue() == pkb.wild_value_bool);

    REQUIRE(pkb.value_value_calls == 0);
    result = parent_star.Evaluate(pkb);
    REQUIRE(pkb.value_value_calls == 1);
    REQUIRE(pkb.last_rel_passed == RelationType::PARENT_STAR);
    REQUIRE(pkb.last_value_passed == "2");
    REQUIRE(pkb.last_value_2_passed == "3");
    REQUIRE(result->IsBooleanResult());
    REQUIRE(result->GetBooleanClauseValue() == pkb.value_value_bool);

    REQUIRE_NOTHROW(ParentClause(std::make_unique<StmtRef>(re),
                                 std::make_unique<StmtRef>(print), false));
  }

  SECTION("Calls/* Clauses") {
    // Also tests synonymSynonym and wildSynonym
    CallsClause calls = CallsClause(std::make_unique<EntRef>(proc),
                                    std::make_unique<EntRef>(proc2), false);
    CallsClause calls_star = CallsClause(std::make_unique<EntRef>(),
                                         std::make_unique<EntRef>(proc), true);
    REQUIRE(pkb.synonym_synonym_calls == 0);
    result = calls.Evaluate(pkb);
    REQUIRE(pkb.synonym_synonym_calls == 1);
    REQUIRE(pkb.last_rel_passed == RelationType::CALLS);
    REQUIRE(pkb.last_entity_type_passed == EntityType::PROCEDURE);
    REQUIRE(pkb.last_entity_type_2_passed == EntityType::PROCEDURE);
    REQUIRE(result->GetNumDeclarations() == 2);
    REQUIRE_THAT(
        result->GetDeclarations(),
        Catch::UnorderedEquals(std::vector<PqlDeclaration>{proc, proc2}));
    auto paired_results = ArrayUtility::SplitPairVector(result->GetPairedResultValues());
    REQUIRE(paired_results.first == pkb.synonym_synonym_values_1);
    REQUIRE(paired_results.second == pkb.synonym_synonym_values_2);

    pkb.last_entity_type_passed = EntityType::IF;  // reset
    REQUIRE(pkb.wild_synonym_calls == 0);
    result = calls_star.Evaluate(pkb);
    REQUIRE(pkb.wild_synonym_calls == 1);
    REQUIRE(pkb.last_rel_passed == RelationType::CALLS_STAR);
    REQUIRE(pkb.last_entity_type_passed == EntityType::PROCEDURE);
    REQUIRE(result->GetNumDeclarations() == 1);
    REQUIRE(result->Contains(proc));
    REQUIRE_FALSE(result->Contains(proc2));
    auto values = result->GetSingleResultValues();
    REQUIRE(values == pkb.wild_synonym_values);

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
    REQUIRE(pkb.synonym_wild_calls == 0);
    result = modifies_s.Evaluate(pkb);
    REQUIRE(pkb.synonym_wild_calls == 1);
    REQUIRE(pkb.last_rel_passed == RelationType::MODIFIES_S);
    REQUIRE(pkb.last_entity_type_passed == EntityType::PRINT);
    REQUIRE(result->GetNumDeclarations() == 1);
    REQUIRE(result->Contains(print));
    REQUIRE_FALSE(result->Contains(proc));
    REQUIRE(result->GetDeclarations() == std::vector<PqlDeclaration>{print});
    auto values = result->GetSingleResultValues();
    REQUIRE(values == pkb.synonym_wild_values);

    REQUIRE(pkb.synonym_value_calls == 0);
    result = modifies_p.Evaluate(pkb);
    REQUIRE(pkb.synonym_value_calls == 1);
    REQUIRE(pkb.last_rel_passed == RelationType::MODIFIES_P);
    REQUIRE(pkb.last_entity_type_passed == EntityType::PROCEDURE);
    REQUIRE(pkb.last_value_passed == "var");
    REQUIRE(result->GetNumDeclarations() == 1);
    REQUIRE(result->Contains(proc));
    REQUIRE_FALSE(result->Contains(s));
    REQUIRE(result->GetDeclarations() == std::vector<PqlDeclaration>{proc});
    values = result->GetSingleResultValues();
    REQUIRE(values == pkb.synonym_value_values);

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
    REQUIRE(pkb.value_synonym_calls == 0);
    result = uses_s.Evaluate(pkb);
    REQUIRE(pkb.value_synonym_calls == 1);
    REQUIRE(pkb.last_rel_passed == RelationType::USES_S);
    REQUIRE(pkb.last_value_passed == "2");
    REQUIRE(pkb.last_entity_type_passed == EntityType::VARIABLE);
    REQUIRE(result->GetNumDeclarations() == 1);
    REQUIRE(result->Contains(v));
    REQUIRE(result->GetDeclarations() == std::vector<PqlDeclaration>{v});
    auto values = result->GetSingleResultValues();
    REQUIRE(values == pkb.value_synonym_values);

    REQUIRE(pkb.value_wild_calls == 0);
    result = uses_p.Evaluate(pkb);
    REQUIRE(pkb.value_wild_calls == 1);
    REQUIRE(pkb.last_rel_passed == RelationType::USES_P);
    REQUIRE(result->GetNumDeclarations() == 0);
    REQUIRE(result->GetBooleanClauseValue() == pkb.value_wild_bool);
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
    result = next.Evaluate(pkb);
    REQUIRE(pkb.synonym_synonym_calls == 1);
    REQUIRE(pkb.last_rel_passed == RelationType::NEXT);
    REQUIRE(pkb.last_entity_type_passed == EntityType::WHILE);
    REQUIRE(pkb.last_entity_type_2_passed == EntityType::STMT);
    REQUIRE(result->GetNumDeclarations() == 2);
    REQUIRE_THAT(result->GetDeclarations(),
                 Catch::UnorderedEquals(std::vector<PqlDeclaration>{w, s}));

    result = next_star.Evaluate(pkb);
    REQUIRE(pkb.wild_value_calls == 1);
    REQUIRE(pkb.last_rel_passed == RelationType::NEXT_STAR);
    REQUIRE(pkb.last_value_passed == "12");
    REQUIRE(result->IsBooleanResult());

    // NextClause accepts all types that StmtRef would accept.
  }

  SECTION("Affects Clauses") {
    AffectsClause affects = AffectsClause(std::make_unique<StmtRef>(a),
                                          std::make_unique<StmtRef>(22));
    result = affects.Evaluate(pkb);
    REQUIRE(pkb.synonym_value_calls == 1);
    REQUIRE(pkb.last_rel_passed == RelationType::AFFECTS);
    REQUIRE(pkb.last_entity_type_passed == EntityType::ASSIGN);
    REQUIRE(pkb.last_value_passed == "22");
    REQUIRE(result->GetNumDeclarations() == 1);
    REQUIRE_THAT(result->GetDeclarations(),
                 Catch::UnorderedEquals(std::vector<PqlDeclaration>{a}));
    // AffectsClause accepts all types that StmtRef would accept. But only
    // returns values other than 'None' for Assign statements.
  }

  SECTION("Test clause with Same Synonym in Both Args") {
    NextClause next = NextClause(std::make_unique<StmtRef>(s),
                                 std::make_unique<StmtRef>(s), false);
    result = next.Evaluate(pkb);
    REQUIRE(result->GetNumDeclarations() == 1);
    REQUIRE(result->GetSingleResultValues().empty());

    pkb.synonym_synonym_values = {std::make_pair("42", "42")};
    NextClause next_2 = NextClause(std::make_unique<StmtRef>(a),
                                   std::make_unique<StmtRef>(a), false);
    result = next_2.Evaluate(pkb);
    REQUIRE(result->GetNumDeclarations() == 1);
    REQUIRE_FALSE(result->IsBooleanResult());
    REQUIRE(result->GetSingleResultValues() == std::vector<std::string>{"42"});
  }
}
