#include "QPSController.h"

#include <iostream>
#include <sstream>

#include "../shared/tokenizer/Tokenizer.h"
#include "common/SelectClause.h"
#include "common/SuchThatClause.h"
#include "parser/ContextBuilder.h"

QPSController::QPSController(){};

void QPSController::HandleQuery(
    std::string& query, std::list<std::string>& results,
    std::shared_ptr<QueryEvaluator> query_evaluator) {
  std::vector<std::shared_ptr<Token>> tokens = this->TokenizeQuery(query);
  this->CheckSyntax(tokens);

  std::shared_ptr<Context> context = this->FormContext(tokens);

  std::shared_ptr<AExpression> expression_tree =
      this->FormExpressionTree(tokens);

  this->InterpretContext(context, expression_tree);
  std::vector<PqlDeclaration> selected_declarations =
      context->GetSelectedDeclarations();
  /* std::unique_ptr<SelectClause> select_clause = context.GetSelectClause(); */
  std::vector<std::shared_ptr<Clause>> other_clauses =
      context->GetOtherClauses();
  std::vector<std::vector<std::string>> query_results =
      query_evaluator->evaluateQuery(selected_declarations, other_clauses);
  for (std::vector<std::string> result : query_results) {
    std::string result_string;
    for (std::vector<std::string>::iterator it = result.begin();
         it != result.end(); it++) {
      result_string += *it;
      if (it != result.end()) {
        result_string += ", ";
      }
    }
    results.push_back(result_string);
  }
}

/* std::unique_ptr<QueryExpression> query_expression = */
/*     this->FormQueryExpression(tokens); */
/* std::vector<std::shared_ptr<Clause>> clause_list = */
/*     this->InterpretQueryExpression(std::move(query_expression)); */
/* std::vector<std::string> query_results; */

/* auto select_clause_shared_ptr =
 * std::dynamic_pointer_cast<SelectClause>(clause_list[0]); */
/* auto select_clause =
 * std::make_unique<SelectClause>(*select_clause_shared_ptr); */

/* clause_list.erase(clause_list.begin()); */
/* query_results = query_evaluator->evaluateQuery(std::move(select_clause),
 * clause_list); */
/* std::copy(query_results.begin(), query_results.end(), */
/*           std::back_inserter(results)); */
/* } */

void QPSController::CheckSyntax(std::vector<std::shared_ptr<Token>> tokens) {
  std::unique_ptr<SyntaxChecker> syntax_checker =
      std::make_unique<SyntaxChecker>(tokens);
  syntax_checker->parse();
}

std::shared_ptr<Context> QPSController::FormContext(
    std::vector<std::shared_ptr<Token>> tokens) {
  ContextBuilder context_builder(tokens);
  context_builder.parse();
  return context_builder.GetContext();
}

std::shared_ptr<AExpression> QPSController::FormExpressionTree(
    std::vector<std::shared_ptr<Token>> tokens) {
  std::unique_ptr<ExpressionTreeBuilder> expression_tree_builder =
      std::make_unique<ExpressionTreeBuilder>(tokens);
  expression_tree_builder->parse();
  return expression_tree_builder->GetExpressionTree();
}

void QPSController::InterpretContext(
    std::shared_ptr<Context> context,
    std::shared_ptr<AExpression> expression_tree) {
  std::unique_ptr<QueryInterpreter> query_interpreter =
      std::make_unique<QueryInterpreter>(context, expression_tree);
  query_interpreter->Interpret();
}

std::vector<std::shared_ptr<Token>> QPSController::TokenizeQuery(
    std::string& query) {
  std::istringstream string_stream(query);
  Tokenizer tokenizer = Tokenizer(string_stream);
  return tokenizer.tokenize();
}

// ------------ refactoring ends here -----------------

/* std::unique_ptr<QueryExpression> QPSController::FormQueryExpression( */
/*     std::vector<std::shared_ptr<Token>> tokens) { */
/*   std::unique_ptr<ExpressionTreeBuilder> expression_tree_builder = */
/*       std::make_unique<ExpressionTreeBuilder>(tokens); */
/*   expression_tree_builder->parse(); */
/*   return expression_tree_builder->GetQueryExpression(); */
/* } */

/* std::vector<std::shared_ptr<Clause>> QPSController::InterpretQueryExpression(
 */
/*     std::unique_ptr<QueryExpression> query_expression) { */
/*   std::unique_ptr<QueryInterpreter> query_interpreter = */
/*       std::make_unique<QueryInterpreter>(); */
/*   query_interpreter->Interpret(*(std::move(query_expression))); */
/*   return query_interpreter->GetClauseList(); */
/* } */

/* std::vector<std::shared_ptr<Clause>> QPSController::ParseAndGetClauses( */
/*     std::vector<std::shared_ptr<Token>>& tokens) { */
/*   this->CheckSyntax(tokens); */
/*   std::unique_ptr<QueryExpression> query_expression = */
/*       this->FormQueryExpression(tokens); */
/*   std::vector<std::shared_ptr<Clause>> clause_list = */
/*       this->InterpretQueryExpression(std::move(query_expression)); */
/*   return clause_list; */
/* } */
