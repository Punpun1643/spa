#pragma once
#include <string>
#include "EntityRef.h"

class EntIdentifierRef: EntityRef {
private:
    std::string entity_identifier;
public:
    bool isMatching(Node node);
};
