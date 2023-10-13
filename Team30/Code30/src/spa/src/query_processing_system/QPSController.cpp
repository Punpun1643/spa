#include "QPSController.h"

#include <iostream>
#include <sstream>
#include <utility>

#include "../shared/tokenizer/Tokenizer.h"
#include "common/SuchThatClause.h"
#include "common/UsesSClause.h"
#include "exceptions/InvalidSyntaxException.h"
#include "parser/ContextBuilder.h"

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

  std::pair<std::vector<PqlDeclaration>, std::vector<std::shared_ptr<Clause>>>
      clauses = this->TokensToClauses(tokens);

  std::vector<PqlDeclaration> selected_declarations = clauses.first;

  // ---Temp addition to make it compile
  std::vector<AttrRef> selected_attr_refs = {};
  for (auto &decl: selected_declarations) {
    selected_attr_refs.push_back(AttrRef(decl));
  }
  // ---

  std::vector<std::shared_ptr<Clause>> other_clauses = clauses.second;

  if (selected_attr_refs.empty()) {
    bool query_results = query_evaluator->EvaluateQuery(other_clauses);
    results.push_back(query_results ? "TRUE" : "FALSE");
  } else {
    std::vector<std::vector<std::string>> query_results =
        query_evaluator->EvaluateQuery(selected_attr_refs, other_clauses);
    for (std::vector<std::string> result : query_results) {
      std::string result_string;
      for (std::vector<std::string>::iterator it = result.begin();
           it != result.end(); it++) {
        result_string += *it;
        if (it != std::prev(result.end())) {
          result_string += " ";
        }
      }
      results.push_back(result_string);
    }
  }
}

std::pair<std::vector<PqlDeclaration>, std::vector<std::shared_ptr<Clause>>>
QPSController::TokensToClauses(std::vector<std::shared_ptr<Token>> tokens) {
  this->CheckSyntax(tokens);

  std::shared_ptr<Context> context = this->FormContext(tokens);

  std::shared_ptr<AExpression> expression_tree =
      this->FormExpressionTree(tokens, context);

  this->InterpretContext(context, expression_tree);

  std::vector<PqlDeclaration> selected_declarations =
      context->GetSelectedDeclarations();
  std::vector<std::shared_ptr<Clause>> other_clauses =
      context->GetOtherClauses();
  return std::make_pair(selected_declarations, other_clauses);
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
    std::vector<std::shared_ptr<Token>> tokens,
    std::shared_ptr<Context> context) {
  std::unique_ptr<ExpressionTreeBuilder> expression_tree_builder =
      std::make_unique<ExpressionTreeBuilder>(tokens, context);
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
