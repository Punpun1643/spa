#include "CallsGraphStmtNode.h"

#include <iterator>
#include <algorithm>
#include <string>
#include <vector>

CallsGraphStmtNode::CallsGraphStmtNode(std::vector<std::string> actors,
                                       std::shared_ptr<CallNode> callNode)
    : callNode(callNode) {
  copy(actors.begin(), actors.end(), back_inserter(this->actors));
  this->callNode = callNode;
}

std::vector<std::string> CallsGraphStmtNode::getActors() {
  return actors;
}

std::shared_ptr<CallNode> CallsGraphStmtNode::getCallNode() {
  return callNode;
}
