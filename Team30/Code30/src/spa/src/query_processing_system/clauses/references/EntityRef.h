#pragma once

class Node {}; // TEMP definition until parser team makes theirs

class EntityRef {
public:
    virtual bool isMatching(Node entity_node) = 0;
    // might change the types if need be
};