#include "QPSController.h"

#include <iostream>

QPSController::QPSController(std::vector<std::shared_ptr<Token>> tokens)
    : tokens(tokens){};

std::vector<std::shared_ptr<Clause>> QPSController::ParseAndGetClauses() {
  std::vector<std::shared_ptr<Token>> tokens = this->tokens;

  // Check syntax
  std::unique_ptr<SyntaxChecker> syntax_checker =
      std::make_unique<SyntaxChecker>(tokens);
  syntax_checker->parse();

  // Build Expression Tree
  std::unique_ptr<ExpressionTreeBuilder> expression_tree_builder =
      std::make_unique<ExpressionTreeBuilder>(tokens);
  expression_tree_builder->parse();
  std::unique_ptr<QueryExpression> query_expression =
      expression_tree_builder->GetQueryExpression();

  // Build and return Clause Vector
  std::unique_ptr<QueryInterpreter> query_interpreter =
      std::make_unique<QueryInterpreter>();
  query_interpreter->Interpret(*query_expression);

  return query_interpreter->GetClauseList();
}
