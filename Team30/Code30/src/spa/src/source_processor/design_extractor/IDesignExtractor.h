#pragma once

#include <memory>

#include "../node/ProgramNode.h"

class IDesignExtractor {
public:
    virtual void extractFromProgram(std::shared_ptr<ProgramNode> node) = 0;
    virtual ~IDesignExtractor() = default;
};