#pragma once

#include <memory>
#include <string>
#include <vector>

#include "ANode.h"

// declare the class ProcedureNode
class ProcedureNode : public ANode {
 public:
    ProcedureNode(std::string procedureName);
    ~ProcedureNode() = default;

  private:
    std::string procedureName;
};
