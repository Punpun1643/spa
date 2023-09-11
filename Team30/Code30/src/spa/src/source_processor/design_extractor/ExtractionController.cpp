#pragma once

#include "EntityExtractor.h"
#include "FollowsExtractor.h"
#include "IExtractionController.h"
#include "ModifiesExtractor.h"
#include "ParentExtractor.h"
#include "UsesExtractor.h"

class ExtractionController : public IExtractionController {
 public:
  ExtractionController() {
    extractors.push_back(std::make_shared<FollowsExtractor>());
    extractors.push_back(std::make_shared<ParentExtractor>());
    extractors.push_back(std::make_shared<UsesExtractor>());
    extractors.push_back(std::make_shared<ModifiesExtractor>());
    extractors.push_back(std::make_shared<EntityExtractor>());
  }

  void executeExtract(std::shared_ptr<ANode> node) {
    for (std::shared_ptr<IDesignExtractor> e : extractors) {
      node->accept(*e);
    }
    std::vector<std::shared_ptr<ANode>> children = node->getChildren<ANode>();
    if (!children.empty()) {
      for (std::shared_ptr<ANode> child : children) {
        executeExtract(child);
      }
    }
  }

 private:
  std::vector<std::shared_ptr<IDesignExtractor>> extractors;
};