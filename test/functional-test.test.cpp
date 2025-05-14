#include <catch2/catch_test_macros.hpp>

#include "e-elimination.h"
#include "functional-test.h"
#include "parser.h"
#include "transducer.h"
#include "trim.h"

TEST_CASE("Test functionality test", "[prod]") {
    Transducer t = trim(epsilon_elimination(parseRegex("(a:b)")));
    vector<bool> inf_nodes = markInfiniteNodes(t);
    REQUIRE(checkFunctionality(t, inf_nodes));
    t = trim(epsilon_elimination(parseRegex("(a:b)|(b:c)")));
    inf_nodes = markInfiniteNodes(t);
    REQUIRE(checkFunctionality(t, inf_nodes));
    t = trim(epsilon_elimination(parseRegex("(a:b)|(b:c)*")));
    inf_nodes = markInfiniteNodes(t);
    REQUIRE(checkFunctionality(t, inf_nodes));
    t = trim(epsilon_elimination(parseRegex("(a:b)|(a:c)")));
    inf_nodes = markInfiniteNodes(t);
    REQUIRE_FALSE(checkFunctionality(t, inf_nodes));
    t = trim(epsilon_elimination(parseRegex("($:b)|($:c)")));
    inf_nodes = markInfiniteNodes(t);
    REQUIRE_FALSE(checkFunctionality(t, inf_nodes));
}