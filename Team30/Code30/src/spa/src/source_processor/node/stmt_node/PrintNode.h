#pragma once

#include <string>

#include "StmtNode.h"
#include "StmtType.h"

class PrintNode : public StmtNode {
 public:
  explicit PrintNode(int stmtIndex, StmtType stmtType, std::string varName);

  void accept(IDesignExtractor& designExtractor) override;

  std::string const& getVarName() const;

  ~PrintNode() = default;

 private:
  std::string const varName;
};