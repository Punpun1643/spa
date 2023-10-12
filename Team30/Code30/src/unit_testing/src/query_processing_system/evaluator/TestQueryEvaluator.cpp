#include <memory>

#include "../../../../spa/src/program_knowledge_base/PKBQPSInterface.h"
#include "../../../../spa/src/query_processing_system/common/FollowsClause.h"
#include "../../../../spa/src/query_processing_system/evaluator/ArrayUtility.h"
#include "../../../../spa/src/query_processing_system/evaluator/QueryEvaluator.h"
#include "../../../../spa/src/query_processing_system/exceptions/InvalidSemanticsException.h"
#include "../../stub/PkbQpsInterfaceStub.h"
#include "../../stub/PkbStub.h"
#include "catch.hpp"

class QeFactoryMethods {
 public:
  static std::shared_ptr<FollowsClause> getFollowsClause(StmtRef ref1,
                                                         StmtRef ref2) {
    return std::make_shared<FollowsClause>(std::make_unique<StmtRef>(ref1),
                                           std::make_unique<StmtRef>(ref2), false);
  }
};

TEST_CASE("Test Query Evaluator") {
  PkbQpsInterfaceStub pkb = PkbQpsInterfaceStub();
  QueryEvaluator qe = QueryEvaluator(pkb);
  PqlDeclaration a = PqlDeclaration("a", EntityType::ASSIGN);
  PqlDeclaration if_decl = PqlDeclaration("if", EntityType::IF);
  PqlDeclaration s = PqlDeclaration("s", EntityType::STMT);
  PqlDeclaration v = PqlDeclaration("v", EntityType::VARIABLE);

  std::shared_ptr<Clause> follows_clause;
  std::shared_ptr<Clause> pattern_clause;
  std::vector<std::vector<std::string>> result;

  auto negation_clause =
      QeFactoryMethods::getFollowsClause(StmtRef(2), StmtRef());

  SECTION("Evaluate boolean query") {
    // bool query with no clauses is true
    REQUIRE(qe.evaluateQuery({}));

    // bool query with a result
    follows_clause = QeFactoryMethods::getFollowsClause(StmtRef(a), StmtRef(1));
    REQUIRE(qe.evaluateQuery({follows_clause}));

    // bool query with no result
    pattern_clause = std::make_shared<PatternClause>(
        a, EntRef(), MatchType::EXACT_MATCH, "blah");
    REQUIRE_FALSE(qe.evaluateQuery({follows_clause, pattern_clause}));
  }

  SECTION("Evaluate query with one decl that is not in the clauses") {
    result = qe.evaluateQuery({a}, {});
    REQUIRE(ArrayUtility::flattenVector(result) == pkb.getAllOfTypeValues);

    // negated by clauses
    follows_clause = QeFactoryMethods::getFollowsClause(StmtRef(2), StmtRef());
    result = qe.evaluateQuery({a}, {follows_clause});
    REQUIRE(result.empty());

    // clause has values
    follows_clause = QeFactoryMethods::getFollowsClause(StmtRef(), StmtRef());
    result = qe.evaluateQuery({a}, {follows_clause});
    REQUIRE(ArrayUtility::flattenVector(result) == pkb.getAllOfTypeValues);
  }

  SECTION("Evaluate query with one decl, decl also in clauses") {
    // output values should follow existing clauses
    follows_clause = QeFactoryMethods::getFollowsClause(StmtRef(a), StmtRef(1));
    result = qe.evaluateQuery({a}, {follows_clause});
    REQUIRE(ArrayUtility::flattenVector(result) == pkb.synonymValueValues);
    // Addition of empty clause -> no results
    result = qe.evaluateQuery({a}, {follows_clause, negation_clause});
    REQUIRE(result.empty());
  }

  SECTION("Evaluate query with multiple decls") {
    follows_clause = QeFactoryMethods::getFollowsClause(StmtRef(if_decl), StmtRef());
    pattern_clause = std::make_shared<PatternClause>(
        a, EntRef(v), MatchType::EXACT_MATCH, "blah");
    result = qe.evaluateQuery({s, s, a, v}, {follows_clause, pattern_clause});
    std::vector<std::vector<std::string>> expected_result = {
        {"x","x","123","345"},{"y","y", "123","345"},{"z", "z","123","345"}
    };
    REQUIRE_THAT(result, Catch::UnorderedEquals(expected_result));

    // Addition of empty clause -> no results

    result = qe.evaluateQuery({s, s, a, v}, {follows_clause, pattern_clause, negation_clause});
    REQUIRE(result.empty());
  }
}
