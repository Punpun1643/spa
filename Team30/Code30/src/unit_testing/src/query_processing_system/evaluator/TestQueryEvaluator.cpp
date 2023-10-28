#include <memory>

#include "../../../../spa/src/program_knowledge_base/PKBQPSInterface.h"
#include "../../../../spa/src/query_processing_system/clauses/FollowsClause.h"
#include "../../../../spa/src/query_processing_system/clauses/WithClause.h"
#include "../../../../spa/src/query_processing_system/clauses/PatternAssignClause.h"
#include "../../../../spa/src/query_processing_system/evaluator/QueryEvaluator.h"
#include "query_processing_system/clauses/NotClauseDecorator.h"
#include "../../stub/PkbQpsInterfaceStub.h"
#include "catch.hpp"
#include "shared/ArrayUtility.h"

class QeFactoryMethods {
 public:
  static std::shared_ptr<FollowsClause> getFollowsClause(StmtRef ref1,
                                                         StmtRef ref2) {
    return std::make_shared<FollowsClause>(std::make_unique<StmtRef>(ref1),
                                           std::make_unique<StmtRef>(ref2),
                                           false);
  }
};

TEST_CASE("Test Query Evaluator") {
  PkbQpsInterfaceStub pkb = PkbQpsInterfaceStub();
  QueryEvaluator qe = QueryEvaluator(pkb);
  PqlDeclaration a = PqlDeclaration("a", EntityType::ASSIGN);
  AttrRef a_attr_ref = AttrRef(a);
  PqlDeclaration if_decl = PqlDeclaration("if", EntityType::IF);
  PqlDeclaration s = PqlDeclaration("s", EntityType::STMT);
  PqlDeclaration v = PqlDeclaration("v", EntityType::VARIABLE);
  PqlDeclaration print = PqlDeclaration("print", EntityType::PRINT);
  PqlDeclaration call = PqlDeclaration("call", EntityType::CALL);

  std::shared_ptr<Clause> follows_clause;
  std::shared_ptr<TreeNode> rhs_expr =
      std::make_shared<TreeNode>("", nullptr, nullptr);
  std::shared_ptr<Clause> pattern_clause;
  std::shared_ptr<Clause> with_clause;
  std::vector<std::vector<std::string>> result;

  auto negation_follows_clause =
      QeFactoryMethods::getFollowsClause(StmtRef(2), StmtRef());
  std::shared_ptr<Clause> negation_with_clause =
      std::make_shared<WithClause>(2, 20);

  SECTION("Evaluate boolean query") {
    // bool query with no clauses is true
    REQUIRE(qe.EvaluateQuery({}));

    // bool query with a result
    follows_clause = QeFactoryMethods::getFollowsClause(StmtRef(a), StmtRef(1));
    REQUIRE(qe.EvaluateQuery({follows_clause}));

    // bool query with no result
    pattern_clause = std::make_shared<PatternAssignClause>(
        a, EntRef(), MatchType::EXACT_MATCH, rhs_expr);
    REQUIRE_FALSE(qe.EvaluateQuery({follows_clause, pattern_clause}));
  }

  SECTION("Evaluate query with one decl that is not in the clauses") {
    result = qe.EvaluateQuery({a_attr_ref}, {});
    REQUIRE(ArrayUtility::FlattenVector(result) == pkb.get_all_of_type_values);

    // negated by clauses
    follows_clause = QeFactoryMethods::getFollowsClause(StmtRef(2), StmtRef());
    result = qe.EvaluateQuery({a_attr_ref}, {follows_clause});
    REQUIRE(result.empty());

    // clause has values
    follows_clause = QeFactoryMethods::getFollowsClause(StmtRef(), StmtRef());
    result = qe.EvaluateQuery({a_attr_ref}, {follows_clause});
    REQUIRE(ArrayUtility::FlattenVector(result) == pkb.get_all_of_type_values);
  }

  SECTION("Evaluate query with one decl, decl also in clauses") {
    // output values should follow existing clauses
    follows_clause = QeFactoryMethods::getFollowsClause(StmtRef(a), StmtRef(1));
    result = qe.EvaluateQuery({a_attr_ref}, {follows_clause});
    REQUIRE(ArrayUtility::FlattenVector(result) == pkb.synonym_value_values);
    // Addition of empty clause -> no results
    result = qe.EvaluateQuery({a_attr_ref},
                              {follows_clause, negation_follows_clause});
    REQUIRE(result.empty());
  }

  SECTION("Evaluate query with multiple decls") {
    follows_clause =
        QeFactoryMethods::getFollowsClause(StmtRef(if_decl), StmtRef());
    pattern_clause = std::make_shared<PatternAssignClause>(
        a, EntRef(v), MatchType::EXACT_MATCH, rhs_expr);
    result = qe.EvaluateQuery({AttrRef(s), AttrRef(s), a_attr_ref, AttrRef(v)},
                              {follows_clause, pattern_clause});
    std::vector<std::vector<std::string>> expected_result = {
        {"x", "x", "123", "345"},
        {"y", "y", "123", "345"},
        {"z", "z", "123", "345"}};
    REQUIRE_THAT(result, Catch::UnorderedEquals(expected_result));

    // Addition of empty clause -> no results

    result = qe.EvaluateQuery(
        {AttrRef(s), AttrRef(s), a_attr_ref, AttrRef(v)},
        {follows_clause, pattern_clause, negation_follows_clause});
    REQUIRE(result.empty());
  }

  SECTION("Evaluate query with AttrRefs") {
    follows_clause =
        QeFactoryMethods::getFollowsClause(StmtRef(if_decl), StmtRef());
    AttrRef attr_ref = AttrRef(print, AttrType::VAR_NAME);

    result = qe.EvaluateQuery({attr_ref, AttrRef(if_decl), attr_ref},
                              {follows_clause});
    std::vector<std::vector<std::string>> expected_result = {{"a", "1", "a"},
                                                             {"a", "3", "a"}};
    REQUIRE_THAT(result, Catch::UnorderedEquals(expected_result));

    // No results case
    result = qe.EvaluateQuery({attr_ref, AttrRef(if_decl), attr_ref},
                              {follows_clause, negation_with_clause});
    REQUIRE(result.empty());
  }

  SECTION("Evaluate query with a WithClause") {
    with_clause = std::make_shared<WithClause>(3, AttrRef(print));
    result = qe.EvaluateQuery(
        {AttrRef(print), AttrRef(print, AttrType::VAR_NAME)}, {with_clause});
    std::vector<std::vector<std::string>> expected_result = {
        {"alpha", "a"}, {"beta", "a"}, {"delta", "a"}};
    REQUIRE_THAT(result, Catch::UnorderedEquals(expected_result));
  }

  SECTION("Evaluate query with NotClause") {
   follows_clause = QeFactoryMethods::getFollowsClause(StmtRef(s), StmtRef());
   std::shared_ptr<Clause> not_clause = std::make_shared<NotClauseDecorator>(follows_clause);
   pkb.get_all_of_type_values = {"a", "b", "c", "aa"};
   pkb.synonym_wild_values = {"b", "c"};
   result = qe.EvaluateQuery({AttrRef(s)}, {not_clause});
   std::vector<std::vector<std::string>> expected_result = {{"a"}, {"aa"}};
   REQUIRE_THAT(result, Catch::UnorderedEquals(expected_result));
  }
}
