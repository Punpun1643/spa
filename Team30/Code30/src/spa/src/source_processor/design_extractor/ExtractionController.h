#pragma once

#include <memory>
#include <vector>

#include "IExtractionController.h"
#include "IDesignExtractor.h"
#include "../node/ANode.h"

class ExtractionController : public IExtractionController {
 public:
  explicit ExtractionController();

  void executeExtract(std::shared_ptr<ANode> node);

  ~ExtractionController() = default;

 private:
  std::vector<std::shared_ptr<IDesignExtractor>> extractors;
};