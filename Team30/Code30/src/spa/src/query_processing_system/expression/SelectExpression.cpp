#include "SelectExpression.h"

#include <cassert>

SelectExpression::SelectExpression(std::string synonym, AttrType attr_type,
                                   bool is_boolean) {
  assert(!is_boolean);  // the other constructor shouldve been called
  this->synonym = synonym;
  this->attr_type = attr_type;
  this->is_boolean = is_boolean;
};

SelectExpression::SelectExpression(bool is_boolean) {
  this->is_boolean = is_boolean;
}

std::string SelectExpression::GetSynonym() {
  return this->synonym;
}
AttrType SelectExpression::GetAttrType() {
  return this->attr_type;
}
bool SelectExpression::IsBoolean() {
  return this->is_boolean;
}

void SelectExpression::acceptInterpreter(QueryInterpreter& interpreter) {
  interpreter.Interpret(
      std::dynamic_pointer_cast<SelectExpression>(shared_from_this()));
}
