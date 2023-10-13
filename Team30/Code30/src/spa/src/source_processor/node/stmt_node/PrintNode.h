#pragma once

#include <string>

#include "StmtNode.h"
#include "StmtType.h"

class PrintNode : public StmtNode {
 public:
  explicit PrintNode(int stmtIndex, StmtType stmtType, std::string varName);

  void Accept(IDesignExtractor& designExtractor) override;

  std::string const& GetVarName() const;

  ~PrintNode() = default;

 private:
  std::string const varName;
};