#include <any>
#include <iostream>
#include <stdio.h>

#include "BaseTable.h"

class BaseTable {
  virtual bool insertData(std::any key, std::any value) {
    return true;
  }
  virtual std::any queryData(std::any key) {
    return -1;
  }
};