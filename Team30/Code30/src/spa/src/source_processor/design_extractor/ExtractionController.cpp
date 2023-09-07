#pragma once

#include "IEntityExtractor.h"
#include "IFollowsExtractor.h"
#include "IExtractionController.h"
#include "IModifiesExtractor.h"
#include "IParentExtractor.h"
#include "IUsesExtractor.h"

class ExtractionController : public IExtractionController {
 public:
  ExtractionController() {
    extractors.push_back(std::make_shared<FollowsExtractor>());
    extractors.push_back(std::make_shared<ParentExtractor>());
    extractors.push_back(std::make_shared<UsesExtractor>());
    extractors.push_back(std::make_shared<ModifiesExtractor>());
    extractors.push_back(std::make_shared<EntityExtractor>());
  }

  void executeExtract(std::shared_ptr<TNode> node) {
    for (std::shared_ptr<IDesignExtractor> e : extractors) {
      node->accept(e);
    }
    std::vector<std::shared_ptr<TNode>> children = node->getChildren();
    if (children != nullptr && !children.empty()) {
      for (std::shared_ptr<TNode> child : children) {
        executeExtract(child);
      }
    }
  }

 private:
  std::vector<std::shared_ptr<IDesignExtractor>> extractors;
};