#pragma once

#include <memory>
#include <vector>

#include "ANode.h"
#include "ProcedureNode.h"

class ProgramNode : public ANode {
 public:
    ProgramNode(std::vector<std::shared_ptr<ProcedureNode>> procedures);
    ~ProgramNode() = default;

 private:
    std::vector<std::shared_ptr<ProcedureNode>> procedures;
};