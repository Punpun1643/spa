#include "PKB.h"

#include <stdio.h>

#include <iostream>
#include <string>
#include <utility>
#include <vector>

#include "program_knowledge_base/EntityDatabase.h"
#include "program_knowledge_base/RelDatabase.h"


class PKB {
  EntityDatabase* entData;
  RelDatabase* relData;

  //TODO: Separate out Insertion and Query API Logic (2 different Handler Classes)
  //TODO: Implement actual API Logic.

  public:
  PKB() {
    entData = new EntityDatabase();
    relData = new RelDatabase();
  }
};