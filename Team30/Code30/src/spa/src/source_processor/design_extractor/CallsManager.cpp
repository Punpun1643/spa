#include "CallsManager.h"

#include <vector>
CallsManager::CallsManager() {}

void CallsManager::insertCallsStmt(std::vector<std::string> actors,
                                   std::shared_ptr<CallNode> callNode) {
  if (std::find(existingNodes.begin(), existingNodes.end(), callNode) ==
      existingNodes.end()) {
    // node has already been inserted
    return;
  }

  existingNodes.push_back(callNode);
}

void CallsManager::executeCallsGraphTraversal() {}
