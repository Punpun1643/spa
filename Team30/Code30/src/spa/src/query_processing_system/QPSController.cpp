#include "QPSController.h"

#include <iostream>
#include <sstream>

#include "../shared/tokenizer/ATokenizer.h"
#include "common/SelectClause.h"
#include "common/SuchThatClause.h"

QPSController::QPSController(){};

void QPSController::HandleQuery(
    std::string& query, std::list<std::string>& results,
    std::shared_ptr<QueryEvaluator> query_evaluator) {
  std::vector<std::shared_ptr<Token>> tokens = this->TokenizeQuery(query);
  this->CheckSyntax(tokens);
  std::unique_ptr<QueryExpression> query_expression =
      this->FormQueryExpression(tokens);
  std::vector<std::unique_ptr<Clause>> clause_list =
      this->InterpretQueryExpression(std::move(query_expression));
  // TODO: After milestone 1, need to change way of checking clause contents.
  // cant assume only 1 clause for each type
  std::unique_ptr<std::vector<std::string>> query_results;
  std::unique_ptr<SelectClause> select_clause(
      dynamic_cast<SelectClause*>(clause_list[0].release()));
  if (clause_list.size() == 1) {
    query_results = query_evaluator->evaluateQuery(std::move(select_clause));
  } else if (clause_list.size() == 2) {
    std::unique_ptr<SuchThatClause> such_that_clause(
        dynamic_cast<SuchThatClause*>(clause_list[1].release()));
    query_results = query_evaluator->evaluateQuery(std::move(select_clause),
                                                   std::move(such_that_clause));
  }
  std::copy(query_results->begin(), query_results->end(),
            std::back_inserter(results));
}

std::vector<std::shared_ptr<Token>> QPSController::TokenizeQuery(
    std::string& query) {
  std::istringstream string_stream(query);
  ATokenizer tokenizer = ATokenizer(string_stream);
  return tokenizer.tokenize();
}

void QPSController::CheckSyntax(std::vector<std::shared_ptr<Token>> tokens) {
  std::unique_ptr<SyntaxChecker> syntax_checker =
      std::make_unique<SyntaxChecker>(tokens);
  syntax_checker->parse();
}

std::unique_ptr<QueryExpression> QPSController::FormQueryExpression(
    std::vector<std::shared_ptr<Token>> tokens) {
  std::unique_ptr<ExpressionTreeBuilder> expression_tree_builder =
      std::make_unique<ExpressionTreeBuilder>(tokens);
  expression_tree_builder->parse();
  return expression_tree_builder->GetQueryExpression();
}

std::vector<std::unique_ptr<Clause>> QPSController::InterpretQueryExpression(
    std::unique_ptr<QueryExpression> query_expression) {
  std::unique_ptr<QueryInterpreter> query_interpreter =
      std::make_unique<QueryInterpreter>();
  query_interpreter->Interpret(*(std::move(query_expression)));
  return query_interpreter->GetClauseList();
}

std::vector<std::unique_ptr<Clause>> QPSController::ParseAndGetClauses(
    std::vector<std::shared_ptr<Token>>& tokens) {
  this->CheckSyntax(tokens);
  std::unique_ptr<QueryExpression> query_expression =
      this->FormQueryExpression(tokens);
  std::vector<std::unique_ptr<Clause>> clause_list =
      this->InterpretQueryExpression(std::move(query_expression));
  return clause_list;
}
