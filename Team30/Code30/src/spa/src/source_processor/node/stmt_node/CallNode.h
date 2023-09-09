#pragma once

#include <string>

#include "StmtNode.h"
#include "StmtType.h"

class CallNode : public StmtNode {
 public:
  explicit CallNode(int stmtIndex, StmtType stmtType, std::string procName);

  void accept(IDesignExtractor& designExtractor) override;

  std::string const& getProcName() const;

  ~CallNode() = default;

 private:
  std::string const procName;
};