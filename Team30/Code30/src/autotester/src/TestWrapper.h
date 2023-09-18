#ifndef TESTWRAPPER_H
#define TESTWRAPPER_H

#include <string>
#include <iostream>
#include <list>

// include your other headers here
#include "AbstractWrapper.h"
#include "../../spa/src/program_knowledge_base/PKB.h"
#include "../../spa/src/source_processor/SpController.h"
#include "../../spa/src/query_processing_system/QPSController.h"
#include "../../spa/src/query_processing_system/evaluator/QueryEvaluator.h"

class TestWrapper : public AbstractWrapper {
 public:
  // default constructor
  TestWrapper();

  // destructor
  ~TestWrapper();

  // method for parsing the SIMPLE source
  virtual void parse(std::string filename);

  // method for evaluating a query
  virtual void evaluate(std::string query, std::list<std::string>& results);

 private:
  std::shared_ptr<PKB> pkb;
  std::shared_ptr<SpController> sp_controller;
  std::shared_ptr<QPSController> qps_controller;
  std::shared_ptr<QueryEvaluator> query_evaluator;
};

#endif
