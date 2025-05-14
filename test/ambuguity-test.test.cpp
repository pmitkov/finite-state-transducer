#include <catch2/catch_test_macros.hpp>

#include "e-elimination.h"
#include "functional-test.h"
#include "parser.h"
#include "transducer.h"
#include "trim.h"

TEST_CASE("Test ambiguity test", "[prod]") {
    Transducer t = trim(epsilon_elimination(parseRegex("($:a)*")));
    REQUIRE(checkInfinitelyAmbiguous(t));
    t = trim(epsilon_elimination(parseRegex("($:a)*|(a:b)")));
    REQUIRE(checkInfinitelyAmbiguous(t));
    t = trim(epsilon_elimination(parseRegex("(a:b)")));
    REQUIRE_FALSE(checkInfinitelyAmbiguous(t));
    t = trim(epsilon_elimination(parseRegex("(a:b)*")));
    REQUIRE_FALSE(checkInfinitelyAmbiguous(t));
}