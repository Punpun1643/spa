#include "CallsGraphStmtNode.h"

#include <iterator>

CallsGraphStmtNode::CallsGraphStmtNode(std::vector<std::string> actors,
                                       std::shared_ptr<CallNode> callNode)
    : callNode(callNode) {
  copy(actors.begin(), actors.end(), back_inserter(this->actors));
  this->callNode = callNode;
}

std::vector<std::string> CallsGraphStmtNode::getActors() { return actors; }
