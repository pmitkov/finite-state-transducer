#include <catch2/catch_test_macros.hpp>
#include "catch2/matchers/catch_matchers_range_equals.hpp"

#include "e-elimination.h"
#include "functional-test.h"
#include "parser.h"
#include "transducer.h"

using namespace Catch::Matchers;

TEST_CASE("Test basic RegExp", "[prod]") {
    Transducer t = trim(epsilon_elimination(parseRegex("(a:b)")));
    vector<bool> inf_nodes = markInfiniteNodes(t);
    REQUIRE_THAT(accepting(t, "a", inf_nodes), UnorderedRangeEquals({"b"}));
    t = trim(epsilon_elimination(parseRegex("(a:b)|(a:c)")));
    inf_nodes = markInfiniteNodes(t);
    REQUIRE_THAT(accepting(t, "a", inf_nodes), UnorderedRangeEquals({"b", "c"}));
    t = trim(epsilon_elimination(parseRegex("(a:b)|(a:c)|(a:d)|(a:b)")));
    inf_nodes = markInfiniteNodes(t);
    REQUIRE_THAT(accepting(t, "a", inf_nodes), UnorderedRangeEquals({"b", "c", "d"}));
}

TEST_CASE("Test nested RegExp", "[prod]") {
    Transducer t = trim(epsilon_elimination(parseRegex("((a:b)*(b:c))")));
    vector<bool> inf_nodes = markInfiniteNodes(t);
    REQUIRE_THAT(accepting(t, "b", inf_nodes), UnorderedRangeEquals({"c"}));
    REQUIRE_THAT(accepting(t, "ab", inf_nodes), UnorderedRangeEquals({"bc"}));
    REQUIRE_THAT(accepting(t, "aab", inf_nodes), UnorderedRangeEquals({"bbc"}));
    REQUIRE_THAT(accepting(t, "aaab", inf_nodes), UnorderedRangeEquals({"bbbc"}));
    REQUIRE(accepting(t, "a", inf_nodes).empty());

    t = trim(epsilon_elimination(parseRegex("((((a:b)(b:c))|(a:d)*))*")));
    inf_nodes = markInfiniteNodes(t);
    REQUIRE_THAT(accepting(t, "ab", inf_nodes), UnorderedRangeEquals({"bc"}));
    REQUIRE_THAT(accepting(t, "abab", inf_nodes), UnorderedRangeEquals({"bcbc"}));
    REQUIRE_THAT(accepting(t, "abab", inf_nodes), UnorderedRangeEquals({"bcbc"}));
    REQUIRE_THAT(accepting(t, "", inf_nodes), UnorderedRangeEquals({""}));
    REQUIRE_THAT(accepting(t, "a", inf_nodes), UnorderedRangeEquals({"d"}));
    REQUIRE_THAT(accepting(t, "aaaaaa", inf_nodes), UnorderedRangeEquals({"dddddd"}));

    t = trim(epsilon_elimination(parseRegex("((a:b)*(b:b)(d:d))((b:b)*)")));
    inf_nodes = markInfiniteNodes(t);
    REQUIRE_THAT(accepting(t, "bd", inf_nodes), UnorderedRangeEquals({"bd"}));
    REQUIRE_THAT(accepting(t, "abd", inf_nodes), UnorderedRangeEquals({"bbd"}));
    REQUIRE_THAT(accepting(t, "aabd", inf_nodes), UnorderedRangeEquals({"bbbd"}));
    REQUIRE(accepting(t, "aaaab", inf_nodes).empty());
    REQUIRE_THAT(accepting(t, "aaaabd", inf_nodes), UnorderedRangeEquals({"bbbbbd"}));
    REQUIRE_THAT(accepting(t, "bdbb", inf_nodes), UnorderedRangeEquals({"bdbb"}));
}

TEST_CASE("Test RegExp with infinitely ambiguous domain", "[prod]") {
    Transducer t = trim(epsilon_elimination(parseRegex("($:a)*|(a:b)|(a:c)|(c:d)")));
    vector<bool> inf_nodes = markInfiniteNodes(t);
    const auto& res = accepting(t, "", inf_nodes);
    for (auto& r : res) {
        printf("!!! %s\n", r.c_str());
    }
    REQUIRE_THAT(accepting(t, "a", inf_nodes), UnorderedRangeEquals({"b", "c"}));
    REQUIRE_THAT(accepting(t, "c", inf_nodes), UnorderedRangeEquals({"d"}));
}

TEST_CASE("Test concatenation", "[prod]") {
    Transducer t = trim(epsilon_elimination(parseRegex("(a:b)(a:c)(b:c)")));
    vector<bool> inf_nodes = markInfiniteNodes(t);
    REQUIRE_THAT(accepting(t, "aab", inf_nodes), UnorderedRangeEquals({"bcc"}));
}

TEST_CASE("Test with complex RegExp", "[prod]") {

}