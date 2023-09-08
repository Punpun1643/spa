#pragma once

#include <memory>
#include <vector>

#include "../node/ANode.h"
#include "IDesignExtractor.h"

class IExtractionController {
 public:
  virtual void executeExtract(std::shared_ptr<ANode> node) = 0;

  virtual ~IExtractionController() = default;

 protected:
  std::vector<std::shared_ptr<IDesignExtractor>> extractors;
};