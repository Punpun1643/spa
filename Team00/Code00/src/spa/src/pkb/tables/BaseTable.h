#pragma once

#include <any>
#include <iostream>
#include <stdio.h>

class BaseTable {
  virtual bool insertData(std::any key, std::any value);
  virtual std::any queryData(std::any key);
};