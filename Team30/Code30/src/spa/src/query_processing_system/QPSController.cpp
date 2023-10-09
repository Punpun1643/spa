#include "QPSController.h"

#include <iostream>
#include <sstream>

#include "common/CallsClause.h"

#include "common/SuchThatClause.h"
#include "common/UsesSClause.h"
#include "exceptions/InvalidSyntaxException.h"
#include "parser/ContextBuilder.h"
#include "../shared/tokenizer/Tokenizer.h"

QPSController::QPSController(){};

void QPSController::HandleQuery(
    std::string& query, std::list<std::string>& results,
    std::shared_ptr<QueryEvaluator> query_evaluator) {
  std::vector<std::shared_ptr<Token>> tokens;
  try {
    tokens = this->TokenizeQuery(query);
  } catch (std::invalid_argument e) {
    throw InvalidSyntaxException("Invalid item to be tokenized");
  }

  this->CheckSyntax(tokens);

  std::shared_ptr<Context> context = this->FormContext(tokens);

  std::shared_ptr<AExpression> expression_tree =
      this->FormExpressionTree(tokens);

  this->InterpretContext(context, expression_tree);

  /* std::vector<PqlDeclaration> selected_declarations = */
  /*     context->GetSelectedDeclarations(); */
  /* std::vector<std::shared_ptr<Clause>> other_clauses = */
  /*     context->GetOtherClauses(); */

  std::vector<PqlDeclaration> selected_declarations;
  selected_declarations.push_back(PqlDeclaration("p", EntityType::PROCEDURE));

  std::vector<std::shared_ptr<Clause>> other_clauses;
  other_clauses.push_back(std::make_shared<CallsClause>(
        std::make_unique<EntRef>(PqlDeclaration("p", EntityType::PROCEDURE)),
        std::make_unique<EntRef>(PqlDeclaration("p1", EntityType::PROCEDURE)),
        true
        ));

  std::vector<std::vector<std::string>> query_results =
      query_evaluator->evaluateQuery(selected_declarations, other_clauses);

  std::cout << "qpsc0: " << other_clauses.size() << "\n";
  std::cout << "qpsc1: " << query_results.size() << "\n";


  for (std::vector<std::string> result : query_results) {
    std::string result_string;
    for (std::vector<std::string>::iterator it = result.begin();
         it != result.end(); it++) {
      result_string += *it;
      if (it != std::prev(result.end())) {
        result_string += ", ";
      }
    }
    results.push_back(result_string);
  }
}

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
