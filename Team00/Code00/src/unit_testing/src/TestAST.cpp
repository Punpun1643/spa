
#include "TNode.h"

#include "catch.hpp"
using namespace std;
void require(bool b) {
    REQUIRE(b);
}

TEST_CASE("1st Test") {

    TNode T;
	
	
	
    require(1 == 1);
}

TEST_CASE("kw\'s test case") {

  TNode T;

  require(1 + 6 == 7);
}
