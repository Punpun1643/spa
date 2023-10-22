#pragma once

#include <string>

#include "AExpression.h"
#include "query_processing_system/types/AttrType.h"

class SelectExpression : public AExpression {
 public:
  SelectExpression(std::string synonym, AttrType attr_type, bool is_boolean);
  explicit SelectExpression(bool is_boolean);
  AttrType GetAttrType();
  std::string GetSynonym();
  bool IsBoolean();
  void acceptInterpreter(QueryInterpreter& interpreter) override;

  ~SelectExpression() = default;

 private:
  bool is_boolean;
  AttrType attr_type;
  std::string synonym;
};
