#pragma once

#include <memory>
#include <vector>

class IDesignExtractor;

class ANode : public std::enable_shared_from_this<ANode> {
 public:
  ANode();

  template <typename T, typename std::enable_if<
                            std::is_base_of<ANode, T>::value>::type* = nullptr>
  std::vector<std::shared_ptr<T>> GetChildren();

  virtual void Accept(IDesignExtractor& design_extractor) = 0;

  virtual ~ANode() = default;
};
