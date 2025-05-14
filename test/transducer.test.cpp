#include <fstream>
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
    REQUIRE_THAT(accepting(t, "a", inf_nodes), UnorderedRangeEquals({"b", "c"}));
    REQUIRE_THAT(accepting(t, "c", inf_nodes), UnorderedRangeEquals({"d"}));
}

TEST_CASE("Test concatenation", "[prod]") {
    Transducer t = trim(epsilon_elimination(parseRegex("(a:b)(a:c)(b:c)")));
    vector<bool> inf_nodes = markInfiniteNodes(t);
    REQUIRE_THAT(accepting(t, "aab", inf_nodes), UnorderedRangeEquals({"bcc"}));
}

string replacePattern(const string& s, const vector<string>& args) {
    std::string result = s;
    size_t pos = 0;
    size_t index = 0;
    while ((pos = result.find("{}", pos)) != std::string::npos && index < args.size()) {
        result.replace(pos, 2, args[index++]);
        pos += args[index - 1].length();
    }
    return result;
}

string languagePlus(const string& s) {
    return replacePattern("({})({})*", {s, s});
}

vector<string> readFile(const string& path) {
    vector<string> lines;
    ifstream file(path);
    if (!file.is_open()) {
        throw runtime_error("Could not open file " + path);
    }
    string line;
    while (getline(file, line)) {
        lines.push_back(line);
    }

    file.close();
    return lines;

}


TEST_CASE("Test with complex RegExp", "[prod]") {
    string ones = "(I:1)|(II:2)|(III:3)|(IV:4)|(V:5)|(VI:6)|(VII:7)|(VIII:8)|(IX:9)";
    string ones0 = replacePattern("{}|($:0)", {ones});

    string tens = "(X:1)|(XX:2)|(XXX:3)|(XL:4)|(L:5)|(LX:6)|(LXX:7)|(LXXX:8)|(XC:9)";
    string tens0 = replacePattern("{}|($:0)", {tens});

    string hundreds = "(C:1)|(CC:2)|(CCC:3)|(CD:4)|(D:5)|(DC:6)|(DCC:7)|(DCCC:8)|(CM:9)";
    string hundreds0 = replacePattern("{}|($:0)", {hundreds});

    string thousands = "(M:1)|(MM:2)|(MMM:3)";
    string thousands1 = "(M:1)|((MM:$)($:0)*($:2))|(MMM:3)";
    string thousands2 = "(M:1)|(MM:2)|((MMM:$)($:0)*($:3))";

    string n1_99 = replacePattern("({})|(({})({}))", {ones, tens, ones0});
    string n00_99 = replacePattern("({})({})", {tens0, ones0});

    string n1_999 = replacePattern("({})|(({})({}))", {n1_99, hundreds, n00_99});
    string n000_999 = replacePattern("({})({})", {hundreds0, n00_99});

    string n = replacePattern("({})|(({})({}))", {n1_999, thousands, n000_999});
    string n1 = replacePattern("({})|(({})({}))", {n1_999, thousands1, n000_999});
    string n2 = replacePattern("({})|(({})({}))|(({})({})({}))", {n1_999, thousands2, n000_999, thousands1, "($:00)", ones});

    string nPlus = languagePlus(n);
    Transducer t = trim(epsilon_elimination(parseRegex(nPlus)));
    vector<bool> inf_nodes = markInfiniteNodes(t);
    REQUIRE_FALSE(checkInfinitelyAmbiguous(t));
    REQUIRE_FALSE(checkFunctionality(t,inf_nodes));

    string n1Plus = languagePlus(n1);
    t = trim(epsilon_elimination(parseRegex(n1Plus)));
    inf_nodes = markInfiniteNodes(t);
    REQUIRE(checkInfinitelyAmbiguous(t));
    vector<string> words = readFile("./n1+_MCMXXLIMCMX.txt");
    REQUIRE_THAT(accepting(t, "MCMXXLIMCMX", inf_nodes), UnorderedRangeEquals(words));

    string n2Plus = languagePlus(n2);
    t = trim(epsilon_elimination(parseRegex(n2Plus)));
    inf_nodes = markInfiniteNodes(t);
    REQUIRE(checkInfinitelyAmbiguous(t));
    words = readFile("./n2+_MCMXXLIMCMXIMMDXCMMCMMD.txt");
    REQUIRE_THAT(accepting(t, "MCMXXLIMCMXIMMDXCMMCMMD", inf_nodes), UnorderedRangeEquals(words));
}