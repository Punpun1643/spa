#include "SelectExpression.h"

#include <cassert>

SelectExpression::SelectExpression(std::string synonym, bool is_boolean) {
  if (is_boolean) {
    assert(synonym == "BOOLEAN");
  }
  this->synonym = synonym;
  this->is_boolean = is_boolean;
};

std::string SelectExpression::GetSynonym() { return this->synonym; }
bool SelectExpression::IsBoolean() { return this->is_boolean; }

void SelectExpression::acceptInterpreter(QueryInterpreter& interpreter) {
  interpreter.Interpret(
      std::dynamic_pointer_cast<SelectExpression>(shared_from_this()));
}
