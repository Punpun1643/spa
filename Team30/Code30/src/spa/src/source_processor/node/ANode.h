#pragma once

#include <memory>
#include <vector>

class IDesignExtractor;

class ANode : public std::enable_shared_from_this<ANode> {
public:
    ANode();

    template<typename T, typename std::enable_if<std::is_base_of<ANode, T>::value>::type * = nullptr>
    std::vector <std::shared_ptr<T>> getChildren() {
        // note: return empty vector by default,
        // nodes that have children should override this method
        return std::vector < std::shared_ptr < T >> ();
    }

    virtual void accept(IDesignExtractor &designExtractor) = 0;

    virtual ~ANode() = default;
};

