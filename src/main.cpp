#include <stdio.h>
#include <vector>
#include <unordered_set>
#include <string>

#include "transducer.h"
#include "trim.h"
#include "e-elimination.h"
#include "functional-test.h"
#include "parser.h"
            
#define X first
#define Y second

using namespace std;

constexpr int BUFF_SIZE = (1 << 24);

char BUFF[BUFF_SIZE];

[[noreturn]] int main() {
    scanf(" %s", BUFF);
    const string s(BUFF);
    Transducer t = parseRegex(s);
    t = epsilon_elimination(t);
    t = trim(t);
    vector<bool> V = markInfiniteNodes(t);
    if (checkInfinitelyAmbiguous(t)) {
        printf("Language is infinitely ambiguous\n");
    } else {
        if (checkFunctionality(t, V)) {
        printf("Language is functional\n");
        } else {
            printf("Language is not functional\n");
        }
    }
    while (true) {
        scanf(" %s", BUFF);
        const unordered_set<string> res = accepting(t, string(BUFF), V);
        printf("Got %lu results\n", res.size());
        for (const auto& s : res) {
            printf("%s\n", s.c_str());
        }
    }
}