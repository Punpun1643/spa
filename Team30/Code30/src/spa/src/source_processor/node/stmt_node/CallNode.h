#pragma once

#include <string>

#include "StmtNode.h"
#include "StmtType.h"

class CallNode : public StmtNode {
 public:
  explicit CallNode(int stmtIndex, StmtType stmtType, std::string procName);

  void Accept(IDesignExtractor& designExtractor) override;

  std::string const& GetProcName() const;

  ~CallNode() = default;

 private:
  std::string const procName;
};