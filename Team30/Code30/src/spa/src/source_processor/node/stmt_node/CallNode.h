#pragma once

#include <string>

#include "StmtNode.h"
#include "StmtType.h"

class CallNode : public StmtNode {
 public:
  explicit CallNode(int stmt_index, StmtType stmt_type, std::string proc_name);

  void Accept(IDesignExtractor& design_extractor) override;

  std::string const& GetProcName() const;

  ~CallNode() = default;

 private:
  std::string const proc_name;
};
