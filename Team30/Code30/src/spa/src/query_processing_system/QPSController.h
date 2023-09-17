#pragma once

#include "common/Clause.h"
#include "expression/QueryExpression.h"
#include "parser/ExpressionTreeBuilder.h"
#include "parser/SyntaxChecker.h"

class QPSController {
 public:
  QPSController(std::vector<std::shared_ptr<Token>> tokens);

  std::vector<std::shared_ptr<Clause>> ParseAndGetClauses();

 private:
  std::vector<std::shared_ptr<Token>> tokens;
};
