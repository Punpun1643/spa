#include "CallStmtCacheObject.h"

#include <iterator>

CallStmtCacheObject::CallStmtCacheObject(std::vector<std::string> actors,
                                         std::shared_ptr<CallNode> callNode) {
  copy(actors.begin(), actors.end(), back_inserter(this->actors));
  this->callNode = callNode;
}

std::vector<std::string> CallStmtCacheObject::getActors() { 
    return actors; 
}

std::shared_ptr<CallNode> CallStmtCacheObject::getCallNode() {
  return callNode;
};

bool CallStmtCacheObject::updateVars(std::vector<std::string>> newVars) { 
    int prevLength = vars.size();
    // combine newVars with vars;
    // if new length same as old length return false
    
}
