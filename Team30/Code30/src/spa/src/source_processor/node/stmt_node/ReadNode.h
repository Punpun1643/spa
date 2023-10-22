#pragma once

#include <string>

#include "StmtNode.h"
#include "StmtType.h"

class ReadNode : public StmtNode {
 public:
  explicit ReadNode(int stmt_index, StmtType stmt_type, std::string var_name);

  void Accept(IDesignExtractor& design_extractor) override;

  std::string const& GetVarName() const;

  ~ReadNode() = default;

 private:
  std::string const var_name;
};
