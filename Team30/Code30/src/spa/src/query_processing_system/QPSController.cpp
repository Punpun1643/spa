#include "QPSController.h"

#include <memory>
#include <sstream>
#include <string>
#include <utility>
#include <vector>

#include "../shared/tokenizer/Tokenizer.h"
#include "clauses/SuchThatClause.h"
#include "clauses/UsesSClause.h"
#include "exceptions/InvalidSyntaxException.h"
#include "parser/ContextBuilder.h"

QPSController::QPSController() {}

std::string const QPSController::BOOLEAN_TRUE_RESULT = "TRUE";
std::string const QPSController::BOOLEAN_FALSE_RESULT = "FALSE";
std::string const QPSController::RESULT_STRING_DELIMITER = " ";

void QPSController::HandleQuery(
    std::string& query, std::list<std::string>& results,
    std::shared_ptr<QueryEvaluator> query_evaluator) {
  std::vector<std::shared_ptr<Token>> tokens;
  try {
    tokens = this->TokenizeQuery(query);
  } catch (std::invalid_argument e) {
    throw InvalidSyntaxException("Invalid item to be tokenized");
  }

  std::pair<std::vector<AttrRef>, std::vector<std::shared_ptr<Clause>>>
      clauses = this->TokensToClauses(tokens);

  std::vector<AttrRef> selected_attr_refs = clauses.first;

  std::vector<std::shared_ptr<Clause>> other_clauses = clauses.second;

  if (selected_attr_refs.empty()) {
    bool query_results = query_evaluator->EvaluateQuery(other_clauses);
    results.push_back(query_results ? this->BOOLEAN_TRUE_RESULT
                                    : this->BOOLEAN_FALSE_RESULT);
  } else {
    std::vector<std::vector<std::string>> query_results =
        query_evaluator->EvaluateQuery(selected_attr_refs, other_clauses);
    for (std::vector<std::string> result : query_results) {
      std::string result_string;
      for (std::vector<std::string>::iterator it = result.begin();
           it != result.end(); it++) {
        result_string += *it;
        if (it != std::prev(result.end())) {
          result_string += this->RESULT_STRING_DELIMITER;
        }
      }
      results.push_back(result_string);
    }
  }
}

std::pair<std::vector<AttrRef>, std::vector<std::shared_ptr<Clause>>>
QPSController::TokensToClauses(std::vector<std::shared_ptr<Token>> tokens) {
  this->CheckSyntax(tokens);

  std::shared_ptr<Context> context = this->FormContext(tokens);

  std::shared_ptr<AExpression> expression_tree =
      this->FormExpressionTree(tokens, context);

  this->InterpretContext(context, expression_tree);

  std::vector<AttrRef> selected_attr_refs = context->GetSelectedAttrRefs();
  std::vector<std::shared_ptr<Clause>> other_clauses =
      context->GetPriorityClauses();
  return std::make_pair(selected_attr_refs, other_clauses);
}

void QPSController::CheckSyntax(std::vector<std::shared_ptr<Token>> tokens) {
  std::unique_ptr<SyntaxChecker> syntax_checker =
      std::make_unique<SyntaxChecker>(tokens);
  syntax_checker->Parse();
}

std::shared_ptr<Context> QPSController::FormContext(
    std::vector<std::shared_ptr<Token>> tokens) {
  ContextBuilder context_builder(tokens);
  context_builder.Parse();
  return context_builder.GetContext();
}

std::shared_ptr<AExpression> QPSController::FormExpressionTree(
    std::vector<std::shared_ptr<Token>> tokens,
    std::shared_ptr<Context> context) {
  std::unique_ptr<ExpressionTreeBuilder> expression_tree_builder =
      std::make_unique<ExpressionTreeBuilder>(tokens, context);
  expression_tree_builder->Parse();
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
  return tokenizer.Tokenize();
}
