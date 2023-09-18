#pragma once

#include "common/Clause.h"
#include "expression/QueryExpression.h"
#include "parser/ExpressionTreeBuilder.h"
#include "parser/SyntaxChecker.h"

class QPSController {
 public:
  QPSController();

  /* std::vector<std::shared_ptr<Clause>> ParseAndGetClauses(std::string query_file_name); */
  std::vector<std::unique_ptr<Clause>> ParseAndGetClauses(std::string query);
  std::vector<std::unique_ptr<Clause>> ParseAndGetClauses(std::vector<std::shared_ptr<Token>> tokens);

 private:
  std::vector<std::shared_ptr<Token>> tokens;
  std::vector<std::unique_ptr<Clause>> Parse();
};
