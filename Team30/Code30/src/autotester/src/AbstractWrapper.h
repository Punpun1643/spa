#ifndef ABSTRACTWRAPPER_H
#define ABSTRACTWRAPPER_H

#include <iostream>
#include <list>
#include <string>

class AbstractWrapper {
 public:
  static bool volatile GlobalStop;
  // method for parsing the SIMPLE source
  virtual void parse(std::string filename) = 0;
  // method for evaluating a query
  virtual void evaluate(std::string query, std::list<std::string>& results) = 0;
};

class WrapperFactory {
 public:
  // static method for creating a wrapper
  static AbstractWrapper* createWrapper();

 private:
  // the following three will prevent the creation of WrapperFactory objects
  WrapperFactory();
  WrapperFactory(WrapperFactory const&);
  WrapperFactory& operator=(WrapperFactory const&);

  // the Wrapper object
  static AbstractWrapper* wrapper;
};

#endif
