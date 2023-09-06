#pragma once

#include <memory>

class IDesignExtractor;

class ANode : public std::enable_shared_from_this<ANode> {
public:
    ANode();
    virtual void accept(IDesignExtractor &designExtractor) = 0;
    virtual ~ANode() = default;
};