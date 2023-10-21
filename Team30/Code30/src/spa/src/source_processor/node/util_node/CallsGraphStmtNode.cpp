#include "CallsGraphStmtNode.h"

#include <algorithm>
#include <iterator>
#include <string>
#include <vector>

CallsGraphStmtNode::CallsGraphStmtNode(std::vector<std::string> actors,
                                       std::shared_ptr<CallNode> call_node)
    : call_node(call_node) {
  copy(actors.begin(), actors.end(), back_inserter(this->actors));
  this->call_node = call_node;
}

std::vector<std::string> CallsGraphStmtNode::GetActors() {
  return actors;
}

std::shared_ptr<CallNode> CallsGraphStmtNode::GetCallNode() {
  return call_node;
}
